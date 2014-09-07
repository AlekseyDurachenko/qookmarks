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
#include "cbookmarkitem.h"
#include "cbookmarkmgr.h"
#include <QDebug>

CBookmarkItem::CBookmarkItem(const CBookmarkItemData &data, CBookmarkMgr *mgr)
{
    m_row = -1;
    m_id = -1;
    m_data = data;
    m_mgr = mgr;
}

CBookmarkItem::CBookmarkItem(int id, const CBookmarkItemData &data,
        CBookmarkMgr *mgr)
{
    m_row = -1;
    m_id = id;
    m_data = data;
    m_mgr = mgr;
}

CBookmarkItem::CBookmarkItem(int row, int id, const CBookmarkItemData &data,
        CBookmarkMgr *mgr)
{
    m_row = row;
    m_id = id;
    m_data = data;
    m_mgr = mgr;
}

void CBookmarkItem::setData(const CBookmarkItemData &data)
{
    m_data = data;
    if (m_mgr)
        m_mgr->callbackBookmarkDataChanged(this);
}

void CBookmarkItem::setRow(int row)
{
    m_row = row;
}

void CBookmarkItem::setId(int id)
{
    m_id = id;
}
