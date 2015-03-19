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
#include "ctagitem.h"
#include "cbookmarkitem.h"
#include "cbookmarkmgr.h"
#include "ctagmgr.h"


CTagItem::CTagItem(const CTag &data, CTagMgr *mgr, CTagItem *parent)
{
    m_data = data;
    m_tagMgr = mgr;
    m_parent = parent;
}

CTagItem::~CTagItem()
{
    removeAll();
    notifyBookmarkAboutDestroyed();
}

QSet<CBookmarkItem *> CTagItem::bookmarksRecursively() const
{
    QSet<CBookmarkItem *> bookmarks = m_bookmarks;
    foreach (CTagItem *item, m_children)
        bookmarks += item->bookmarksRecursively();

    return bookmarks;
}

void CTagItem::bookmarkAdd(CBookmarkItem *item)
{
    if (m_bookmarks.contains(item))
        return;

    m_bookmarks.insert(item);
    item->callbackTagAdd(this);
    m_tagMgr->callbackBookmarksChanged(this);
}

void CTagItem::bookmarkRemove(CBookmarkItem *item)
{
    if (!m_bookmarks.contains(item))
        return;

    m_bookmarks.remove(item);
    item->callbackTagRemove(this);
    m_tagMgr->callbackBookmarksChanged(this);
}

int CTagItem::indexOf(const QString &name) const
{
    for (int i = 0; i < m_children.count(); ++i)
        if (m_children.at(i)->data().name() == name)
            return i;

    return -1;
}

CTagItem *CTagItem::find(const QString &name) const
{
    foreach (CTagItem *tag, m_children)
        if (tag->data().name() == name)
            return tag;

    return 0;
}

bool CTagItem::aboveOf(CTagItem *item) const
{
    // itself is not above of itself
    if (this == item)
        return false;

    for (item = item->parent(); item != 0; item = item->parent())
        if (item == this)
            return true;

    return false;
}

QStringList CTagItem::path() const
{
    QStringList result;
    for (const CTagItem *item = this; item->parent(); item = item->parent())
        result.push_front(item->data().name());

    return result;
}

CTagItem *CTagItem::add(const CTag &data)
{
    if (find(data.name()))
        return 0;

    CTagItem *item = new CTagItem(data, m_tagMgr, this);
    int index = m_children.count();

    m_tagMgr->callbackAboutToBeInserted(this, index, index);
    m_children.push_back(item);
    m_tagMgr->callbackInserted(this, index, index);

    return item;
}

CTagItem *CTagItem::replace(const CTag &data)
{
    CTagItem *item = find(data.name());
    if (item)
    {
        item->setData(data);
    }
    else
    {
        item = new CTagItem(data, m_tagMgr, this);
        int index = m_children.count();

        m_tagMgr->callbackAboutToBeInserted(this, index, index);
        m_children.push_back(item);
        m_tagMgr->callbackInserted(this, index, index);
    }
    return item;
}

void CTagItem::removeAt(int index)
{
    m_tagMgr->callbackAboutToBeRemoved(this, index, index);
    delete m_children.takeAt(index);
    m_tagMgr->callbackRemoved(this, index, index);
}

void CTagItem::removeAll()
{
    if (m_children.isEmpty())
        return;

    int last = m_children.count() - 1;

    m_tagMgr->aboutToBeRemoved(this, 0, last);
    while (m_children.count())
        delete m_children.takeLast();
    m_tagMgr->callbackRemoved(this, 0, last);
}

bool CTagItem::moveTo(CTagItem *newParent)
{
    if (aboveOf(newParent))
        return false;

    CTagItem *oldParent = m_parent;
    int oldIndex = index();
    int newIndex = newParent->count();
    m_tagMgr->callbackAboutToBeMoved(this);
    m_tagMgr->callbackAboutToBeMoved(oldParent, oldIndex, oldIndex, newParent, newIndex);
    newParent->add(oldParent->takeAt(oldIndex));
    m_tagMgr->callbackMoved(oldParent, oldIndex, oldIndex, newParent, newIndex);
    m_tagMgr->callbackMoved(this);

    return true;
}

bool CTagItem::setData(const CTag &data)
{
    CTagItem *item = m_parent->find(data.name());
    if (item && item != this)
        return false;

    if (m_data != data)
    {
        CTag old = m_data;
        m_data = data;
        m_tagMgr->callbackDataChanged(this, old, data);
    }

    return true;
}

bool CTagItem::checkIntersection(const QSet<CTagItem *> &a,
                                 const QSet<CTagItem *> &b)
{
    if (a.count() < b.count()) // optimization
    {
        foreach (CTagItem *tag, a)
            if (b.contains(tag))
                return true;
    }
    else
    {
        foreach (CTagItem *tag, b)
            if (a.contains(tag))
                return true;
    }

    return false;
}

void CTagItem::setParent(CTagItem *parent)
{
    m_parent = parent;
}

void CTagItem::add(CTagItem *item)
{
    item->setParent(this);
    m_children.push_back(item);
}

CTagItem *CTagItem::takeAt(int index)
{
    CTagItem *item = m_children.takeAt(index);
    item->setParent(0);
    return item;
}

void CTagItem::notifyBookmarkAboutDestroyed()
{
    foreach (CBookmarkItem *item, m_bookmarks)
    {
        m_bookmarks.remove(item);
        item->callbackTagRemove(this);
    }
}

void CTagItem::callbackBookmarkDestroyed(CBookmarkItem *bookmark)
{
    m_bookmarks.remove(bookmark);
    m_tagMgr->callbackBookmarksChanged(this);
}
