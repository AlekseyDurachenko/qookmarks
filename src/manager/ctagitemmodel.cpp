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
#include "ctagitemmodel.h"
#include "singleton.h"


CTagItemModel::CTagItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    connect(GTagMgr(), SIGNAL(aboutToBeInserted(CTagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeInserted(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(inserted(CTagItem*,int,int)),
            this, SLOT(tagMgr_inserted(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(aboutToBeRemoved(CTagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeRemoved(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(removed(CTagItem*,int,int)),
            this, SLOT(tagMgr_removed(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(aboutToBeMoved(CTagItem*,int,int,CTagItem*,int)),
            this, SLOT(tagMgr_aboutToBeMoved(CTagItem*,int,int,CTagItem*,int)));
    connect(GTagMgr(), SIGNAL(moved(CTagItem*,int,int,CTagItem*,int)),
            this, SLOT(tagMgr_moved(CTagItem*,int,int,CTagItem*,int)));
    connect(GTagMgr(), SIGNAL(dataChanged(CTagItem*)),
            this, SLOT(tagMgr_dataChanged(CTagItem*)));
}

QVariant CTagItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return QString("%1").arg(item->data().name());

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return QIcon(":/icons/tag-icon-default.png");

    if (role == Qt::UserRole)
        return CTagItem::variantFromPtr(item);

    return QVariant();
}

Qt::ItemFlags CTagItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

   return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant CTagItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal
            && role == Qt::DisplayRole
            && section == 0)
        return tr("Name");

    return QVariant();
}

QModelIndex CTagItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CTagItem *parentItem;
    if (!parent.isValid())
        parentItem = GTagMgr()->rootItem();
    else
        parentItem = static_cast<CTagItem *>(parent.internalPointer());

    return createIndex(row, column, parentItem->at(row));
}

QModelIndex CTagItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CTagItem *childItem = static_cast<CTagItem *>(index.internalPointer());
    CTagItem *parentItem = childItem->parent();

    if (parentItem == GTagMgr()->rootItem())
        return QModelIndex();

    return createIndex(parentItem->index(), 0, parentItem);
}

int CTagItemModel::rowCount(const QModelIndex &parent) const
{
     if (parent.column() > 0)
         return 0;

     CTagItem *parentItem;
     if (!parent.isValid())
         parentItem = GTagMgr()->rootItem();
     else
         parentItem = static_cast<CTagItem *>(parent.internalPointer());

     return parentItem->count();
}

int CTagItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

void CTagItemModel::tagMgr_aboutToBeInserted(CTagItem *parent,
        int first, int last)
{
    if (parent == GTagMgr()->rootItem())
        beginInsertRows(QModelIndex(), first, last);
    else
        beginInsertRows(createIndex(parent->index(), 0, parent), first, last);
}

void CTagItemModel::tagMgr_inserted(CTagItem */*parent*/,
        int /*first*/, int /*last*/)
{
    endInsertRows();
}

void CTagItemModel::tagMgr_aboutToBeRemoved(CTagItem *parent,
        int first, int last)
{
    if (parent == GTagMgr()->rootItem())
        beginRemoveRows(QModelIndex(), first, last);
    else
        beginRemoveRows(createIndex(parent->index(), 0, parent), first, last);
}

void CTagItemModel::tagMgr_removed(CTagItem */*parent*/,
        int /*first*/, int /*last*/)
{
    endRemoveRows();
}

void CTagItemModel::tagMgr_aboutToBeMoved(CTagItem *srcParent,
        int srcFirst, int srcLast, CTagItem *dstParent, int dstIndex)
{
    QModelIndex si = QModelIndex();
    QModelIndex di = QModelIndex();
    if (srcParent != GTagMgr()->rootItem())
        si = createIndex(srcParent->index(), 0, srcParent);
    if (dstParent != GTagMgr()->rootItem())
        di = createIndex(dstParent->index(), 0, dstParent);
    beginMoveRows(si, srcFirst, srcLast, di, dstIndex);
}

void CTagItemModel::tagMgr_moved(CTagItem */*srcParent*/,
        int /*srcFirst*/, int /*srcLast*/,
        CTagItem */*dstParent*/, int /*dstIndex*/)
{
    endMoveRows();
}

void CTagItemModel::tagMgr_dataChanged(CTagItem *item)
{
    int index = item->index();
    emit dataChanged(createIndex(index, 0, item),
                     createIndex(index,  columnCount()-1, item));
}
