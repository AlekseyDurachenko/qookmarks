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

    switch (m_type)
    {
    case RootItem:
        m_data.setName("/");
        break;
    case Tag:
        m_data.setName("Tag");
        break;
    case Favorites:
        m_data.setName("Favorites");
        break;
    case Rated:
        m_data.setName("Rated Bookmarks");
        break;
    case ReadLater:
        m_data.setName("Read it Later");
        break;
    case Bookmarks:
        m_data.setName("Bookmarks");
        break;
    case Trash:
        m_data.setName("Deleted Bookmarks");
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
    if (m_parent)
        return m_parent->childIndexOf(const_cast<CTagItem *>(this));

    return -1;
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

bool CTagItem::setData(const CTagItemData &data)
{
    if (m_parent && m_parent->searchHash().contains(data.name()))
        return false;

    m_data = data;
    if (m_mgr)
        m_mgr->callbackTagDataChanged(this);

    return true;
}

int CTagItem::childCount() const
{
    return m_childList.count();
}

CTagItem *CTagItem::childAt(int row) const
{
    return m_childList.at(row);
}

CTagItem *CTagItem::findChild(const QString &name) const
{
    return m_searchHash.value(name, 0);
}

QList<CTagItem *> CTagItem::fetchAllSubtags() const
{
    QList<CTagItem *> tagList;

    foreach (CTagItem *item, m_childList)
        tagList += (item->fetchAllSubtags());
    tagList.push_back(const_cast<CTagItem *>(this));

    return tagList;
}

void CTagItem::addChild(CTagItem *item)
{
    int row = m_childList.count();
    item->setParent(this);
    m_childList.push_back(item);
    m_searchHash[item->data().name()] = item;
    if (m_mgr)
        m_mgr->callbackTagInserted(this, row, row);
}

CTagItem *CTagItem::takeChild(int row)
{
    CTagItem *item = m_childList.takeAt(row);
    m_searchHash.remove(item->data().name());
    if (m_mgr)
        m_mgr->callbackTagRemoved(this, row, row);
    return item;
}

int CTagItem::childIndexOf(CTagItem *item) const
{
    return m_childList.indexOf(item);
}

void CTagItem::setParent(CTagItem *parent)
{
    m_parent = parent;
}

QHash<QString, CTagItem *> &CTagItem::searchHash()
{
    return m_searchHash;
}
