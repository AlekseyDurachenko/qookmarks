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
#include "cnetworkmgr.h"
#include <QNetworkAccessManager>
#include "cdownloadfaviconreply.h"
#include "ccheckurlreply.h"


CNetworkMgr::CNetworkMgr(QObject *parent) : QObject(parent)
{
    m_network = 0;
}

CNetworkMgr::~CNetworkMgr()
{
}

void CNetworkMgr::setNetwork(QNetworkAccessManager *network)
{
    m_network = network;
}

CDownloadFaviconReply *CNetworkMgr::favicon(
        const CDownloadFaviconRequest &request)
{
    return new CDownloadFaviconReply(request, m_network, this);
}

CCheckUrlReply *CNetworkMgr::checkUrl(const CCheckUrlRequest &request)
{
    return new CCheckUrlReply(request, m_network, this);
}
