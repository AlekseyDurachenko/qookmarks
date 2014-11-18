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
#ifndef CWEBICONFETCHJOB_H
#define CWEBICONFETCHJOB_H

#include <QObject>
#include <QNetworkReply>
#include <QIcon>


class CWebIconFetchJob : public QObject
{
    Q_OBJECT
public:
    explicit CWebIconFetchJob(QNetworkAccessManager *network, QObject *parent = 0);
    bool isFinished() const;
    bool isRunning() const;
    QUrl url() const;
    inline const QIcon &icon() const;

    void start(const QUrl &url);
    void abort();
private slots:
    void slot_reply_finished();
signals:
    void finished();
private:
    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;
    QUrl m_url;
    QIcon m_icon;
};

const QIcon &CWebIconFetchJob::icon() const
{
    return m_icon;
}


#endif // CWEBICONFETCHJOB_H
