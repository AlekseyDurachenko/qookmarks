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
#ifndef CWEBDOWNLOADJOB_H
#define CWEBDOWNLOADJOB_H

#include <QObject>
#include <QNetworkReply>
#include <QWebPage>

class CWebDownloadJob : public QObject
{
    Q_OBJECT
public:
    explicit CWebDownloadJob(QNetworkAccessManager *network, QObject *parent = 0);
    bool isFinished() const;
    bool isRunning() const;
    QUrl url() const;
    int httpError() const;
    QString httpErrorString() const;

    void start(const QUrl &url);
    void abort();
private slots:
    void slot_reply_finished();
    void slot_load_finished();
signals:
    void finished();
private:
    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;
    QUrl m_url;
    int m_httpErrorCode;
    QString m_httpErrorString;
    QWebPage *webPage ;
};


#endif // CWEBDOWNLOADJOB_H
