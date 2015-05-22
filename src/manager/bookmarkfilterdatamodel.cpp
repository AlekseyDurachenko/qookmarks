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
#include "bookmarkfilterdatamodel.h"
#include "bookmarkfilter.h"
#include "bookmarkitem.h"
#include "bookmarkmgr.h"
#include "manager.h"
#include "prj.h"
#include "singleton.h"
#include "tagmgr.h"
#include <QDebug>


BookmarkFilterDataModel::BookmarkFilterDataModel(QObject *parent) :
     AbstractBookmarkDataModel(parent)
{
    m_filter = 0;

    connect(GBookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(bookmarkMgr_inserted(int,int)));
    connect(GBookmarkMgr(), SIGNAL(aboutToBeRemoved(int,int)),
            this, SLOT(bookmarkMgr_aboutToBeRemoved(int,int)));
    connect(GBookmarkMgr(), SIGNAL(dataChanged(BookmarkItem*)),
            this, SLOT(bookmarkMgr_dataChanged(BookmarkItem*)));
    connect(GBookmarkMgr(), SIGNAL(tagsChanged(BookmarkItem*)),
            this, SLOT(bookmarkMgr_tagsChanged(BookmarkItem*)));
}

BookmarkFilterDataModel::~BookmarkFilterDataModel()
{
}

void BookmarkFilterDataModel::setFilter(AbstractBookmarkFilter *filter)
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

int BookmarkFilterDataModel::count() const
{
    return m_bookmarks.count();
}

BookmarkItem *BookmarkFilterDataModel::at(int index) const
{
    return m_bookmarks.at(index);
}

void BookmarkFilterDataModel::invalidate()
{
    m_bookmarks.clear();
    if (m_filter)
        foreach (BookmarkItem *item, GBookmarkMgr()->bookmarks())
            if (m_filter->validate(item))
                m_bookmarks.push_back(item);

    emit reseted();
}

void BookmarkFilterDataModel::invalidate(BookmarkItem *item,
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

void BookmarkFilterDataModel::insert(BookmarkItem *item)
{
    int index = m_bookmarks.count();
    emit aboutToBeInserted(index, index);
    m_bookmarks.push_back(item);
    emit inserted(index, index);
}

void BookmarkFilterDataModel::remove(int index)
{
    emit aboutToBeRemoved(index, index);
    m_bookmarks.removeAt(index);
    emit removed(index, index);
}

void BookmarkFilterDataModel::filter_changed()
{
    invalidate();
}

void BookmarkFilterDataModel::filter_destroyed()
{
    m_filter = 0;
    invalidate();
}

void BookmarkFilterDataModel::bookmarkMgr_inserted(int first, int last)
{
    for (int i = first; i <= last; ++i)
        invalidate(GBookmarkMgr()->at(i));
}

void BookmarkFilterDataModel::bookmarkMgr_aboutToBeRemoved(int first, int last)
{
    for (int i = first; i <= last; ++i)
    {
        int index = m_bookmarks.indexOf(GBookmarkMgr()->at(i));
        if (index != -1)
            remove(index);
    }
}

void BookmarkFilterDataModel::bookmarkMgr_dataChanged(BookmarkItem *item)
{
    invalidate(item, true);
}

void BookmarkFilterDataModel::bookmarkMgr_tagsChanged(BookmarkItem *item)
{
    invalidate(item);
}
