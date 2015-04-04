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
#ifndef CWEBPAGEDOWNLOADER_H
#define CWEBPAGEDOWNLOADER_H

#include <QObject>
#include <QList>
#include <QWebPage>
#include <QHash>
#include "cwebfiledownloader.h"


class CWebPageDownloader : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError = 0,            // success
        OperationCanceledError, // aborted by user before the finished()
        FileSystemError,        // can't create file or directory
        OtherError
    };

    explicit CWebPageDownloader(QNetworkAccessManager *network, QObject *parent = 0);
    virtual ~CWebPageDownloader();

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline const QString &fileName() const;
    inline const QString &resourcesPath() const;
    void setFileName(const QString& fileName);

    void start();
    void abort();

    inline bool isRunning() const;
    inline bool isFinished() const;

    inline Error error() const;
    QString errorString() const;
signals:
    void downloadProgress(qint64 total, qint64 count);
    void finished();
public slots:
    void resource_finished();
    void webpage_loadFinished(bool ok);
private:
    QString addUrlToDownloader(const QString &urlstr, const QString &ext = QString());
    void updateProgress();
private:
    QNetworkAccessManager *m_network;
    QWebPage *m_webPage;

    QList<CWebFileDownloader *> m_resources;
    QHash<QUrl, QString> m_existsResources; // for resource files deduplication
    int m_totalResources;   // total count of the resource files
    int m_fileSeqNumber;    // used for resource file names

    QUrl m_url;
    QString m_fileName;
    QString m_resourcePath;

    bool m_isRunning;
    bool m_isFinished;

    Error m_error;
    QString m_errorString;
};

const QUrl &CWebPageDownloader::url() const
{
    return m_url;
}

const QString &CWebPageDownloader::fileName() const
{
    return m_fileName;
}

const QString &CWebPageDownloader::resourcesPath() const
{
    return m_resourcePath;
}

bool CWebPageDownloader::isRunning() const
{
    return m_isRunning;
}

bool CWebPageDownloader::isFinished() const
{
    return m_isFinished;
}

CWebPageDownloader::Error CWebPageDownloader::error() const
{
    return m_error;
}


#endif // CWEBPAGEDOWNLOADER_H
