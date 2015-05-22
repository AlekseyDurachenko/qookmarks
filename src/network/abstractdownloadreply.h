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
#ifndef ABSTRACTDOWNLOADREPLY_H
#define ABSTRACTDOWNLOADREPLY_H

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
class QNetworkAccessManager;


class AbstractDownloadReply : public QObject
{
    Q_OBJECT
public:
    enum ErrorType
    {
        NoError                 = 0x0000,
        OperationCanceledError  = 0x0001,
        RedirectLoopError       = 0x0002,
        RetryAttemptError       = 0x0003,
        NotFoundError           = 0x0004,
        FileError               = 0x0005,
        NetworkError            = 0x0006
    };
public:
    explicit AbstractDownloadReply(QNetworkAccessManager *network,
                                   QObject *parent = 0);
    virtual ~AbstractDownloadReply();

    inline ErrorType error() const;
    inline const QString &errorString() const;
    inline int httpStatusCode() const;
    inline const QString &httpReasonPhrase() const;
    inline QNetworkReply::NetworkError networkError() const;
    inline const QByteArray &mimeType() const;
    inline const QByteArray &charset() const;
signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
public slots:
    void abort();
private slots:
    void reply_readyRead();
    void reply_finished();
protected:
    virtual void processStart();
    virtual void processData();
    virtual void processEnd();
    QByteArray readAll();
    qint64 bytesAvailable() const;
    void fetchUrl(const QUrl &url, int maxRetryCount = 9,
                  int maxRedirectCount = 9, bool fetchData = true);
    void setError(ErrorType error, const QString &text);
private:
    QNetworkReply *createReply(const QNetworkRequest &request);
    QUrl redirectUrl() const;
    bool isRedirectFound() const;
    bool isRetryNeeded() const;
    bool isNotFound() const;
private:
    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;
    QNetworkRequest m_request;
    int m_maxRetryCount;
    int m_maxRedirectCount;
    bool m_fetchData;
    bool m_firstProgressIteration;
    ErrorType m_error;
    QString m_errorString;
    int m_httpStatusCode;
    QString m_httpReasonPhrase;
    QNetworkReply::NetworkError m_networkError;
    QByteArray m_mimeType;
    QByteArray m_charset;
};

AbstractDownloadReply::ErrorType AbstractDownloadReply::error() const
{
    return m_error;
}

const QString &AbstractDownloadReply::errorString() const
{
    return m_errorString;
}

int AbstractDownloadReply::httpStatusCode() const
{
    return m_httpStatusCode;
}

const QString &AbstractDownloadReply::httpReasonPhrase() const
{
    return m_httpReasonPhrase;
}

QNetworkReply::NetworkError AbstractDownloadReply::networkError() const
{
    return m_networkError;
}

const QByteArray &AbstractDownloadReply::mimeType() const
{
    return m_mimeType;
}

const QByteArray &AbstractDownloadReply::charset() const
{
    return m_charset;
}


#endif // ABSTRACTDOWNLOADREPLY_H
