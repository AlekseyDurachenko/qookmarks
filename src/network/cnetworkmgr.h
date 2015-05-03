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
#ifndef CNETWORKMGR_H
#define CNETWORKMGR_H

#include <QObject>
#include "cdownloadwebpageinforeply.h"
#include "cdownloadfaviconrequest.h"
#include "ccheckurlrequest.h"
class QNetworkAccessManager;
class CDownloadWebPageInfoReply;
class CDownloadFaviconReply;
class CCheckUrlReply;


class CNetworkMgr : public QObject
{
    Q_OBJECT
public:
    explicit CNetworkMgr(QObject *parent = 0);
    virtual ~CNetworkMgr();

    inline QNetworkAccessManager *network() const;
    void setNetwork(QNetworkAccessManager *network);

    CDownloadWebPageInfoReply *webPageInfo(const CDownloadWebPageInfoRequest &request);
    CDownloadFaviconReply *favicon(const CDownloadFaviconRequest &request);
    CCheckUrlReply *checkUrl(const CCheckUrlRequest &request);    
private:
    QNetworkAccessManager *m_network;
};

QNetworkAccessManager *CNetworkMgr::network() const
{
    return m_network;
}


#endif // CNETWORKMGR_H
