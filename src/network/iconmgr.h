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
#ifndef ICONMGR_H
#define ICONMGR_H

#include <QIcon>
#include <QObject>
#include <QUrl>
class Prj;


class IconMgr
{
public:
    IconMgr();

    void setIconsPath(const QString &iconPath);
    void loadIcon(const QString &host);
    void loadIcons(const QList<QUrl> &urls);
    void unloadIcons();

    bool hasIcon(const QUrl &url) const;
    QIcon icon(const QUrl &url, const QIcon &defaultIcon = QIcon()) const;
    void saveIcon(const QUrl &url, const QIcon &icon);
private:
    QString keyFromUrl(const QUrl &url) const;
    QString iconFileName(const QUrl &url) const;
    QString iconFileName(const QString &host) const;
    QString iconFullFileName(const QUrl &url) const;
    QString iconFullFileName(const QString &host) const;
private:
    QString m_iconPath;
    QHash<QString, QIcon> m_iconHash;
};


#endif // ICONMGR_H
