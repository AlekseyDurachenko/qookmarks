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
#include "cbookmarkfilterdatamodel.h"
#include "cmanager.h"
#include "ctagmgr.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "cbookmarkfilter.h"
#include <QDebug>


CBookmarkFilterDataModel::CBookmarkFilterDataModel(CManager *manager,
        QObject *parent) :  QObject(parent)
{
    m_manager = 0;
    m_filter = 0;

    setManager(manager);
}

CBookmarkFilterDataModel::~CBookmarkFilterDataModel()
{
}

void CBookmarkFilterDataModel::setManager(CManager *manager)
{
    if (m_manager == manager)
        return;

    if (m_manager)
    {
        disconnect(m_manager->tagMgr(), 0, this, 0);
        disconnect(m_manager->bookmarkMgr(), 0, this, 0);
    }

    m_manager = manager;
    if (m_manager)
    {
        connect(m_manager->bookmarkMgr(), SIGNAL(destroyed()),
                this, SLOT(bookmarkMgr_destroyed()));
        connect(m_manager->bookmarkMgr(), SIGNAL(inserted(int,int)),
                this, SLOT(bookmarkMgr_inserted(int,int)));
        connect(m_manager->bookmarkMgr(), SIGNAL(aboutToBeRemoved(int,int)),
                this, SLOT(bookmarkMgr_aboutToBeRemoved(int,int)));
        connect(m_manager->bookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*,CBookmark,CBookmark)),
                this, SLOT(bookmarkMgr_dataChanged(CBookmarkItem*)));
        connect(m_manager->bookmarkMgr(), SIGNAL(tagsChanged(CBookmarkItem*)),
                this, SLOT(bookmarkMgr_tagsChanged(CBookmarkItem*)));
    }

    invalidate();
}

void CBookmarkFilterDataModel::setFilter(CAbstractBookmarkFilter *filter)
{
    if (m_filter == filter)
        return;

    if (m_filter)
        disconnect(m_filter, 0, this, 0);

    m_filter = filter;
    if (m_filter)
    {

        connect(m_filter, SIGNAL(destroyed()), this, SLOT(filter_destroyed()));
        connect(m_filter, SIGNAL(changed()), this, SLOT(filter_changed()));
    }

    invalidate();
}

void CBookmarkFilterDataModel::invalidate()
{
    m_bookmarks.clear();
    if (m_manager->bookmarkMgr() && m_filter)
        foreach (CBookmarkItem *item, m_manager->bookmarkMgr()->bookmarks())
            if (m_filter->validate(item))
                m_bookmarks.push_back(item);

    emit reseted();
}

void CBookmarkFilterDataModel::invalidate(CBookmarkItem *item,
        bool hasDataChanges)
{
    if (!m_filter)
        return;

    bool valid = m_filter->validate(item);
    int index = indexOf(item);

    if (index == -1 && valid == true)
        insert(item);
    else if (index != -1 && valid == false)
        remove(index);
    else if (index != -1 && hasDataChanges)
        emit dataChanged(index, index);
}

void CBookmarkFilterDataModel::insert(CBookmarkItem *item)
{
    int index = m_bookmarks.count();
    emit aboutToBeInserted(index, index);
    m_bookmarks.push_back(item);
    emit inserted(index, index);
}

void CBookmarkFilterDataModel::remove(int index)
{
    emit aboutToBeRemoved(index, index);
    m_bookmarks.removeAt(index);
    emit removed(index, index);
}

void CBookmarkFilterDataModel::filter_changed()
{
    invalidate();
}

void CBookmarkFilterDataModel::filter_destroyed()
{
    m_filter = 0;
    invalidate();
}

void CBookmarkFilterDataModel::bookmarkMgr_inserted(int first, int last)
{
    for (int i = first; i <= last; ++i)
        invalidate(m_manager->bookmarkMgr()->at(i));
}

void CBookmarkFilterDataModel::bookmarkMgr_aboutToBeRemoved(int first, int last)
{
    for (int i = first; i <= last; ++i)
    {
        int index = m_bookmarks.indexOf(m_manager->bookmarkMgr()->at(i));
        if (index != -1)
            remove(index);
    }
}

void CBookmarkFilterDataModel::bookmarkMgr_dataChanged(CBookmarkItem *item)
{
    invalidate(item, true);
}

void CBookmarkFilterDataModel::bookmarkMgr_tagsChanged(CBookmarkItem *item)
{
    invalidate(item);
}

void CBookmarkFilterDataModel::bookmarkMgr_destroyed()
{
    m_manager = 0;
    invalidate();
}
