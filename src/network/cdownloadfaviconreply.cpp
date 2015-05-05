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
#include "cdownloadfaviconreply.h"
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif


CDownloadFaviconReply::CDownloadFaviconReply(
        const CDownloadFaviconRequest &request,
        QNetworkAccessManager *network, QObject *parent) :
    CAbstractDownloadReply(network, parent)
{
    m_request = request;
    QUrl favIconUrl = request.url();
    favIconUrl.setPath("/favicon.ico");
    favIconUrl.setFragment(QString());
#if QT_VERSION >= 0x050000
    favIconUrl.setQuery(QUrlQuery());
#else
    favIconUrl.setQueryItems(QList<QPair<QString, QString> >());
#endif
    fetchUrl(favIconUrl,
             request.maxRetryCount(),
             request.maxRedirectCount(),
             true);
}

void CDownloadFaviconReply::processEnd()
{
    QPixmap pixmap;
    if (!pixmap.loadFromData(readAll()))
        m_favicon = QIcon();
    else
        m_favicon = pixmap;
}
