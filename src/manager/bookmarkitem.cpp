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
#include "bookmarkitem.h"
#include <QVariant>
#include "tagitem.h"
#include "bookmarkmgr.h"


BookmarkItem::BookmarkItem(const Bookmark &data, BookmarkMgr *bookmarkMgr)
{
    m_data = data;
    m_bookmarkMgr = bookmarkMgr;
}

BookmarkItem::~BookmarkItem()
{
    notifyTagAboutDestroyed();
}

int BookmarkItem::index() const
{
    return m_bookmarkMgr->indexOf(const_cast<BookmarkItem *>(this));
}

bool BookmarkItem::setData(const Bookmark &data)
{
    BookmarkItem *item = m_bookmarkMgr->find(data.url());
    if (item && item != this)
        return false;

    if (m_data != data)
    {
        m_bookmarkMgr->callbackAboutToBeDataChanged(this);
        m_data = data;
        m_bookmarkMgr->callbackDataChanged(this);
    }

    return true;
}

QVariant BookmarkItem::variantFromPtr(BookmarkItem *item)
{
    return QVariant::fromValue<void *>(static_cast<void *>(item));
}

BookmarkItem *BookmarkItem::variantToPtr(const QVariant &data)
{
    return static_cast<BookmarkItem *>(data.value<void *>());
}

void BookmarkItem::notifyTagAboutDestroyed()
{
    foreach (TagItem *item, m_tags)
    {
        m_tags.remove(item);
        item->callbackBookmarkDestroyed(this);
    }
}

void BookmarkItem::callbackTagAdd(TagItem *tag)
{
    m_bookmarkMgr->callbackAboutToBeTagsChanged(this);
    m_tags.insert(tag);
    m_bookmarkMgr->callbackTagsChanged(this);
}

void BookmarkItem::callbackTagRemove(TagItem *tag)
{
    m_bookmarkMgr->callbackAboutToBeTagsChanged(this);
    m_tags.remove(tag);
    m_bookmarkMgr->callbackTagsChanged(this);
}
