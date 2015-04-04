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
#include "cwebfiledownloader.h"
#include <QNetworkRequest>
#include <QDebug>


CWebFileDownloader::CWebFileDownloader(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_reply = 0;
    m_file = 0;
    m_retryCount = 5;
    m_isAborted = false;
    m_isStarted = false;
    m_isFinished = false;
}

CWebFileDownloader::~CWebFileDownloader()
{
    if (m_reply)
    {
        m_reply->abort();
        m_reply->deleteLater();
    }

    delete m_file;
}

void CWebFileDownloader::setUrl(const QUrl &url)
{
    m_url = url;
}

void CWebFileDownloader::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void CWebFileDownloader::start()
{
    if (isStarted())
        return;

    m_isStarted = true;
    m_isAborted = false;
    m_isFinished = false;

    m_reply = m_network->get(QNetworkRequest(m_url));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SIGNAL(downloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(readyRead()),
            this, SLOT(reply_readyRead()));
    connect(m_reply, SIGNAL(finished()),
            this, SLOT(reply_finished()));
}

void CWebFileDownloader::abort()
{
    if (m_reply)
    {
        m_reply->abort();
        m_isAborted = true;
    }
}

void CWebFileDownloader::reply_readyRead()
{
    if (!m_file && !m_fileName.isEmpty())
    {
        m_file = new QFile(m_fileName);
        m_file->open(QIODevice::WriteOnly);
    }

    const QByteArray buffer = m_reply->readAll();
    if (!m_fileName.isEmpty())
    {
        m_file->write(buffer);
    }
}

QUrl redirectUrl(const QUrl& possibleRedirectUrl,
                               const QUrl& oldRedirectUrl)  {
    QUrl redirectUrl;
    /*
     * Check if the URL is empty and
     * that we aren't being fooled into a infinite redirect loop.
     * We could also keep track of how many redirects we have been to
     * and set a limit to it, but we'll leave that to you.
     */
    if(!possibleRedirectUrl.isEmpty() &&
       possibleRedirectUrl != oldRedirectUrl) {
        redirectUrl = possibleRedirectUrl;
    }
    return redirectUrl;
}

void CWebFileDownloader::reply_finished()
{
    m_isFinished = true;
    m_isStarted = false;

    delete m_file;
    m_file = 0;

    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QVariant possibleRedirectUrl =
                 m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    m_reply->deleteLater();
    m_reply = 0;

    QUrl url = redirectUrl(possibleRedirectUrl.toUrl(), m_url);
    if (!url.isEmpty())
    {
        setUrl(url);
        start();

        //qDebug() << "redirect" << url;
    }
    else
    {
        if (statusCode == 503 && m_retryCount != 0)
        {
            --m_retryCount;
            start();
        }
        else
        {
            //qDebug() << "done!";
            emit finished();
        }
    }
}
