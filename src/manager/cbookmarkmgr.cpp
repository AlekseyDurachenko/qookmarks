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
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "cmanager.h"
#include "consts.h"


CBookmarkMgr::CBookmarkMgr(CManager *mgr) : QObject(mgr)
{
    m_mgr = mgr;
    counterReset();
}

CBookmarkMgr::~CBookmarkMgr()
{
    removeAll();
}

int CBookmarkMgr::indexOf(CBookmarkItem *item) const
{
    return m_bookmarkItems.indexOf(item);
}

int CBookmarkMgr::indexOf(const QUrl &url) const
{
    for (int i = 0; i < m_bookmarkItems.count(); ++i)
        if (m_bookmarkItems.at(i)->data().url() == url)
            return i;
    return -1;
}

CBookmarkItem *CBookmarkMgr::find(const QUrl &url) const
{
    int index = indexOf(url);
    if (index == -1)
        return 0;
    return m_bookmarkItems.at(index);
}

CBookmarkItem *CBookmarkMgr::add(const CBookmark &data)
{
    if (find(data.url()))
        return 0;

    CBookmarkItem *item = new CBookmarkItem(data, this);
    int index = m_bookmarkItems.count();

    emit aboutToBeInserted(index, index);
    m_bookmarkItems.push_back(item);
    counterItemInsert(item);
    emit inserted(index, index);

    return item;
}

CBookmarkItem *CBookmarkMgr::replace(const CBookmark &data)
{
    CBookmarkItem *item = find(data.url());
    if (item)
    {
        item->setData(data);
    }
    else
    {
        item = new CBookmarkItem(data, this);
        int index = m_bookmarkItems.count();

        emit aboutToBeInserted(index, index);
        m_bookmarkItems.push_back(item);
        counterItemInsert(item);
        emit inserted(index, index);
    }
    return item;
}

void CBookmarkMgr::removeAt(int index)
{
    emit aboutToBeRemoved(index, index);
    counterItemRemove(m_bookmarkItems.at(index));
    delete m_bookmarkItems.takeAt(index);
    emit removed(index, index);
}

void CBookmarkMgr::removeAll()
{
    if (m_bookmarkItems.isEmpty())
        return;

    int last = m_bookmarkItems.count() - 1;

    emit aboutToBeRemoved(0, last);
    counterReset();
    while (m_bookmarkItems.count())
        delete m_bookmarkItems.takeLast();
    emit removed(0, last);
}

void CBookmarkMgr::callbackAboutToBeDataChanged(CBookmarkItem *item)
{
    counterAboutToBeDataChanged(item);
    emit aboutToBeDataChanged(item);
}

void CBookmarkMgr::callbackDataChanged(CBookmarkItem *item)
{
    counterDataChanged(item);
    emit dataChanged(item);
}

void CBookmarkMgr::callbackAboutToBeTagsChanged(CBookmarkItem *item)
{
    counterAboutToBeTagsChanged(item);
    emit aboutToBeTagsChanged(item);
}

void CBookmarkMgr::callbackTagsChanged(CBookmarkItem *item)
{
    counterTagsChanged(item);
    emit tagsChanged(item);
}

void CBookmarkMgr::counterAboutToBeTagsChanged(CBookmarkItem *item)
{
    if (item->tags().isEmpty())
        m_counterUntagged -= 1;

    if (item->data().isTrash() && item->tags().isEmpty())
        m_counterTrashUntagged -= 1;
}

void CBookmarkMgr::counterTagsChanged(CBookmarkItem *item)
{
    if (item->tags().isEmpty())
        m_counterUntagged += 1;

    if (item->data().isTrash() && item->tags().isEmpty())
        m_counterTrashUntagged += 1;
}

void CBookmarkMgr::counterAboutToBeDataChanged(CBookmarkItem *item)
{
    if (item->data().isFavorite())
        m_counterFavorite -= 1;

    if (item->data().isReadItLater())
        m_counterReadItLater -= 1;

    if (item->data().rating() > Bookmark::MinRating)
        m_counterRated -= 1;

    if (item->data().isTrash())
    {
        if (item->data().isFavorite())
            m_counterTrashFavorite -= 1;

        if (item->data().isReadItLater())
            m_counterTrashReadItLater -= 1;

        if (item->data().rating() > Bookmark::MinRating)
            m_counterTrashRated -= 1;

        m_counterTrash -= 1;
    }
}

void CBookmarkMgr::counterDataChanged(CBookmarkItem *item)
{
    if (item->data().isFavorite())
        m_counterFavorite += 1;

    if (item->data().isReadItLater())
        m_counterReadItLater += 1;

    if (item->data().rating() > Bookmark::MinRating)
        m_counterRated += 1;

    if (item->data().isTrash())
    {
        if (item->data().isFavorite())
            m_counterTrashFavorite += 1;

        if (item->data().isReadItLater())
            m_counterTrashReadItLater += 1;

        if (item->data().rating() > Bookmark::MinRating)
            m_counterTrashRated += 1;

        m_counterTrash += 1;
    }
}

void CBookmarkMgr::counterItemRemove(CBookmarkItem *item)
{
    counterAboutToBeTagsChanged(item);
    counterAboutToBeDataChanged(item);
}

void CBookmarkMgr::counterItemInsert(CBookmarkItem *item)
{
    counterTagsChanged(item);
    counterDataChanged(item);
}

void CBookmarkMgr::counterReset()
{
    m_counterFavorite       = 0;
    m_counterReadItLater    = 0;
    m_counterRated          = 0;
    m_counterUntagged       = 0;
    m_counterTrash          = 0;
    m_counterTrashFavorite  = 0;
    m_counterTrashReadItLater   = 0;
    m_counterTrashRated     = 0;
    m_counterTrashUntagged  = 0;
}
