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
#include <QDebug>

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
    foreach (CTagItem *item, m_tagList)
        item->bookmarkRemove(this);

    foreach (CBookmarkItem *item, m_childList)
        delete item;
}

void CBookmarkItem::setTitle(const QString &title)
{
    m_title = title;
}

bool CBookmarkItem::tagContains(CTagItem *item) const
{
    return m_tagList.contains(item);
}

void CBookmarkItem::tagAdd(CTagItem *item)
{
    qDebug() << title() << "CBookmarkItem::tagAdd" << item->title();
    item->bookmarkAdd(this);
    m_tagList.push_back(item);
}

void CBookmarkItem::tagRemove(CTagItem *item)
{
    qDebug() << title() << "CBookmarkItem::tagRemove" << item->title();
    item->bookmarkRemove(this);
    m_tagList.removeAll(item);
}

void CBookmarkItem::addChild(CBookmarkItem *item)
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

