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
#ifndef TAGITEMMODEL_H
#define TAGITEMMODEL_H

#include "tagitem.h"
#include <QAbstractItemModel>


class TagItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TagItemModel(QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
protected slots:
    void tagMgr_aboutToBeInserted(TagItem *parent ,int first, int last);
    void tagMgr_inserted(TagItem *parent, int first, int last);
    virtual void tagMgr_aboutToBeRemoved(TagItem *parent, int first, int last);
    void tagMgr_removed(TagItem *parent, int first, int last);
    void tagMgr_aboutToBeMoved(TagItem *srcParent, int srcFirst, int srcLast,
                               TagItem *dstParent, int dstIndex);
    void tagMgr_moved(TagItem *srcParent, int srcFirst, int srcLast,
                      TagItem *dstParent, int dstIndex);
    void tagMgr_dataChanged(TagItem *item);
};


#endif // CHECKEDTAGITEMMODEL_H
