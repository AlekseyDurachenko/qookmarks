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
#include <QDebug>

CTagItem::CTagItem(CTagItem::Type type, const CTagItemData &data,
        CBookmarkMgr *mgr, CTagItem *parent)
{
    m_row = -1;
    m_id = -1;
    m_type = type;
    m_data = data;
    m_mgr = mgr;
    m_parent = parent;
}

CTagItem::CTagItem(int id, CTagItem::Type type, const CTagItemData &data,
        CBookmarkMgr *mgr, CTagItem *parent)
{
    m_row = -1;
    m_id = id;
    m_type = type;
    m_data = data;
    m_mgr = mgr;
    m_parent = parent;
}

CTagItem::CTagItem(int row, int id, CTagItem::Type type,
        const CTagItemData &data, CBookmarkMgr *mgr, CTagItem *parent)
{
    m_row = row;
    m_id = id;
    m_type = type;
    m_data = data;
    m_mgr = mgr;
    m_parent = parent;
}

CTagItem::~CTagItem()
{
    foreach (CTagItem *item, m_childList)
        delete item;
}

void CTagItem::setData(const CTagItemData &data)
{
    m_data = data;
    if (m_mgr)
        m_mgr->callbackTagDataChanged(this);
}

int CTagItem::childCount() const
{
    return m_childList.count();
}

CTagItem *CTagItem::childAt(int row) const
{
    return m_childList.at(row);
}

void CTagItem::addChild(CTagItem *item)
{
    item->setRow(m_childList.count());
    item->setParent(this);
    m_childList.push_back(item);
}

CTagItem *CTagItem::takeChild(int row)
{
    return m_childList.takeAt(row);
}

void CTagItem::setParent(CTagItem *parent)
{
    m_parent = parent;
}

void CTagItem::setRow(int row)
{
    m_row = row;
}

void CTagItem::setId(int id)
{
    m_id = id;
}
