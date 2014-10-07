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
#include "ctagitem.h"
#include "cbookmarkmgr.h"


CTagItem::CTagItem(CTagItem::Type type, CBookmarkMgr *mgr, CTagItem *parent)
{
    m_type = type;
    m_mgr = mgr;
    m_parent = parent;

    // default names for top level items
    switch (m_type)
    {
    case RootItem:
        m_data.setName(QObject::tr("/"));
        break;
    case Tag:
        m_data.setName(QObject::tr("Tag"));
        break;
    case Favorites:
        m_data.setName(QObject::tr("Favorites"));
        break;
    case Rated:
        m_data.setName(QObject::tr("Rated Bookmarks"));
        break;
    case ReadLater:
        m_data.setName(QObject::tr("Read it Later"));
        break;
    case Bookmarks:
        m_data.setName(QObject::tr("Bookmarks"));
        break;
    case Trash:
        m_data.setName(QObject::tr("Deleted Bookmarks"));
        break;
    }
}

CTagItem::CTagItem(const CTagItemData &data,
        CBookmarkMgr *mgr, CTagItem *parent)
{
    m_type = CTagItem::Tag;
    m_data = data;
    m_mgr = mgr;
    m_parent = parent;
}

CTagItem::~CTagItem()
{
    qDeleteAll(m_childList);
}

int CTagItem::row() const
{
    if (!m_parent)
        return -1;

    return m_parent->childIndexOf(const_cast<CTagItem *>(this));
}

QStringList CTagItem::path() const
{
    QStringList itemPath;
    const CTagItem *item = this;
    while (item && item->type() == CTagItem::Tag)
    {
        itemPath.push_front(item->data().name());
        item = item->parent();
    }

    return itemPath;
}

QIcon CTagItem::icon() const
{
    switch(m_type)
    {
    case CTagItem::Tag:
        return QIcon(":/icons/bookmark-tag.png");
    case CTagItem::Favorites:
        return QIcon(":/icons/bookmark-favorites.png");
    case CTagItem::Rated:
        return QIcon(":/icons/bookmark-rated.png");
    case CTagItem::ReadLater:
        return QIcon(":/icons/bookmark-readlater.png");
    case CTagItem::Bookmarks:
        return QIcon(":/icons/bookmark-bookmarks.png");
    case CTagItem::Trash:
        return QIcon(":/icons/bookmark-trash.png");
    default:
        return QIcon();
    }
}

bool CTagItem::canSetData(const CTagItemData &data)
{
    // we can search for duplicates only when there is a parent
    if (!m_parent)
        return true;

    // unique field (name) is already exists
    CTagItem *item = m_parent->findChild(data.name());
    if (item && item != this)
        return false;

    return true;
}

bool CTagItem::setData(const CTagItemData &data)
{
    if (!canSetData(data))
        return false;

    // nothing to be update
    if (m_data == data)
        return true;

    m_searchHash.remove(m_data.name());
    m_searchHash[data.name()] = this;
    m_data = data;

    m_mgr->callbackTagDataChanged(this);

    return true;
}

void CTagItem::addChild(CTagItem *item)
{
    item->setParent(this);

    int row = m_childList.count();
    m_childList.push_back(item);
    m_searchHash[item->data().name()] = item;

    m_mgr->callbackTagInserted(this, row, row);
}

CTagItem *CTagItem::takeChild(int row)
{
    m_mgr->callbackTagBeginRemove(this, row, row);

    CTagItem *item = m_childList.takeAt(row);
    m_searchHash.remove(item->data().name());

    m_mgr->callbackTagRemoved(this, row, row);

    return item;
}

void CTagItem::setParent(CTagItem *parent)
{
    m_parent = parent;
}
