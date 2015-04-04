// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "cwebpagedownloader.h"
#include <QDir>
#include <QDebug>
#include <QStack>
#include <QWebElement>
#include <QWebFrame>


CWebPageDownloader::CWebPageDownloader(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_webPage = 0;
    m_fileSeqNumber = 0;
    m_totalResources = 0;
    m_error = NoError;

    m_isRunning = false;
    m_isFinished = false;
}

CWebPageDownloader::~CWebPageDownloader()
{
    abort();
}

void CWebPageDownloader::setUrl(const QUrl &url)
{
    m_url = url;
}

void CWebPageDownloader::setFileName(const QString &fileName)
{
    m_fileName = fileName;
    m_resourcePath = fileName + ".res";
}

void CWebPageDownloader::start()
{
    if (isRunning())
        return;

    if (!QDir().mkpath(resourcesPath()))
    {
        m_error = FileSystemError;
        m_errorString = tr("can't create the destination directory");
        return;
    }

    m_fileSeqNumber = 0;
    m_totalResources = 0;
    m_error = NoError;
    m_existsResources.clear();
    m_isRunning = true;
    m_isFinished = false;

    // QWebPage is working normally only if we use setUrl() (codepage problems)
    m_webPage = new QWebPage(this);
    m_webPage->setNetworkAccessManager(m_network);
    connect(m_webPage->mainFrame(), SIGNAL(loadFinished(bool)),
            this, SLOT(webpage_loadFinished(bool)));
    connect(m_webPage->mainFrame(), SIGNAL(loadFinished(bool)),
            m_webPage, SLOT(deleteLater()));
    m_webPage->mainFrame()->setUrl(m_url);

    emit downloadProgress(0, 1);
}

void CWebPageDownloader::abort()
{
    if (!isRunning())
        return;

    if (m_webPage)
        m_webPage->triggerAction(QWebPage::Stop);

    foreach (CWebFileDownloader *downloader, m_resources)
        downloader->abort();

    m_error = OperationCanceledError;
    m_isRunning = false;
}

QString CWebPageDownloader::errorString() const
{
    switch (m_error)
    {
    case NoError:
        return tr("success");
    case OperationCanceledError:
        return tr("operation aborted");
    case FileSystemError:
    case OtherError:
        return m_errorString;
    }

    return QString();
}

void CWebPageDownloader::resource_finished()
{
    m_resources.removeAll(qobject_cast<CWebFileDownloader *>(sender()));
    updateProgress();
}

void CWebPageDownloader::webpage_loadFinished(bool ok)
{
    QWebFrame *webFrame = m_webPage->mainFrame();
    m_webPage = 0;

    if (ok == false)
    {
        m_error = OtherError;
        m_errorString = tr("webkit: can't load the page");
        updateProgress();
        return;
    }

    QStack<QWebElement> stack;
    QWebElement e = webFrame->documentElement();
    while (!e.isNull() || !stack.isEmpty())
    {
        if (e.isNull())
        {
            e = stack.pop();
            e = e.nextSibling();
        }
        else
        {
            if (e.tagName().toLower() == "link"
                    && e.attribute("rel").toLower() == "stylesheet")
            {
                e.setAttribute("href", addUrlToDownloader(e.attribute("href"), ".css"));
            }
            else if (e.tagName().toLower() == "script")
            {
                e.setAttribute("src", addUrlToDownloader(e.attribute("src"), ".js"));
            }
            else if (e.tagName().toLower() == "img")
            {
                e.setAttribute("src", addUrlToDownloader(e.attribute("src")));
            }
            else if (e.tagName().toLower() == "meta"
                    && e.attribute("http-equiv").toLower() == "content-type")
            {
                // we save the page as utf8
                e.setAttribute("content", "text/html; charset=utf-8");
            }
            else if (e.tagName().toLower() == "a")
            {
                // convert the relative links to the absolute links
                e.setAttribute("href", m_url.resolved(QUrl(e.attribute("href"))).toString());
            }

            stack.push(e);
            e = e.firstChild();
        }
    }

    // add content type if not exists
    if (webFrame->findAllElements("meta[http-equiv=content-type]").count() == 0)
    {
        QWebElement head = webFrame->findFirstElement("head");
        head.setInnerXml("<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />" + head.toInnerXml());
    }

    // write the modified webpage
    QFile outputFile(m_fileName);
    if (!outputFile.open(QIODevice::WriteOnly))
    {
        m_error = FileSystemError;
        m_errorString = outputFile.errorString();
    }
    QByteArray ba = webFrame->toHtml().toUtf8();
    if (!outputFile.write(ba) != ba.size())
    {
        m_error = FileSystemError;
        m_errorString = outputFile.errorString();
    }

    updateProgress();
}

QString CWebPageDownloader::addUrlToDownloader(const QString &urlstr, const QString &ext)
{
    QUrl url = m_url.resolved(QUrl(urlstr));
    if (!url.isValid())
        return QString();

    if (m_existsResources.contains(url))
        return m_existsResources.value(url);

    // autodetect the extension
    QString extension = ext;
    if (extension.isEmpty())
    {
        extension = urlstr.split("/").last().split(".").last();
        if (extension == urlstr)
            extension = QString();
        else
            extension = "." + extension;
    }

    QString fileName = QString::number(++m_fileSeqNumber) + extension;
    QString fullFileName = resourcesPath() + QDir::separator() + fileName;
    QString relativeFileName = QFileInfo(m_resourcePath).fileName() + QDir::separator() + fileName;
    m_existsResources.insert(url, relativeFileName);

    CWebFileDownloader *downloader = new CWebFileDownloader(m_network, this);
    connect(downloader, SIGNAL(finished()), this, SLOT(resource_finished()));
    connect(downloader, SIGNAL(finished()), downloader, SLOT(deleteLater()));
    downloader->setFileName(fullFileName);
    downloader->setUrl(url);
    downloader->start();
    m_resources << downloader;
    m_totalResources += 1;

    return relativeFileName;
}

void CWebPageDownloader::updateProgress()
{
    if (m_totalResources == 0)
        emit downloadProgress(1, 1);
    else
        emit downloadProgress(m_totalResources-m_resources.count(), m_totalResources);

    if (m_resources.count() == 0)
    {
        m_isRunning = false;
        m_isFinished = true;
        emit finished();
    }
}
