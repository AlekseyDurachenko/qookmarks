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
#include "ciconmgr.h"
#include <QDir>
#include <QSet>
#include "hash_functions.h"


CIconMgr::CIconMgr()
{
}

void CIconMgr::setIconsPath(const QString &iconPath)
{
    m_iconPath = iconPath;
}

void CIconMgr::loadIcon(const QString &host)
{
    if (!QFile::exists(iconFullFileName(host)))
        return;

    m_iconHash.insert(host, QIcon(iconFullFileName(host)));
}

void CIconMgr::loadIcons(const QList<QUrl> &urls)
{
    QSet<QString> hosts;
    foreach (const QUrl &url, urls)
        hosts.insert(keyFromUrl(url));

    foreach (const QString &host, hosts)
        loadIcon(host);
}

void CIconMgr::unloadIcons()
{
    m_iconHash.clear();
}

bool CIconMgr::hasIcon(const QUrl &url) const
{
    return m_iconHash.contains(keyFromUrl(url));
}

QIcon CIconMgr::icon(const QUrl &url, const QIcon &defaultIcon) const
{
    return m_iconHash.value(keyFromUrl(url), defaultIcon);
}

void CIconMgr::saveIcon(const QUrl &url, const QIcon &icon)
{
    if (iconFullFileName(url).isEmpty())
        return;

    iconFullFileName(url);
    icon.pixmap(QSize(32, 32)).save(iconFullFileName(url));

    m_iconHash[keyFromUrl(url)] = icon;

    // TODO: update bookmarks
}

QString CIconMgr::keyFromUrl(const QUrl &url) const
{
    return url.host();
}

QString CIconMgr::iconFullFileName(const QUrl &url) const
{
    return iconFullFileName(keyFromUrl(url));
}

QString CIconMgr::iconFullFileName(const QString &host) const
{
    if (host.isEmpty() || m_iconPath.isEmpty())
        return QString();

    return m_iconPath + QDir::separator() + iconFileName(host);
}

QString CIconMgr::iconFileName(const QUrl &url) const
{
    return iconFileName(keyFromUrl(url));
}

QString CIconMgr::iconFileName(const QString &host) const
{
    return sha1(host) + "_" + md5(host) + ".ico";
}
