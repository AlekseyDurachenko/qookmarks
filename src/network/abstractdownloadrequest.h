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
#ifndef ABSTRACTDOWNLOADREQUEST_H
#define ABSTRACTDOWNLOADREQUEST_H

#include <QUrl>


class AbstractDownloadRequest
{
public:
    AbstractDownloadRequest();

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline int maxRetryCount() const;
    void setMaxRetryCount(int maxRetryCount);

    inline int maxRedirectCount() const;
    void setMaxRedirectCount(int maxRedirectCount);
private:
    QUrl m_url;
    int m_maxRetryCount;
    int m_maxRedirectCount;
};

const QUrl &AbstractDownloadRequest::url() const
{
    return m_url;
}

int AbstractDownloadRequest::maxRetryCount() const
{
    return m_maxRetryCount;
}

int AbstractDownloadRequest::maxRedirectCount() const
{
    return m_maxRedirectCount;
}


#endif // ABSTRACTDOWNLOADREQUEST_H
