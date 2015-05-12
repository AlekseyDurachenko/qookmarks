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
#ifndef CBOOKMARKMGR_H
#define CBOOKMARKMGR_H

#include <QObject>
#include "cbookmark.h"
class CManager;
class CTagMgr;
class CBookmarkItem;

class CBookmarkMgr : public QObject
{
    Q_OBJECT

    friend class CBookmarkItem;
    friend class CTagMgr;
    friend class CManager;
private:
    explicit CBookmarkMgr(CManager *mgr = 0);
    virtual ~CBookmarkMgr();
public:
    inline CManager *mgr() const;

    inline int count() const;
    int indexOf(CBookmarkItem *item) const;
    int indexOf(const QUrl &url) const;
    inline const QList<CBookmarkItem *> &bookmarks() const;
    inline CBookmarkItem *at(int index) const;
    CBookmarkItem *find(const QUrl &url) const;
    CBookmarkItem *add(const CBookmark &data);
    CBookmarkItem *replace(const CBookmark &data);
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
    void aboutToBeDataChanged(CBookmarkItem *item);
    void dataChanged(CBookmarkItem *item);
    void aboutToBeTagsChanged(CBookmarkItem *item);
    void tagsChanged(CBookmarkItem *item);
private:
    void callbackAboutToBeDataChanged(CBookmarkItem *item);
    void callbackDataChanged(CBookmarkItem *item);
    void callbackAboutToBeTagsChanged(CBookmarkItem *item);
    void callbackTagsChanged(CBookmarkItem *item);
private:
    void counterAboutToBeTagsChanged(CBookmarkItem *item);
    void counterTagsChanged(CBookmarkItem *item);
    void counterAboutToBeDataChanged(CBookmarkItem *item);
    void counterDataChanged(CBookmarkItem *item);
    void counterItemRemove(CBookmarkItem *item);
    void counterItemInsert(CBookmarkItem *item);
    void counterReset();
private:
    QList<CBookmarkItem *> m_bookmarkItems;
    CManager *m_mgr;
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

CManager *CBookmarkMgr::mgr() const
{
    return m_mgr;
}

int CBookmarkMgr::count() const
{
    return m_bookmarkItems.count();
}

const QList<CBookmarkItem *> &CBookmarkMgr::bookmarks() const
{
    return m_bookmarkItems;
}

CBookmarkItem *CBookmarkMgr::at(int index) const
{
    return m_bookmarkItems.at(index);
}

int CBookmarkMgr::favoriteCount() const
{
    return m_counterFavorite;
}

int CBookmarkMgr::readItLaterCount() const
{
    return m_counterReadItLater;
}

int CBookmarkMgr::ratedCount() const
{
    return m_counterRated;
}

int CBookmarkMgr::untaggedCount() const
{
    return m_counterUntagged;
}

int CBookmarkMgr::trashCount() const
{
    return m_counterTrash;
}

int CBookmarkMgr::trashFavoriteCount() const
{
    return m_counterTrashFavorite;
}

int CBookmarkMgr::trashReadItLaterCount() const
{
    return m_counterTrashReadItLater;
}

int CBookmarkMgr::trashRatedCount() const
{
    return m_counterTrashRated;
}

int CBookmarkMgr::trashUntaggedCount() const
{
    return m_counterTrashUntagged;
}

int CBookmarkMgr::notTrashCount() const
{
    return m_bookmarkItems.count() - m_counterTrash;
}

int CBookmarkMgr::notTrashFavoriteCount() const
{
    return m_counterFavorite - m_counterTrashFavorite;
}

int CBookmarkMgr::notTrashReadItLaterCount() const
{
    return m_counterReadItLater - m_counterTrashReadItLater;
}

int CBookmarkMgr::notTrashRatedCount() const
{
    return m_counterRated - m_counterTrashRated;
}

int CBookmarkMgr::notTrashUntaggedCount() const
{
    return m_counterUntagged - m_counterTrashUntagged;
}

#endif // CBOOKMARKMGR_H
