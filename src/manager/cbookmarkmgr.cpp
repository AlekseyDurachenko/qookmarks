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
    tagInit();
    bookmarkInit();
}

CBookmarkMgr::~CBookmarkMgr()
{
    foreach (CBookmarkItem *item, m_bookmarkList)
        delete item;

    delete m_tagRootItem;
}

CTagItem *CBookmarkMgr::tagRootItem() const
{
    return m_tagRootItem;
}

CTagItem *CBookmarkMgr::tagOtherItem() const
{
    return m_tagOtherItem;
}

CTagItem *CBookmarkMgr::tagUntaggedItem() const
{
    return m_tagUntaggedItem;
}

CTagItem *CBookmarkMgr::tagReadLaterItem() const
{
    return m_tagReadLaterItem;
}

CTagItem *CBookmarkMgr::tagFavoritesItem() const
{
    return m_tagFavoritesItem;
}

CTagItem *CBookmarkMgr::tagFind(CTagItem *parentItem, const QString &name)
{
    return parentItem->findChild(name);
}

CTagItem *CBookmarkMgr::tagAdd(CTagItem *parent, const CTagItemData &data)
{
    if (parent->findChild(data.name()))
        return 0;

    CTagItem *item = new CTagItem(CTagItem::Tag, data, this, parent);
    parent->addChild(item);
    return item;
}

bool CBookmarkMgr::tagMove(CTagItem *newParent, CTagItem *item)
{
    if (newParent->findChild(item->data().name()))
        return false;

    CTagItem *oldParent = item->parent();
    newParent->addChild(oldParent->takeChild(oldParent->childIndexOf(item)));

    return true;
}

void CBookmarkMgr::tagRemove(CTagItem *item)
{
    item->parent()->takeChild(item->parent()->childIndexOf(item));

    for (int row = 0; row < item->childCount(); ++row)
        tagRemove(item->childAt(row));

    foreach (CBookmarkItem *bookmark, m_bookmarkList)
    {
        if (bookmark->tags().contains(item))
        {
            bookmark->removeTag(item);
            int row = m_bookmarkList.indexOf(bookmark);
            emit bookmarkDataChanged(row, row);
        }
    }

    delete item;
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
    foreach (CBookmarkItem *item, m_bookmarkList)
        if (item->data().url() == url)
            return item;
    return 0;
}

CBookmarkItem *CBookmarkMgr::bookmarkAdd(const CBookmarkItemData &data)
{
    int row = m_bookmarkList.count();
    CBookmarkItem *bookmark = new CBookmarkItem(data, this);
    m_bookmarkList.push_back(bookmark);
    emit bookmarkInserted(row, row);
    return bookmark;
}

void CBookmarkMgr::bookmarkRemove(CBookmarkItem *bookmark)
{
    int row = m_bookmarkList.indexOf(bookmark);
    delete m_bookmarkList.takeAt(row);
    emit bookmarkRemoved(row, row);
}

void CBookmarkMgr::bookmarkAddTag(CBookmarkItem *bookmark, CTagItem *tag)
{
    bookmark->insertTag(tag);
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::bookmarkRemoveTag(CBookmarkItem *bookmark, CTagItem *tag)
{
    bookmark->removeTag(tag);
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::bookmarkClearTag(CBookmarkItem *bookmark)
{
    bookmark->clearTags();
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::callbackBookmarkDataChanged(CBookmarkItem *bookmark)
{
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::callbackTagDataChanged(CTagItem *tag)
{
    emit tagDataChanged(tag->parent(), tag->row(), tag->row());
}

void CBookmarkMgr::callbackTagInserted(CTagItem *parent, int first, int last)
{
    emit tagInserted(parent, first, last);
}

void CBookmarkMgr::callbackTagRemoved(CTagItem *parent, int first, int last)
{
    emit tagRemoved(parent, first, last);
}

void CBookmarkMgr::tagInit()
{
    m_tagRootItem = new CTagItem(CTagItem::RootItem, this);
    m_tagOtherItem = new CTagItem(CTagItem::Other, this);
    m_tagUntaggedItem = new CTagItem(CTagItem::Untagged, this);
    m_tagReadLaterItem = new CTagItem(CTagItem::ReadLater, this);
    m_tagFavoritesItem = new CTagItem(CTagItem::Favorites, this);
    m_tagRootItem->addChild(m_tagOtherItem);
    m_tagRootItem->addChild(m_tagUntaggedItem);
    m_tagRootItem->addChild(m_tagReadLaterItem);
    m_tagRootItem->addChild(m_tagFavoritesItem);
}

void CBookmarkMgr::bookmarkInit()
{
}
