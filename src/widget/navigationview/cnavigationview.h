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
#ifndef CNAVIGATIONVIEW_H
#define CNAVIGATIONVIEW_H

#include <QTreeView>
#include "inavigationactions.h"
class CManager;


class CNavigationView : public QTreeView, public INavigationActions
{
    Q_OBJECT
public:
    CNavigationView(QWidget *parent = 0);
    virtual ~CNavigationView();
protected:
    virtual void tagsCopyOrMove(const QList<QStringList> &tags,
                                const QStringList &parentTag);
    virtual void bookmarksAssignTag(const QList<QUrl> &bookmarks,
                                    const QStringList& tag);
    virtual void bookmarksMarkFavorite(const QList<QUrl> &bookmarks);
    virtual void bookmarksMarkReadLater(const QList<QUrl> &bookmarks);
    virtual void bookmarksMarkTrash(const QList<QUrl> &bookmarks);
    virtual void bookmarksClearTags(const QList<QUrl> &bookmarks);
};


#endif // CNAVIGATIONVIEW_H
