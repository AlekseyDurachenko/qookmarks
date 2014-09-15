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
    m_tmpTagCache.clear();
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
    for (int i = 0; i < parentItem->childCount(); ++i)
        if (parentItem->childAt(i)->data().name() == name)
            return parentItem->childAt(i);
    return 0;
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
        if (bookmark->tags().contains(item))
        {
            bookmark->removeTag(item);
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
    bookmark->insertTag(tag);
    CStorage::bookmarkTagAdd(bookmark->id(), tag->id());
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::bookmarkRemoveTag(CBookmarkItem *bookmark, CTagItem *tag)
{
    bookmark->removeTag(tag);
    CStorage::bookmarkTagRemove(bookmark->id(), tag->id());
    int row = m_bookmarkList.indexOf(bookmark);
    emit bookmarkDataChanged(row, row);
}

void CBookmarkMgr::bookmarkClearTag(CBookmarkItem *bookmark)
{
    bookmark->clearTags();
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
    m_tagOtherItem = createTopLevelTag(CTagItem::Other);
    m_tagUntaggedItem = createTopLevelTag(CTagItem::Untagged);
    m_tagReadLaterItem = createTopLevelTag(CTagItem::ReadLater);
    m_tagFavoritesItem = createTopLevelTag(CTagItem::Favorites);
    recursiveTagRead(m_tagOtherItem);
    m_tagRootItem->addChild(m_tagOtherItem);
    m_tagRootItem->addChild(m_tagUntaggedItem);
    m_tagRootItem->addChild(m_tagReadLaterItem);
    m_tagRootItem->addChild(m_tagFavoritesItem);
    m_tmpTagCache[m_tagOtherItem->id()] = m_tagOtherItem;
    m_tmpTagCache[m_tagUntaggedItem->id()] = m_tagUntaggedItem;
    m_tmpTagCache[m_tagReadLaterItem->id()] = m_tagReadLaterItem;
    m_tmpTagCache[m_tagFavoritesItem->id()] = m_tagFavoritesItem;
}

CTagItem *CBookmarkMgr::createTopLevelTag(CTagItem::Type type)
{
    QSqlQuery query = CStorage::createQuery();
    query.prepare("SELECT id, name FROM TTag "
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
    query.prepare("SELECT id, type, name FROM TTag WHERE parentId = :id");
    query.bindValue(":id", parentItem->id());
    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value(0).toInt();
            CTagItem::Type type =
                    static_cast<CTagItem::Type>(query.value(1).toInt());

            CTagItemData data;
            data.setName(query.value(2).toString());

            CTagItem *item = new CTagItem(id, type, data, this, parentItem);
            m_tmpTagCache[item->id()] = item;
            recursiveTagRead(item);
            parentItem->addChild(item);
        }
    }
}

void CBookmarkMgr::bookmarkInit()
{
    QSqlQuery query = CStorage::createQuery();
    query.prepare("SELECT id, title, url, httpcode FROM TBookmark");
    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value(0).toInt();

            CBookmarkItemData data;
            data.setTitle(query.value(1).toString());
            data.setUrl(query.value(2).toUrl());
            data.setHttpCode(query.value(3).toInt());

            CBookmarkItem *item = new CBookmarkItem(id, data, this);
            foreach (CTagItem *tag, bookmarkTagRead(item))
                item->insertTag(tag);
            m_bookmarkList.push_back(item);
        }
    }
}

QSet<CTagItem *> CBookmarkMgr::bookmarkTagRead(CBookmarkItem *bookmark)
{
    QSet<CTagItem *> set;

    QSqlQuery query = CStorage::createQuery();
    query.prepare("SELECT TTagId FROM TBookmarkTag WHERE TBookmarkId = :id");
    query.bindValue(":id", bookmark->id());
    if (query.exec())
        while (query.next())
            set.insert(m_tmpTagCache[query.value(0).toInt()]);

    return set;
}
