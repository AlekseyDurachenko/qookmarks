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
#include "tagitem.h"
#include "bookmarkitem.h"
#include "bookmarkmgr.h"
#include "tagmgr.h"


TagItem::TagItem(const Tag &data, TagMgr *mgr, TagItem *parent)
{
    m_data = data;
    m_tagMgr = mgr;
    m_parent = parent;
}

TagItem::~TagItem()
{
    removeAll();
    notifyBookmarkAboutDestroyed();
}

QSet<BookmarkItem *> TagItem::bookmarksRecursively() const
{
    QSet<BookmarkItem *> bookmarks = m_bookmarks;
    foreach (TagItem *item, m_children)
        bookmarks += item->bookmarksRecursively();

    return bookmarks;
}

void TagItem::bookmarkAdd(BookmarkItem *item)
{
    if (m_bookmarks.contains(item))
        return;

    m_bookmarks.insert(item);
    item->callbackTagAdd(this);
    m_tagMgr->callbackBookmarksChanged(this);
}

void TagItem::bookmarkRemove(BookmarkItem *item)
{
    if (!m_bookmarks.contains(item))
        return;

    m_bookmarks.remove(item);
    item->callbackTagRemove(this);
    m_tagMgr->callbackBookmarksChanged(this);
}

int TagItem::indexOf(const QString &name) const
{
    for (int i = 0; i < m_children.count(); ++i)
        if (m_children.at(i)->data().name() == name)
            return i;

    return -1;
}

QSet<TagItem *> TagItem::recursiveChildren() const
{
    QSet<TagItem *> tags;

    tags += const_cast<TagItem *>(this);
    foreach (TagItem *item, m_children)
        tags += item->recursiveChildren();

    return tags;
}

TagItem *TagItem::find(const QString &name) const
{
    foreach (TagItem *tag, m_children)
        if (tag->data().name() == name)
            return tag;

    return 0;
}

bool TagItem::aboveOf(TagItem *item) const
{
    // itself is not above of itself
    if (this == item)
        return false;

    for (item = item->parent(); item != 0; item = item->parent())
        if (item == this)
            return true;

    return false;
}

QStringList TagItem::path() const
{
    QStringList result;
    for (const TagItem *item = this; item->parent(); item = item->parent())
        result.push_front(item->data().name());

    return result;
}

TagItem *TagItem::add(const Tag &data)
{
    if (find(data.name()))
        return 0;

    TagItem *item = new TagItem(data, m_tagMgr, this);
    int index = m_children.count();

    m_tagMgr->callbackAboutToBeInserted(this, index, index);
    m_children.push_back(item);
    m_tagMgr->callbackInserted(this, index, index);

    return item;
}

TagItem *TagItem::replace(const Tag &data)
{
    TagItem *item = find(data.name());
    if (item)
    {
        item->setData(data);
    }
    else
    {
        item = new TagItem(data, m_tagMgr, this);
        int index = m_children.count();

        m_tagMgr->callbackAboutToBeInserted(this, index, index);
        m_children.push_back(item);
        m_tagMgr->callbackInserted(this, index, index);
    }
    return item;
}

void TagItem::removeAt(int index)
{
    m_tagMgr->callbackAboutToBeRemoved(this, index, index);
    delete m_children.takeAt(index);
    m_tagMgr->callbackRemoved(this, index, index);
}

void TagItem::removeAll()
{
    if (m_children.isEmpty())
        return;

    int last = m_children.count() - 1;

    m_tagMgr->aboutToBeRemoved(this, 0, last);
    while (m_children.count())
        delete m_children.takeLast();
    m_tagMgr->callbackRemoved(this, 0, last);
}

bool TagItem::moveTo(TagItem *newParent)
{
    if (aboveOf(newParent))
        return false;

    TagItem *oldParent = m_parent;
    int oldIndex = index();
    int newIndex = newParent->count();
    m_tagMgr->callbackAboutToBeMoved(this);
    m_tagMgr->callbackAboutToBeMoved(oldParent, oldIndex, oldIndex, newParent, newIndex);
    newParent->add(oldParent->takeAt(oldIndex));
    m_tagMgr->callbackMoved(oldParent, oldIndex, oldIndex, newParent, newIndex);
    m_tagMgr->callbackMoved(this);

    return true;
}

bool TagItem::setData(const Tag &data)
{
    TagItem *item = m_parent->find(data.name());
    if (item && item != this)
        return false;

    if (m_data != data)
    {
        m_tagMgr->callbackAboutToBeDataChanged(this);
        m_data = data;
        m_tagMgr->callbackDataChanged(this);
    }

    return true;
}

bool TagItem::checkIntersection(const QSet<TagItem *> &a,
                                 const QSet<TagItem *> &b)
{
    if (a.count() < b.count()) // optimization
    {
        foreach (TagItem *tag, a)
            if (b.contains(tag))
                return true;
    }
    else
    {
        foreach (TagItem *tag, b)
            if (a.contains(tag))
                return true;
    }

    return false;
}

QVariant TagItem::variantFromPtr(TagItem *item)
{
    return QVariant::fromValue<void *>(static_cast<void *>(item));
}

TagItem *TagItem::variantToPtr(const QVariant &data)
{
    return static_cast<TagItem *>(data.value<void *>());
}

void TagItem::setParent(TagItem *parent)
{
    m_parent = parent;
}

void TagItem::add(TagItem *item)
{
    item->setParent(this);
    m_children.push_back(item);
}

TagItem *TagItem::takeAt(int index)
{
    TagItem *item = m_children.takeAt(index);
    item->setParent(0);
    return item;
}

void TagItem::notifyBookmarkAboutDestroyed()
{
    foreach (BookmarkItem *item, m_bookmarks)
    {
        m_bookmarks.remove(item);
        item->callbackTagRemove(this);
    }
}

void TagItem::callbackBookmarkDestroyed(BookmarkItem *bookmark)
{
    m_bookmarks.remove(bookmark);
    m_tagMgr->callbackBookmarksChanged(this);
}
