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
#ifndef CWEBFILEDOWNLOADER_H
#define CWEBFILEDOWNLOADER_H

#include <QObject>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>


class CWebFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit CWebFileDownloader(QNetworkAccessManager *network, QObject *parent = 0);
    virtual ~CWebFileDownloader();

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline const QString &fileName() const;
    void setFileName(const QString& fileName);

    void start();
    void abort();
    inline bool isStarted() const;
    inline bool isAborted() const;
    inline bool isFinished() const;
signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
private slots:
    void reply_readyRead();
    void reply_finished();
private:
    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;
    QFile *m_file;
    QUrl m_url;
    QString m_fileName;
    int m_retryCount;
    bool m_isAborted;
    bool m_isStarted;
    bool m_isFinished;
};

const QUrl &CWebFileDownloader::url() const
{
    return m_url;
}

const QString &CWebFileDownloader::fileName() const
{
    return m_fileName;
}

bool CWebFileDownloader::isStarted() const
{
    return m_isStarted;
}

bool CWebFileDownloader::isAborted() const
{
    return m_isAborted;
}

bool CWebFileDownloader::isFinished() const
{
    return m_isFinished;
}


#endif // CWEBFILEDOWNLOADER_H
