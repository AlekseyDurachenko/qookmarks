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
#ifndef INAVIGATIONACTIONS_H
#define INAVIGATIONACTIONS_H

#include <QUrl>
#include <QList>
#include <QStringList>


class INavigationActions
{
public:
    virtual void navActMoveTags(const QList<QStringList> &tags, const QStringList &parentTag) = 0;
    virtual void navActAssignTag(const QList<QUrl> &bookmarks, const QStringList& tag) = 0;
    virtual void navActClearTags(const QList<QUrl> &bookmarks) = 0;
    virtual void navActFavorite(const QList<QUrl> &bookmarks) = 0;
    virtual void navActReadItLater(const QList<QUrl> &bookmarks) = 0;
    virtual void navActTrash(const QList<QUrl> &bookmarks) = 0;
};


#endif // INAVIGATIONACTIONS_H
