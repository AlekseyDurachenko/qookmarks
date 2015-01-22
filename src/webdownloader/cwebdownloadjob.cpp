// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "cwebdownloadjob.h"
#include <QDebug>
#include <QtXml>
#include <QtWebKit>

CWebDownloadJob::CWebDownloadJob(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_reply = 0;
    m_url = QUrl();
    m_httpErrorCode = 0;
    m_httpErrorString = QString();
}

bool CWebDownloadJob::isFinished() const
{
    if (m_reply)
        return m_reply->isFinished();

    return true;
}

bool CWebDownloadJob::isRunning() const
{
    if (m_reply)
        return m_reply->isRunning();

    return false;
}

QUrl CWebDownloadJob::url() const
{
    return m_url;
}

int CWebDownloadJob::httpError() const
{
    return m_httpErrorCode;
}

QString CWebDownloadJob::httpErrorString() const
{
    return m_httpErrorString;
}

void CWebDownloadJob::start(const QUrl &url)
{
    if (m_reply)
        m_reply->abort();

    m_url = url;
    m_httpErrorCode = 0;
    m_httpErrorString = QString();
    m_reply = m_network->get(QNetworkRequest(url));
    connect(m_reply, SIGNAL(finished()), this, SLOT(slot_reply_finished()));
}

void CWebDownloadJob::abort()
{
    if (m_reply)
        m_reply->abort();
}

void CWebDownloadJob::slot_reply_finished()
{
    if (m_reply->error() != QNetworkReply::OperationCanceledError)
    {
        m_httpErrorCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        m_httpErrorString = m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        QByteArray array = m_reply->readAll();
        webPage = new QWebPage(this);
        QWebFrame *frame = webPage->mainFrame();
        webPage->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
        webPage->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
        webPage->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
        webPage->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
        //frame->load(QUrl("http://ya.ru"));
        connect(frame, SIGNAL(loadFinished(bool)), this, SLOT(slot_load_finished()));
        frame->setHtml(array);
        //frame->setContent(array);
        //frame->setUrl(m_url);
//        qDebug() << frame->toHtml();
//        QWebElement e = frame->documentElement();
//        QStack<QWebElement> stack;

//        while (!e.isNull() || !stack.isEmpty())
//        {
//            if (e.isNull())
//            {
//                e = stack.pop();
//                qDebug() << QString().fill(' ', stack.count()) + "/" + e.tagName();
//                e = e.nextSibling();
//            }
//            else
//            {
//                qDebug() << QString().fill(' ', stack.count()) + e.tagName();
//                stack.push(e);
//                e = e.firstChild();
//            }
//        }

    }
    m_reply->deleteLater();
    m_reply = 0;
    emit finished();
}

void CWebDownloadJob::slot_load_finished()
{
    QWebFrame *frame = webPage->mainFrame();
    QWebElement e = frame->documentElement();
    QStack<QWebElement> stack;
    while (!e.isNull() || !stack.isEmpty())
    {
        if (e.isNull())
        {
            e = stack.pop();
            qDebug() << QString().fill(' ', stack.count()) + "/" + e.tagName();
            e = e.nextSibling();
        }
        else
        {
            qDebug() << QString().fill(' ', stack.count()) + e.tagName();
            stack.push(e);
            e = e.firstChild();
        }
    }

}
