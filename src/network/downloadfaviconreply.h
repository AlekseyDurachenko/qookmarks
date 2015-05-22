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
#ifndef DOWNLOADFAVICONREPLY_H
#define DOWNLOADFAVICONREPLY_H

#include <QIcon>
#include "abstractdownloadreply.h"
#include "downloadfaviconrequest.h"


class DownloadFaviconReply : public AbstractDownloadReply
{
    Q_OBJECT
public:
    DownloadFaviconReply(const DownloadFaviconRequest &request,
                         QNetworkAccessManager *network,
                         QObject *parent = 0);

    inline const DownloadFaviconRequest &request() const;
    inline const QIcon &favicon() const;
protected:
    virtual void processEnd();
private:
    DownloadFaviconRequest m_request;
    QIcon m_favicon;
};

const QIcon &DownloadFaviconReply::favicon() const
{
    return m_favicon;
}

const DownloadFaviconRequest &DownloadFaviconReply::request() const
{
    return m_request;
}


#endif // DOWNLOADFAVICONREPLY_H
