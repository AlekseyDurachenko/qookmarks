// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "tagmgr.h"
#include "bookmarkitem.h"
#include "bookmarkmgr.h"
#include "manager.h"
#include "tagitem.h"


TagMgr::TagMgr(Manager *mgr) : QObject(mgr)
{
    m_mgr = mgr;
    m_rootItem = new TagItem(Tag(), this, 0);
}

TagMgr::~TagMgr()
{
    delete m_rootItem;
}

TagItem *TagMgr::findByPath(const QStringList &path) const
{
    if (path.isEmpty())
        return m_rootItem;

    TagItem *item = rootItem();
    for (int index = 0; index < path.count() && item; ++index)
        item = item->find(path.at(index));

    return item;
}

void TagMgr::callbackAboutToBeInserted(TagItem *parent, int first, int last)
{
    emit aboutToBeInserted(parent, first, last);
}

void TagMgr::callbackInserted(TagItem *parent, int first, int last)
{
    emit inserted(parent, first, last);
}

void TagMgr::callbackAboutToBeRemoved(TagItem *parent, int first, int last)
{
    emit aboutToBeRemoved(parent, first, last);
}

void TagMgr::callbackRemoved(TagItem *parent, int first, int last)
{
    emit removed(parent, first, last);
}

void TagMgr::callbackAboutToBeMoved(TagItem *item)
{
    foreach (BookmarkItem *bookmark, item->bookmarksRecursively())
        m_mgr->bookmarkMgr()->callbackAboutToBeTagsChanged(bookmark);
}

void TagMgr::callbackAboutToBeMoved(TagItem *sourceParent, int sourceFirst,
        int sourceLast, TagItem *destinationParent, int destinationIndex)
{
    emit aboutToBeMoved(sourceParent, sourceFirst, sourceLast,
                        destinationParent, destinationIndex);
}

void TagMgr::callbackMoved(TagItem *sourceParent, int sourceFirst,
        int sourceLast, TagItem *destinationParent, int destinationIndex)
{
    emit moved(sourceParent, sourceFirst, sourceLast,
               destinationParent, destinationIndex);
}

void TagMgr::callbackMoved(TagItem *item)
{
    foreach (BookmarkItem *bookmark, item->bookmarksRecursively())
        m_mgr->bookmarkMgr()->callbackTagsChanged(bookmark);
}

void TagMgr::callbackAboutToBeDataChanged(TagItem *item)
{
    emit aboutToBeDataChanged(item);
}

void TagMgr::callbackDataChanged(TagItem *item)
{
    emit dataChanged(item);
}

void TagMgr::callbackBookmarksChanged(TagItem *item)
{
    emit bookmarksChanged(item);
}
