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
#ifndef BOOKMARKFILTERDATAMODEL_H
#define BOOKMARKFILTERDATAMODEL_H

#include "abstractbookmarkdatamodel.h"
#include <QObject>
class Manager;
class AbstractBookmarkFilter;


class BookmarkFilterDataModel : public AbstractBookmarkDataModel
{
    Q_OBJECT
public:
    explicit BookmarkFilterDataModel(QObject *parent = 0);
    virtual ~BookmarkFilterDataModel();

    inline AbstractBookmarkFilter *filter() const;
    void setFilter(AbstractBookmarkFilter *filter);

    virtual int count() const;
    virtual BookmarkItem *at(int index) const;
    inline int indexOf(BookmarkItem *item) const;
    inline QList<BookmarkItem *> bookmarks() const;
private:
    void invalidate();
    void invalidate(BookmarkItem *item, bool hasDataChanges = false);
    void insert(BookmarkItem *item);
    void remove(int index);
private slots:
    void filter_changed();
    void filter_destroyed();
    void bookmarkMgr_inserted(int first, int last);
    void bookmarkMgr_aboutToBeRemoved(int first, int last);
    void bookmarkMgr_dataChanged(BookmarkItem *item);
    void bookmarkMgr_tagsChanged(BookmarkItem *item);
private:
    QList<BookmarkItem *> m_bookmarks;
    AbstractBookmarkFilter *m_filter;
};


AbstractBookmarkFilter *BookmarkFilterDataModel::filter() const
{
    return m_filter;
}

int BookmarkFilterDataModel::indexOf(BookmarkItem *item) const
{
    return m_bookmarks.indexOf(item);
}

QList<BookmarkItem *> BookmarkFilterDataModel::bookmarks() const
{
    return m_bookmarks;
}


#endif // BOOKMARKFILTERDATAMODEL_H
