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
#ifndef CDOWNLOADFAVICONREPLY_H
#define CDOWNLOADFAVICONREPLY_H

#include <QIcon>
#include "cabstractdownloadreply.h"
#include "cdownloadfaviconrequest.h"


class CDownloadFavIconReply : public CAbstractDownloadReply
{
    Q_OBJECT
public:
    CDownloadFavIconReply(const CDownloadFavIconRequest &request,
                          QNetworkAccessManager *network,
                          QObject *parent = 0);

    inline const CDownloadFavIconRequest &request() const;
    inline const QIcon &favIcon() const;
protected:
    virtual void processEnd();
private:
    CDownloadFavIconRequest m_request;
    QIcon m_favIcon;
};

const QIcon &CDownloadFavIconReply::favIcon() const
{
    return m_favIcon;
}

const CDownloadFavIconRequest &CDownloadFavIconReply::request() const
{
    return m_request;
}


#endif // CDOWNLOADFAVICONREPLY_H
