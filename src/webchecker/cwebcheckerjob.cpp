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
#include "cwebcheckerjob.h"
#include <QDebug>


CWebCheckerJob::CWebCheckerJob(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_reply = 0;
    m_url = QUrl();
    m_httpErrorCode = 0;
    m_httpErrorString = QString();
}

bool CWebCheckerJob::isFinished() const
{
    if (m_reply)
        return m_reply->isFinished();

    return true;
}

bool CWebCheckerJob::isRunning() const
{
    if (m_reply)
        return m_reply->isRunning();

    return false;
}

QUrl CWebCheckerJob::url() const
{
    return m_url;
}

int CWebCheckerJob::httpError() const
{
    return m_httpErrorCode;
}

QString CWebCheckerJob::httpErrorString() const
{
    return m_httpErrorString;
}

void CWebCheckerJob::start(const QUrl &url)
{
    if (m_reply)
        m_reply->abort();

    m_url = url;
    m_httpErrorCode = 0;
    m_httpErrorString = QString();
    m_reply = m_network->head(QNetworkRequest(url));
    connect(m_reply, SIGNAL(finished()), this, SLOT(slot_reply_finished()));
}

void CWebCheckerJob::abort()
{
    if (m_reply)
        m_reply->abort();
}

void CWebCheckerJob::slot_reply_finished()
{
    if (m_reply->error() != QNetworkReply::OperationCanceledError)
    {
        m_httpErrorCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        m_httpErrorString = m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    }
    m_reply->deleteLater();
    m_reply = 0;
    emit finished();
}
