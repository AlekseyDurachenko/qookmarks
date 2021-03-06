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
#ifndef WEBPAGESCREENSHOT_H
#define WEBPAGESCREENSHOT_H

#include "webfiledownloader.h"
#include <QHash>
#include <QList>
#include <QObject>
#include <QWebPage>


class WebPageScreenshot : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError = 0,            // success
        OperationCanceledError, // aborted by user before the finished()
        FileSystemError,        // can't create file or directory
        OtherError
    };

    explicit WebPageScreenshot(QNetworkAccessManager *network, QObject *parent = 0);
    virtual ~WebPageScreenshot();

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline const QSize &screenshotSize() const;
    void setScreenshotSize(const QSize &size);

    inline const QImage &screenshot() const;
    void start();
    void abort();

    inline bool isRunning() const;
    inline bool isFinished() const;

    inline Error error() const;
    QString errorString() const;
signals:
    void finished();
public slots:
    void webpage_loadFinished(bool ok);
private:
    QNetworkAccessManager *m_network;
    QWebPage *m_webPage;
    QImage m_screenshot;
    QSize m_screenshotSize;

    QUrl m_url;

    bool m_isRunning;
    bool m_isFinished;

    Error m_error;
    QString m_errorString;
};

const QUrl &WebPageScreenshot::url() const
{
    return m_url;
}

const QSize &WebPageScreenshot::screenshotSize() const
{
    return m_screenshotSize;
}

const QImage &WebPageScreenshot::screenshot() const
{
    return m_screenshot;
}

bool WebPageScreenshot::isRunning() const
{
    return m_isRunning;
}

bool WebPageScreenshot::isFinished() const
{
    return m_isFinished;
}

WebPageScreenshot::Error WebPageScreenshot::error() const
{
    return m_error;
}


#endif // WEBPAGEDOWNLOADER_H
