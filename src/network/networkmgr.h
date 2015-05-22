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
#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include <QObject>
#include "downloadwebpageinforeply.h"
#include "downloadfaviconrequest.h"
#include "checkurlrequest.h"
class QNetworkAccessManager;
class DownloadWebPageInfoReply;
class DownloadFaviconReply;
class CheckUrlReply;


class NetworkMgr : public QObject
{
    Q_OBJECT
public:
    explicit NetworkMgr(QObject *parent = 0);
    virtual ~NetworkMgr();

    inline QNetworkAccessManager *network() const;
    void setNetwork(QNetworkAccessManager *network);

    DownloadWebPageInfoReply *webPageInfo(const DownloadWebPageInfoRequest &request);
    DownloadFaviconReply *favicon(const DownloadFaviconRequest &request);
    CheckUrlReply *checkUrl(const CheckUrlRequest &request);
private:
    QNetworkAccessManager *m_network;
};

QNetworkAccessManager *NetworkMgr::network() const
{
    return m_network;
}


#endif // NETWORKMGR_H
