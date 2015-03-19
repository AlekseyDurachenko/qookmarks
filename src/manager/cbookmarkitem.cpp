// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "ctagitem.h"
#include "cbookmarkmgr.h"


CBookmarkItem::CBookmarkItem(const CBookmark &data, CBookmarkMgr *bookmarkMgr)
{
    m_data = data;
    m_bookmarkMgr = bookmarkMgr;
}

CBookmarkItem::~CBookmarkItem()
{
    notifyTagAboutDestroyed();
}

int CBookmarkItem::index() const
{
    return m_bookmarkMgr->indexOf(const_cast<CBookmarkItem *>(this));
}

bool CBookmarkItem::setData(const CBookmark &data)
{
    CBookmarkItem *item = m_bookmarkMgr->find(data.url());
    if (item && item != this)
        return false;

    if (m_data != data)
    {
        CBookmark old = m_data;
        m_data = data;
        m_bookmarkMgr->callbackDataChanged(this, old, data);
    }

    return true;
}

void CBookmarkItem::notifyTagAboutDestroyed()
{
    foreach (CTagItem *item, m_tags)
    {
        m_tags.remove(item);
        item->callbackBookmarkDestroyed(this);
    }
}

void CBookmarkItem::callbackTagAdd(CTagItem *tag)
{
    m_tags.insert(tag);
    m_bookmarkMgr->callbackTagsChanged(this);
}

void CBookmarkItem::callbackTagRemove(CTagItem *tag)
{
    m_tags.remove(tag);
    m_bookmarkMgr->callbackTagsChanged(this);
}
