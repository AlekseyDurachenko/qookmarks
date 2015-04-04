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
#include "cwebpagescreenshot.h"
#include <QDir>
#include <QDebug>
#include <QStack>
#include <QWebElement>
#include <QWebFrame>
#include <QPainter>


CWebPageScreenshot::CWebPageScreenshot(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_webPage = 0;
    m_error = NoError;

    m_isRunning = false;
    m_isFinished = false;
}

CWebPageScreenshot::~CWebPageScreenshot()
{
    abort();
}

void CWebPageScreenshot::setUrl(const QUrl &url)
{
    m_url = url;
}

void CWebPageScreenshot::setScreenshotSize(const QSize &size)
{
    m_screenshotSize = size;
}

void CWebPageScreenshot::start()
{
    if (isRunning())
        return;

    m_error = NoError;
    m_isRunning = true;
    m_isFinished = false;

    m_webPage = new QWebPage(this);
    m_webPage->setNetworkAccessManager(m_network);
    connect(m_webPage->mainFrame(), SIGNAL(loadFinished(bool)),
            this, SLOT(webpage_loadFinished(bool)));
    connect(m_webPage->mainFrame(), SIGNAL(loadFinished(bool)),
            m_webPage, SLOT(deleteLater()));
    m_webPage->mainFrame()->setUrl(m_url);
}

void CWebPageScreenshot::abort()
{
    if (!isRunning())
        return;

    if (m_webPage)
        m_webPage->triggerAction(QWebPage::Stop);

    m_error = OperationCanceledError;
    m_isRunning = false;
}

QString CWebPageScreenshot::errorString() const
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

void CWebPageScreenshot::webpage_loadFinished(bool ok)
{
    m_webPage->setViewportSize(m_screenshotSize);
    m_webPage->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_webPage->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    m_screenshot = QImage(m_webPage->viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&m_screenshot);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    m_webPage->mainFrame()->render(&painter);
    painter.end();
    //m_screenshot.save("/tmp/my.png");
    //qDebug() << "done!";

    m_webPage = 0;
    m_isRunning = false;
    m_isFinished = true;

    emit finished();
}
