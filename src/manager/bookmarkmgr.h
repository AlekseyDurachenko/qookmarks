// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef BOOKMARKMGR_H
#define BOOKMARKMGR_H

#include "bookmark.h"
#include <QObject>
class BookmarkItem;
class Manager;
class TagMgr;


class BookmarkMgr : public QObject
{
    Q_OBJECT

    friend class BookmarkItem;
    friend class TagMgr;
    friend class Manager;
private:
    explicit BookmarkMgr(Manager *mgr = 0);
    virtual ~BookmarkMgr();
public:
    inline Manager *mgr() const;

    inline int count() const;
    int indexOf(BookmarkItem *item) const;
    int indexOf(const QUrl &url) const;
    inline const QList<BookmarkItem *> &bookmarks() const;
    inline BookmarkItem *at(int index) const;
    BookmarkItem *find(const QUrl &url) const;
    BookmarkItem *add(const Bookmark &data);
    BookmarkItem *replace(const Bookmark &data);
    void removeAt(int index);
    void removeAll();

    inline int favoriteCount() const;
    inline int readItLaterCount() const;
    inline int ratedCount() const;
    inline int untaggedCount() const;
    inline int trashCount() const;
    inline int trashFavoriteCount() const;
    inline int trashReadItLaterCount() const;
    inline int trashRatedCount() const;
    inline int trashUntaggedCount() const;
    inline int notTrashCount() const;
    inline int notTrashFavoriteCount() const;
    inline int notTrashReadItLaterCount() const;
    inline int notTrashRatedCount() const;
    inline int notTrashUntaggedCount() const;
signals:
    void aboutToBeInserted(int first, int last);
    void inserted(int first, int last);
    void aboutToBeRemoved(int first, int last);
    void removed(int first, int last);
    void aboutToBeDataChanged(BookmarkItem *item);
    void dataChanged(BookmarkItem *item);
    void aboutToBeTagsChanged(BookmarkItem *item);
    void tagsChanged(BookmarkItem *item);
private:
    void callbackAboutToBeDataChanged(BookmarkItem *item);
    void callbackDataChanged(BookmarkItem *item);
    void callbackAboutToBeTagsChanged(BookmarkItem *item);
    void callbackTagsChanged(BookmarkItem *item);
private:
    void counterAboutToBeTagsChanged(BookmarkItem *item);
    void counterTagsChanged(BookmarkItem *item);
    void counterAboutToBeDataChanged(BookmarkItem *item);
    void counterDataChanged(BookmarkItem *item);
    void counterItemRemove(BookmarkItem *item);
    void counterItemInsert(BookmarkItem *item);
    void counterReset();
private:
    QList<BookmarkItem *> m_bookmarkItems;
    Manager *m_mgr;
    int m_counterFavorite;
    int m_counterReadItLater;
    int m_counterRated;
    int m_counterUntagged;
    int m_counterTrash;
    int m_counterTrashFavorite;
    int m_counterTrashReadItLater;
    int m_counterTrashRated;
    int m_counterTrashUntagged;
};

Manager *BookmarkMgr::mgr() const
{
    return m_mgr;
}

int BookmarkMgr::count() const
{
    return m_bookmarkItems.count();
}

const QList<BookmarkItem *> &BookmarkMgr::bookmarks() const
{
    return m_bookmarkItems;
}

BookmarkItem *BookmarkMgr::at(int index) const
{
    return m_bookmarkItems.at(index);
}

int BookmarkMgr::favoriteCount() const
{
    return m_counterFavorite;
}

int BookmarkMgr::readItLaterCount() const
{
    return m_counterReadItLater;
}

int BookmarkMgr::ratedCount() const
{
    return m_counterRated;
}

int BookmarkMgr::untaggedCount() const
{
    return m_counterUntagged;
}

int BookmarkMgr::trashCount() const
{
    return m_counterTrash;
}

int BookmarkMgr::trashFavoriteCount() const
{
    return m_counterTrashFavorite;
}

int BookmarkMgr::trashReadItLaterCount() const
{
    return m_counterTrashReadItLater;
}

int BookmarkMgr::trashRatedCount() const
{
    return m_counterTrashRated;
}

int BookmarkMgr::trashUntaggedCount() const
{
    return m_counterTrashUntagged;
}

int BookmarkMgr::notTrashCount() const
{
    return m_bookmarkItems.count() - m_counterTrash;
}

int BookmarkMgr::notTrashFavoriteCount() const
{
    return m_counterFavorite - m_counterTrashFavorite;
}

int BookmarkMgr::notTrashReadItLaterCount() const
{
    return m_counterReadItLater - m_counterTrashReadItLater;
}

int BookmarkMgr::notTrashRatedCount() const
{
    return m_counterRated - m_counterTrashRated;
}

int BookmarkMgr::notTrashUntaggedCount() const
{
    return m_counterUntagged - m_counterTrashUntagged;
}

#endif // BOOKMARKMGR_H
