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


CBookmarkItem::CBookmarkItem(const CBookmarkItemData &data, CBookmarkMgr *mgr)
{
    m_data = data;
    m_mgr = mgr;
}

bool CBookmarkItem::tagCanAdd(CTagItem *tag)
{
    switch (tag->type())
    {
    case CTagItem::Tag:
    case CTagItem::Favorites:
    case CTagItem::ReadLater:
    case CTagItem::Trash:
        return true;
    default:
        return false;
    }

    return false;
}

bool CBookmarkItem::tagAdd(CTagItem *tag)
{
    // we can't accept the top level items
    // but for some type of it we can processing (syntax sugar)
    if (tag->type() != CTagItem::Tag)
    {
        switch (tag->type())
        {
        case CTagItem::Favorites:
            if (!m_data.isFavorite())
                m_data.setFavorite(true);
            else
                return true;
            break;
        case CTagItem::ReadLater:
            if (!m_data.isReadLater())
                m_data.setReadLater(true);
            else
                return true;
            break;
        case CTagItem::Trash:
            if (!m_data.isDeleted())
                m_data.setDeleted(true);
            else
                return true;
            break;
        default:
            return false;
        }

        m_mgr->callbackBookmarkDataChanged(this);
        return true;
    }

    // tag already exists
    if (m_tags.contains(tag))
        return false;

    m_tags.insert(tag);
    m_mgr->callbackBookmarkDataChanged(this);

    return true;
}

void CBookmarkItem::tagRemove(CTagItem *tag)
{
    if (m_tags.remove(tag))
        m_mgr->callbackBookmarkDataChanged(this);
}

void CBookmarkItem::tagRemoveAll()
{
    if (m_tags.count() == 0)
        return;

    m_tags.clear();
    m_mgr->callbackBookmarkDataChanged(this);
}

bool CBookmarkItem::canSetData(const CBookmarkItemData &data)
{
    // unique field (url) is already exists
    CBookmarkItem *bookmark = m_mgr->bookmarkFind(data.url());
    if (bookmark && bookmark != this)
        return false;

    return true;
}

bool CBookmarkItem::setData(const CBookmarkItemData &data)
{
    if (!canSetData(data))
        return false;

    // nothing to be update
    if (m_data == data)
        return true;

    m_data = data;
    m_mgr->callbackBookmarkDataChanged(this);

    return true;
}
