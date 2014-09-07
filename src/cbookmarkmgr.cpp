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

void CBookmarkMgr::tagInit()
{
    m_tagRootItem = new CTagItem(CTagItem::RootItem, CTagItemData(), this, 0);
    // HACK:
    m_tagRootItem->addChild(new CTagItem(CTagItem::Tag, CTagItemData(), this));
    m_tagRootItem->addChild(new CTagItem(CTagItem::ReadLater, CTagItemData(), this));
    m_tagRootItem->addChild(new CTagItem(CTagItem::Favorites, CTagItemData(), this));
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
