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
#include "networkmgr.h"
#include "downloadwebpageinforeply.h"
#include "downloadfaviconreply.h"
#include "checkurlreply.h"
#include <QNetworkAccessManager>


NetworkMgr::NetworkMgr(QObject *parent) : QObject(parent)
{
    m_network = 0;
}

NetworkMgr::~NetworkMgr()
{
}

void NetworkMgr::setNetwork(QNetworkAccessManager *network)
{
    m_network = network;
}

DownloadWebPageInfoReply *NetworkMgr::webPageInfo(const DownloadWebPageInfoRequest &request)
{
    return new DownloadWebPageInfoReply(request, m_network, this);
}

DownloadFaviconReply *NetworkMgr::favicon(
        const DownloadFaviconRequest &request)
{
    return new DownloadFaviconReply(request, m_network, this);
}

CheckUrlReply *NetworkMgr::checkUrl(const CheckUrlRequest &request)
{
    return new CheckUrlReply(request, m_network, this);
}
