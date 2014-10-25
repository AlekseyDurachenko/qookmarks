// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include <QDebug>


CBookmarkMgr::CBookmarkMgr(QObject *parent) :
    QObject(parent)
{
    tagHierarchyCreate();
}

CBookmarkMgr::~CBookmarkMgr()
{
    qDeleteAll(m_bookmarkList);
    delete m_tagRootItem;
}

CTagItem *CBookmarkMgr::tagRootItem() const
{
    return m_tagRootItem;
}

CTagItem *CBookmarkMgr::tagFavoritesItem() const
{
    return m_tagFavoritesItem;
}

CTagItem *CBookmarkMgr::tagRatedItem() const
{
    return m_tagRatedItem;
}

CTagItem *CBookmarkMgr::tagReadLaterItem() const
{
    return m_tagReadLaterItem;
}

CTagItem *CBookmarkMgr::tagBookmarksItem() const
{
    return m_tagBookmarksItem;
}

CTagItem *CBookmarkMgr::tagTrashItem() const
{
    return m_tagTrashItem;
}

bool CBookmarkMgr::tagCanMove(CTagItem *newParentItem, CTagItem *item)
{
    // prevent incorrect usage of the function
    if (newParentItem == 0|| item == 0)
        return false;

    // it is the same
    if (newParentItem == item)
        return false;

    // we can move only the CTagItem::Tag
    // to CTagItem::Tag or CTagItem::Bookmarks
    if ((newParentItem->type() != CTagItem::Bookmarks
            && newParentItem->type() != CTagItem::Tag)
            || item->type() != CTagItem::Tag)
        return false;

    // item cannot be parent of newParentItem
    CTagItem *nextLevel = newParentItem;
    while (nextLevel)
        if (nextLevel->parent() == item)
            return false;
        else
            nextLevel = nextLevel->parent();

    if (newParentItem->findChild(item->data().name()))
        return false;

    return true;
}

CTagItem *CBookmarkMgr::tagFind(CTagItem *parentItem, const QString &name)
{
    // prevent incorrect usage of the function
    if (parentItem == 0)
        return 0;

    return parentItem->findChild(name);
}

CTagItem *CBookmarkMgr::tagFind(const QStringList &path)
{
    QStringList itemPath = path;
    CTagItem *item = tagBookmarksItem();
    while(item && itemPath.count())
        item = item->findChild(itemPath.takeFirst());

    if (itemPath.count())
        return 0;

    return item;
}

CTagItem *CBookmarkMgr::tagAdd(CTagItem *parentItem, const CTagItemData &data)
{
    // prevent incorrect usage of the function
    if (parentItem == 0)
        return 0;

    // we can add only to the CTagItem::Tag or CTagItem::Bookmarks
    if (parentItem->type() != CTagItem::Bookmarks
            && parentItem->type() != CTagItem::Tag)
        return 0;

    if (parentItem->findChild(data.name()))
        return 0;

    CTagItem *item = new CTagItem(data, this, parentItem);
    parentItem->addChild(item);
    return item;
}

bool CBookmarkMgr::tagMove(CTagItem *newParent, CTagItem *item)
{
    if (!tagCanMove(newParent, item))
        return 0;

    CTagItem *oldParent = item->parent();
    newParent->addChild(oldParent->takeChild(oldParent->childIndexOf(item)));

    return true;
}

void CBookmarkMgr::tagRemove(CTagItem *item)
{
    // prevent incorrect usage of the function
    if (item == 0)
        return;

    // we can remove only the CTagItem::Tag
    if (item->type() != CTagItem::Tag)
        return;

    item->parent()->takeChild(item->parent()->childIndexOf(item));

    for (int row = 0; row < item->childCount(); ++row)
        tagRemove(item->childAt(row));

    foreach (CBookmarkItem *bookmark, m_bookmarkList)
    {
        if (bookmark->tags().contains(item))
        {
            bookmark->tagRemove(item);
            int row = m_bookmarkList.indexOf(bookmark);
            emit bookmarkDataChanged(row, row);
        }
    }

    delete item;
}

void CBookmarkMgr::tagRemoveAll()
{
    foreach (CBookmarkItem *bookmark, m_bookmarkList)
        bookmark->tagRemoveAll();

    while (m_tagBookmarksItem->childCount())
        delete m_tagBookmarksItem->takeChild(0);

    emit bookmarkDataChanged(0, m_bookmarkList.count()-1);
}

int CBookmarkMgr::bookmarkCount() const
{
    return m_bookmarkList.count();
}

CBookmarkItem *CBookmarkMgr::bookmarkAt(int index) const
{
    return m_bookmarkList.at(index);
}

CBookmarkItem *CBookmarkMgr::bookmarkFind(const QUrl &url)
{
    return m_bookmarkUrlHash.value(url, 0);
}

CBookmarkItem *CBookmarkMgr::bookmarkAdd(const CBookmarkItemData &data)
{
    if (m_bookmarkUrlHash.contains(data.url()))
        return 0;

    int index = m_bookmarkList.count();
    CBookmarkItem *bookmark = new CBookmarkItem(data, this);
    m_bookmarkList.push_back(bookmark);
    m_bookmarkUrlHash[data.url()] = bookmark;
    emit bookmarkInserted(bookmark);
    emit bookmarkInserted(index, index);

    return bookmark;
}

void CBookmarkMgr::bookmarkRemove(CBookmarkItem *bookmark)
{
    // prevent incorrect usage of the function
    if (bookmark == 0)
        return;

    int index = m_bookmarkList.indexOf(bookmark);
    m_bookmarkUrlHash.remove(bookmark->data().url());
    m_bookmarkList.removeAt(index);
    emit bookmarkRemoved(bookmark);
    emit bookmarkRemoved(index, index);
    delete bookmark;
}

void CBookmarkMgr::bookmarkRemoveAt(int index)
{
    CBookmarkItem *bookmark = m_bookmarkList.takeAt(index);
    m_bookmarkUrlHash.remove(bookmark->data().url());
    emit bookmarkRemoved(bookmark);
    emit bookmarkRemoved(index, index);
    delete bookmark;
}

void CBookmarkMgr::bookmarkRemoveAll()
{
    if (m_bookmarkList.count() == 0)
        return;

    foreach (CBookmarkItem *bookmark, m_bookmarkList)
        emit bookmarkRemoved(bookmark);

    int count = m_bookmarkList.count();
    qDeleteAll(m_bookmarkList);
    m_bookmarkList.clear();
    m_bookmarkUrlHash.clear();
    emit bookmarkRemoved(0, count-1);
}

void CBookmarkMgr::callbackBookmarkDataChanged(CBookmarkItem *bookmark)
{
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::callbackTagDataChanged(CTagItem *tag)
{
    emit tagDataChanged(tag);
    emit tagDataChanged(tag->parent(), tag->row(), tag->row());
}

void CBookmarkMgr::callbackTagInserted(CTagItem *tag)
{
    emit tagInserted(tag);
}

void CBookmarkMgr::callbackTagRemoved(CTagItem *tag)
{
    emit tagRemoved(tag);
}

void CBookmarkMgr::callbackTagInserted(CTagItem *parent, int first, int last)
{
    emit tagInserted(parent, first, last);
}

void CBookmarkMgr::callbackTagRemoved(CTagItem *parent, int first, int last)
{
    emit tagRemoved(parent, first, last);
}

void CBookmarkMgr::tagHierarchyCreate()
{
    m_tagRootItem = new CTagItem(CTagItem::None, this);
    m_tagFavoritesItem = new CTagItem(CTagItem::Favorites, this);
    m_tagRatedItem = new CTagItem(CTagItem::Rated, this);
    m_tagReadLaterItem = new CTagItem(CTagItem::ReadLater, this);
    m_tagBookmarksItem = new CTagItem(CTagItem::Bookmarks, this);
    m_tagTrashItem = new CTagItem(CTagItem::Trash, this);
    m_tagRootItem->addChild(m_tagFavoritesItem);
    m_tagRootItem->addChild(m_tagRatedItem);
    m_tagRootItem->addChild(m_tagReadLaterItem);
    m_tagRootItem->addChild(m_tagBookmarksItem);
    m_tagRootItem->addChild(m_tagTrashItem);
}
