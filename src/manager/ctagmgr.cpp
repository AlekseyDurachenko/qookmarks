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
#include "ctagmgr.h"
#include "cmanager.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"


CTagMgr::CTagMgr(CManager *mgr) : QObject(mgr)
{
    m_mgr = mgr;
    m_rootItem = new CTagItem(CTag(), this, 0);
}

CTagMgr::~CTagMgr()
{
    delete m_rootItem;
}

CTagItem *CTagMgr::findByPath(const QStringList &path) const
{
    CTagItem *item = rootItem();
    for (int index = 0; index < path.count() && item; ++index)
        item = item->find(path.at(index));

    return item;
}

void CTagMgr::callbackAboutToBeInserted(CTagItem *parent, int first, int last)
{
    emit aboutToBeInserted(parent, first, last);
}

void CTagMgr::callbackInserted(CTagItem *parent, int first, int last)
{
    emit inserted(parent, first, last);
}

void CTagMgr::callbackAboutToBeRemoved(CTagItem *parent, int first, int last)
{
    emit aboutToBeRemoved(parent, first, last);
}

void CTagMgr::callbackRemoved(CTagItem *parent, int first, int last)
{
    emit removed(parent, first, last);
}

void CTagMgr::callbackAboutToBeMoved(CTagItem *item)
{
    Q_UNUSED(item);
}

void CTagMgr::callbackAboutToBeMoved(CTagItem *sourceParent, int sourceFirst,
        int sourceLast, CTagItem *destinationParent, int destinationIndex)
{
    emit aboutToBeMoved(sourceParent, sourceFirst, sourceLast,
                        destinationParent, destinationIndex);
}

void CTagMgr::callbackMoved(CTagItem *sourceParent, int sourceFirst,
        int sourceLast, CTagItem *destinationParent, int destinationIndex)
{
    emit moved(sourceParent, sourceFirst, sourceLast,
               destinationParent, destinationIndex);
}

void CTagMgr::callbackMoved(CTagItem *item)
{
    foreach (CBookmarkItem *bookmark, item->bookmarksRecursively())
        m_mgr->bookmarkMgr()->callbackTagsChanged(bookmark);
}

void CTagMgr::callbackDataChanged(CTagItem *item, const CTag &oldTag,
        const CTag &newTag)
{
    emit dataChanged(item, oldTag, newTag);
}

void CTagMgr::callbackBookmarksChanged(CTagItem *item)
{
    emit bookmarksChanged(item);
}
