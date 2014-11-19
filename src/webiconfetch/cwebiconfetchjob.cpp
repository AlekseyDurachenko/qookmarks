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
#include "cwebiconfetchjob.h"
#include <QDebug>


CWebIconFetchJob::CWebIconFetchJob(QNetworkAccessManager *network,
        QObject *parent) : QObject(parent)
{
    m_network = network;
    m_reply = 0;
    m_url = QUrl();
}

bool CWebIconFetchJob::isFinished() const
{
    if (m_reply)
        return m_reply->isFinished();

    return true;
}

bool CWebIconFetchJob::isRunning() const
{
    if (m_reply)
        return m_reply->isRunning();

    return false;
}

QUrl CWebIconFetchJob::url() const
{
    return m_url;
}

void CWebIconFetchJob::start(const QUrl &url)
{
    if (m_reply)
        m_reply->abort();

    m_icon = QIcon();
    m_url = url;
    QUrl uuu = url;
    uuu.setPath("favicon.ico");
    qDebug() << uuu;
    m_reply = m_network->get(QNetworkRequest(uuu));
    connect(m_reply, SIGNAL(finished()), this, SLOT(slot_reply_finished()));
}

void CWebIconFetchJob::abort()
{
    if (m_reply)
        m_reply->abort();
}

void CWebIconFetchJob::slot_reply_finished()
{
    qDebug() << "finished: " << m_reply->errorString();
    if (m_reply->error() == QNetworkReply::NoError)
    {
        QPixmap pixmap;
        pixmap.loadFromData(m_reply->readAll());
        m_icon = QIcon(pixmap);
        qDebug() << pixmap.size();
        qDebug() << m_icon;
    }
    m_reply->deleteLater();
    m_reply = 0;
    emit finished();
}
