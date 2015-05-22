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
#include "abstractdownloadreply.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "version.h"
#include <QDebug>


AbstractDownloadReply::AbstractDownloadReply(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_fetchData = true;
    m_firstProgressIteration = true;
    m_reply = 0;
    m_error = NoError;
    m_httpStatusCode = 0;
    m_networkError = QNetworkReply::NoError;
}

AbstractDownloadReply::~AbstractDownloadReply()
{
    if (m_reply)
    {
        m_reply->abort();
        m_reply->deleteLater();
    }
}

void AbstractDownloadReply::abort()
{
    m_reply->abort();
}

void AbstractDownloadReply::reply_readyRead()
{
    if (m_reply == 0
            || isRedirectFound()
            || isRetryNeeded()
            || isNotFound()
            || m_reply->error() != QNetworkReply::NoError)
        return;

    if (m_firstProgressIteration)
    {
        m_firstProgressIteration = false;
        processStart();
    }

    if (m_reply != 0)
        processData();
}

void AbstractDownloadReply::reply_finished()
{
    m_reply->deleteLater();

    m_httpStatusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_httpReasonPhrase = m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    m_networkError = m_reply->error();

    foreach (QByteArray name, m_reply->rawHeaderList())
    {
        if (name.toLower() == "content-type")
        {
            QList<QByteArray> parts = m_reply->rawHeader(name).split(';');

            if (parts.count() > 0)
                m_mimeType = parts.at(0).trimmed();

            for (int i = 1; i < parts.count(); ++i)
            {
                QList<QByteArray> keyValue = parts.at(i).split('=');
                if (keyValue.count() != 2)
                    continue;
                if (keyValue.at(0).trimmed().toLower() == "charset")
                    m_charset = keyValue.at(1).trimmed();
            }
        }
    }

    if (m_reply->error() == QNetworkReply::NoError
            && isRedirectFound()
            && m_maxRedirectCount > 0)
    {
        m_request = QNetworkRequest(redirectUrl());
        m_reply = createReply(m_request);
        m_maxRedirectCount -= 1;

        return;
    }

    if (m_reply->error() == QNetworkReply::NoError
            && isRetryNeeded()
            && m_maxRetryCount > 0)
    {
        m_request = QNetworkRequest(m_request.url());
        m_reply = createReply(m_request);
        m_maxRetryCount -= 1;

        return;
    }

    if (m_reply->error() == QNetworkReply::OperationCanceledError
            && m_error == NoError)
        setError(OperationCanceledError, tr("operation canceled"));
    else if (m_reply->error() == QNetworkReply::NoError && isRedirectFound())
        setError(RedirectLoopError, tr("redirect loop detected"));
    else if (m_reply->error() == QNetworkReply::NoError && isRetryNeeded())
        setError(RetryAttemptError, tr("retry attempt ended"));
    else if (m_reply->error() == QNetworkReply::NoError && isNotFound())
        setError(NotFoundError, tr("not found"));
    else if (m_reply->error() != QNetworkReply::NoError)
        setError(NetworkError, m_reply->errorString());

    if (!m_firstProgressIteration)
        processEnd();

    m_reply = 0;

    emit finished();
}

void AbstractDownloadReply::processStart()
{
}

void AbstractDownloadReply::processData()
{
}

void AbstractDownloadReply::processEnd()
{
}

QByteArray AbstractDownloadReply::readAll()
{
    return m_reply->readAll();
}

qint64 AbstractDownloadReply::bytesAvailable() const
{
    return m_reply->bytesAvailable();
}

void AbstractDownloadReply::fetchUrl(const QUrl &url, int maxRetryCount,
        int maxRedirectCount, bool fetchData)
{
    m_maxRetryCount = maxRetryCount;
    m_maxRedirectCount = maxRedirectCount;
    m_fetchData = fetchData;
    m_request = QNetworkRequest(url);
    m_request.setRawHeader("User-Agent", appName());
    m_reply = createReply(m_request);
}

void AbstractDownloadReply::setError(AbstractDownloadReply::ErrorType error,
        const QString &text)
{
    m_error = error;
    m_errorString = text;
}

QNetworkReply *AbstractDownloadReply::createReply(
        const QNetworkRequest &request)
{
    QNetworkReply *reply;
    if (m_fetchData)
        reply = m_network->get(request);
    else
        reply = m_network->head(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SIGNAL(downloadProgress(qint64,qint64)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(reply_readyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(reply_finished()));

    return reply;
}

QUrl AbstractDownloadReply::redirectUrl() const
{
    return QUrl(m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());
}

bool AbstractDownloadReply::isRedirectFound() const
{
    QUrl url = redirectUrl();
    if (!url.isEmpty() && url != m_request.url())
        return true;

    return false;
}

bool AbstractDownloadReply::isRetryNeeded() const
{
    int code = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    // 503 - Service Unavailable (for example: images on http://lib.ru/)
    if (code == 503)
        return true;

    return false;
}

bool AbstractDownloadReply::isNotFound() const
{
    int code = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (code >= 200 && code <= 299)
        return false;

    return true;
}
