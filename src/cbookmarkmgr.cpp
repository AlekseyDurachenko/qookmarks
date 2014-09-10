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
#include "cstorage.h"
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

CTagItem *CBookmarkMgr::tagTagRootItem() const
{
    return m_tagTagRootItem;
}

CTagItem *CBookmarkMgr::tagReadLaterItem() const
{
    return m_tagReadLaterItem;
}

CTagItem *CBookmarkMgr::tagFavoritesItem() const
{
    return m_tagFavoritesItem;
}

CTagItem *CBookmarkMgr::tagAdd(CTagItem *parent, const CTagItemData &data)
{
    int id = CStorage::tagInsert(CTagItem::Tag, parent->id(), data);
    CTagItem *item = new CTagItem(id, CTagItem::Tag, data, this, parent);
    parent->addChild(item);
    return item;
}

void CBookmarkMgr::tagMove(CTagItem *newParent, CTagItem *item)
{
    CTagItem *oldParent = item->parent();
    newParent->addChild(oldParent->takeChild(oldParent->childIndexOf(item)));
    CStorage::tagMove(item->id(), newParent->id());
}

void CBookmarkMgr::tagRemove(CTagItem *item)
{
    item->parent()->takeChild(item->parent()->childIndexOf(item));

    for (int row = 0; row < item->childCount(); ++row)
        tagRemove(item->childAt(row));

    foreach (CBookmarkItem *bookmark, m_bookmarkList)
    {
        if (bookmark->pTags()->contains(item))
        {
            bookmark->pTags()->remove(item);
            int row = m_bookmarkList.indexOf(bookmark);
            emit bookmarkDataChanged(row, row);
        }
    }

    CStorage::tagDelete(item->id());
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

CBookmarkItem *CBookmarkMgr::bookmarkAdd(const CBookmarkItemData &data)
{
    int row = m_bookmarkList.count();
    int id = CStorage::bookmarkInsert(data);
    CBookmarkItem *bookmark = new CBookmarkItem(id, data, this);
    m_bookmarkList.push_back(bookmark);
    emit bookmarkInserted(row, row);
    return bookmark;
}

void CBookmarkMgr::bookmarkRemove(CBookmarkItem *bookmark)
{
    int row = m_bookmarkList.indexOf(bookmark);
    CStorage::bookmarkDelete(bookmark->id());
    delete m_bookmarkList.takeAt(row);
    emit bookmarkRemoved(row, row);
}

void CBookmarkMgr::bookmarkAddTag(CBookmarkItem *bookmark, CTagItem *tag)
{
    bookmark->pTags()->insert(tag);
    CStorage::bookmarkTagAdd(bookmark->id(), tag->id());
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::bookmarkRemoveTag(CBookmarkItem *bookmark, CTagItem *tag)
{
    bookmark->pTags()->remove(tag);
    CStorage::bookmarkTagRemove(bookmark->id(), tag->id());
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::bookmarkClearTag(CBookmarkItem *bookmark)
{
    bookmark->pTags()->clear();
    CStorage::bookmarkTagClear(bookmark->id());
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::callbackBookmarkDataChanged(CBookmarkItem *bookmark)
{
    int row = m_bookmarkList.indexOf(bookmark);
    CStorage::bookmarkUpdate(bookmark->id(), bookmark->data());
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::callbackTagDataChanged(CTagItem *tag)
{
    int parentId = (tag->parent()) ? (tag->parent()->id()) : (-1);
    CStorage::tagUpdate(tag->id(), parentId, tag->type(), tag->data());
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
    m_tagTagRootItem = createTopLevelTag(CTagItem::TagRoot);
    m_tagReadLaterItem = createTopLevelTag(CTagItem::ReadLater);
    m_tagFavoritesItem = createTopLevelTag(CTagItem::Favorites);
    recursiveTagRead(m_tagTagRootItem);
    recursiveTagRead(m_tagReadLaterItem);
    recursiveTagRead(m_tagFavoritesItem);
    m_tagRootItem->addChild(m_tagTagRootItem);
    m_tagRootItem->addChild(m_tagReadLaterItem);
    m_tagRootItem->addChild(m_tagFavoritesItem);
}

CTagItem *CBookmarkMgr::createTopLevelTag(CTagItem::Type type)
{
    QSqlQuery query = CStorage::createQuery();
    query.prepare("SELECT id, title FROM TTag "
            " WHERE parentId = -1 AND type = :type");
    query.bindValue(":type", type);

    int id = -1;
    if (query.exec() && query.next())
        id = query.value(0).toInt();
    else
        id = CStorage::tagInsert(type, -1, CTagItemData());

    return new CTagItem(id, type, CTagItemData(), this);
}

void CBookmarkMgr::recursiveTagRead(CTagItem *parentItem)
{
    QSqlQuery query = CStorage::createQuery();
    query.prepare("SELECT id, type, title FROM TTag WHERE parentId = :id");
    query.bindValue(":id", parentItem->id());
    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value(0).toInt();
            CTagItem::Type type =
                    static_cast<CTagItem::Type>(query.value(1).toInt());

            CTagItemData data;
            data.setTitle(query.value(2).toString());

            CTagItem *item = new CTagItem(id, type, data, this, parentItem);
            recursiveTagRead(item);
            parentItem->addChild(item);
        }
    }
}

void CBookmarkMgr::bookmarkInit()
{
    QSqlQuery query = CStorage::createQuery();
    query.prepare("SELECT id, title, url FROM TBookmark");
    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value(0).toInt();

            CBookmarkItemData data;
            data.setTitle(query.value(1).toString());
            data.setUrl(query.value(2).toUrl());

            m_bookmarkList.push_back(new CBookmarkItem(id, data, this));
        }
    }
}
