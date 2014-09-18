// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef CTAGITEMMODEL_H
#define CTAGITEMMODEL_H

#include "ctagitem.h"
#include <QAbstractItemModel>

class CTagItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CTagItemModel(QObject *parent = 0);
    explicit CTagItemModel(CTagItem *rootItem, QObject *parent = 0);

    inline CTagItem *rootItem() const;
    void setRootItem(CTagItem *rootItem);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
private slots:
    void onTagInserted(CTagItem *parent,int first, int last);
    void onTagRemoved(CTagItem *parent,int first, int last);
    void onTagDataChanged(CTagItem *parent,int first, int last);
    void onMgrDestroyed();
private:
    CTagItem *m_rootItem;
};

CTagItem *CTagItemModel::rootItem() const
{
    return m_rootItem;
}

#endif // CTAGITEMMODEL_H
