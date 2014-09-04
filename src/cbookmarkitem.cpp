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
#include "ctagitem.h"

CBookmarkItem::CBookmarkItem(int id, const QString &title, CBookmarkMgr *mgr,
        CBookmarkItem *parent)
{
    m_row = -1;
    m_id = id;
    m_title = title;
    m_mgr = mgr;
    m_parent = parent;
}

CBookmarkItem::~CBookmarkItem()
{
}

void CBookmarkItem::setTitle(const QString &title)
{
    m_title = title;
}

void CBookmarkItem::addTag(CTagItem *tagItem)
{
    m_tagList.push_back(tagItem);
}

void CBookmarkItem::removeTag(CTagItem *tagItem)
{
    m_tagList.removeAll(tagItem);
}

void CBookmarkItem::add(CBookmarkItem *item)
{
    m_childList.push_back(item);
}

void CBookmarkItem::remove(int row)
{
    m_childList.removeAt(row);
}

void CBookmarkItem::setRow(int row)
{
    m_row = row;
}

