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
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "cmanager.h"


CBookmarkMgr::CBookmarkMgr(CManager *mgr) : QObject(mgr)
{
    m_mgr = mgr;
}

CBookmarkMgr::~CBookmarkMgr()
{
    removeAll();
}

int CBookmarkMgr::indexOf(CBookmarkItem *item) const
{
    return m_bookmarkItems.indexOf(item);
}

int CBookmarkMgr::indexOf(const QUrl &url) const
{
    for (int i = 0; i < m_bookmarkItems.count(); ++i)
        if (m_bookmarkItems.at(i)->data().url() == url)
            return i;
    return -1;
}

CBookmarkItem *CBookmarkMgr::find(const QUrl &url) const
{
    int index = indexOf(url);
    if (index == -1)
        return 0;
    return m_bookmarkItems.at(index);
}

CBookmarkItem *CBookmarkMgr::add(const CBookmark &data)
{
    if (find(data.url()))
        return 0;

    CBookmarkItem *item = new CBookmarkItem(data, this);
    int index = m_bookmarkItems.count();

    emit aboutToBeInserted(index, index);
    m_bookmarkItems.push_back(item);
    emit inserted(index, index);

    return item;
}

CBookmarkItem *CBookmarkMgr::replace(const CBookmark &data)
{
    CBookmarkItem *item = find(data.url());
    if (item)
    {
        item->setData(data);
    }
    else
    {
        item = new CBookmarkItem(data, this);
        int index = m_bookmarkItems.count();

        emit aboutToBeInserted(index, index);
        m_bookmarkItems.push_back(item);
        emit inserted(index, index);
    }
    return item;
}

void CBookmarkMgr::removeAt(int index)
{
    emit aboutToBeRemoved(index, index);
    delete m_bookmarkItems.takeAt(index);
    emit removed(index, index);
}

void CBookmarkMgr::removeAll()
{
    if (m_bookmarkItems.isEmpty())
        return;

    int last = m_bookmarkItems.count() - 1;

    emit aboutToBeRemoved(0, last);
    while (m_bookmarkItems.count())
        delete m_bookmarkItems.takeLast();
    emit removed(0, last);
}

void CBookmarkMgr::callbackDataChanged(CBookmarkItem *item,
        const CBookmark& oldData, const CBookmark &newData)
{
    emit dataChanged(item, oldData, newData);
}

void CBookmarkMgr::callbackTagsChanged(CBookmarkItem *item)
{
    emit tagsChanged(item);
}
