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
#ifndef CBOOKMARKFILTERDATAMODEL_H
#define CBOOKMARKFILTERDATAMODEL_H

#include <QObject>
#include "cabstractbookmarkdatamodel.h"
class CManager;
class CAbstractBookmarkFilter;


class CBookmarkFilterDataModel : public CAbstractBookmarkDataModel
{
    Q_OBJECT
public:
    explicit CBookmarkFilterDataModel(QObject *parent = 0);
    virtual ~CBookmarkFilterDataModel();

    inline CAbstractBookmarkFilter *filter() const;
    void setFilter(CAbstractBookmarkFilter *filter);

    virtual int count() const;
    virtual CBookmarkItem *at(int index) const;
    inline int indexOf(CBookmarkItem *item) const;
    inline QList<CBookmarkItem *> bookmarks() const;
private:
    void invalidate();
    void invalidate(CBookmarkItem *item, bool hasDataChanges = false);
    void insert(CBookmarkItem *item);
    void remove(int index);
private slots:
    void filter_changed();
    void filter_destroyed();
    void bookmarkMgr_inserted(int first, int last);
    void bookmarkMgr_aboutToBeRemoved(int first, int last);
    void bookmarkMgr_dataChanged(CBookmarkItem *item);
    void bookmarkMgr_tagsChanged(CBookmarkItem *item);
private:
    QList<CBookmarkItem *> m_bookmarks;
    CAbstractBookmarkFilter *m_filter;
};


CAbstractBookmarkFilter *CBookmarkFilterDataModel::filter() const
{
    return m_filter;
}

int CBookmarkFilterDataModel::indexOf(CBookmarkItem *item) const
{
    return m_bookmarks.indexOf(item);
}

QList<CBookmarkItem *> CBookmarkFilterDataModel::bookmarks() const
{
    return m_bookmarks;
}


#endif // CBOOKMARKFILTERDATAMODEL_H
