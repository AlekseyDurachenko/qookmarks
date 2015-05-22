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
#include "tagitemmodel.h"
#include "icontheme.h"
#include "singleton.h"


TagItemModel::TagItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    connect(GTagMgr(), SIGNAL(aboutToBeInserted(TagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeInserted(TagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(inserted(TagItem*,int,int)),
            this, SLOT(tagMgr_inserted(TagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(aboutToBeRemoved(TagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeRemoved(TagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(removed(TagItem*,int,int)),
            this, SLOT(tagMgr_removed(TagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(aboutToBeMoved(TagItem*,int,int,TagItem*,int)),
            this, SLOT(tagMgr_aboutToBeMoved(TagItem*,int,int,TagItem*,int)));
    connect(GTagMgr(), SIGNAL(moved(TagItem*,int,int,TagItem*,int)),
            this, SLOT(tagMgr_moved(TagItem*,int,int,TagItem*,int)));
    connect(GTagMgr(), SIGNAL(dataChanged(TagItem*)),
            this, SLOT(tagMgr_dataChanged(TagItem*)));
}

QVariant TagItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TagItem *item = static_cast<TagItem *>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return QString("%1").arg(item->data().name());

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return IconTheme::icon("tag-icon-default");

    if (role == Qt::UserRole)
        return TagItem::variantFromPtr(item);

    return QVariant();
}

Qt::ItemFlags TagItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

   return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant TagItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal
            && role == Qt::DisplayRole
            && section == 0)
        return tr("Name");

    return QVariant();
}

QModelIndex TagItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TagItem *parentItem;
    if (!parent.isValid())
        parentItem = GTagMgr()->rootItem();
    else
        parentItem = static_cast<TagItem *>(parent.internalPointer());

    return createIndex(row, column, parentItem->at(row));
}

QModelIndex TagItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TagItem *childItem = static_cast<TagItem *>(index.internalPointer());
    TagItem *parentItem = childItem->parent();

    if (parentItem == GTagMgr()->rootItem())
        return QModelIndex();

    return createIndex(parentItem->index(), 0, parentItem);
}

int TagItemModel::rowCount(const QModelIndex &parent) const
{
     if (parent.column() > 0)
         return 0;

     TagItem *parentItem;
     if (!parent.isValid())
         parentItem = GTagMgr()->rootItem();
     else
         parentItem = static_cast<TagItem *>(parent.internalPointer());

     return parentItem->count();
}

int TagItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

void TagItemModel::tagMgr_aboutToBeInserted(TagItem *parent,
        int first, int last)
{
    if (parent == GTagMgr()->rootItem())
        beginInsertRows(QModelIndex(), first, last);
    else
        beginInsertRows(createIndex(parent->index(), 0, parent), first, last);
}

void TagItemModel::tagMgr_inserted(TagItem */*parent*/,
        int /*first*/, int /*last*/)
{
    endInsertRows();
}

void TagItemModel::tagMgr_aboutToBeRemoved(TagItem *parent,
        int first, int last)
{
    if (parent == GTagMgr()->rootItem())
        beginRemoveRows(QModelIndex(), first, last);
    else
        beginRemoveRows(createIndex(parent->index(), 0, parent), first, last);
}

void TagItemModel::tagMgr_removed(TagItem */*parent*/,
        int /*first*/, int /*last*/)
{
    endRemoveRows();
}

void TagItemModel::tagMgr_aboutToBeMoved(TagItem *srcParent,
        int srcFirst, int srcLast, TagItem *dstParent, int dstIndex)
{
    QModelIndex si = QModelIndex();
    QModelIndex di = QModelIndex();
    if (srcParent != GTagMgr()->rootItem())
        si = createIndex(srcParent->index(), 0, srcParent);
    if (dstParent != GTagMgr()->rootItem())
        di = createIndex(dstParent->index(), 0, dstParent);
    beginMoveRows(si, srcFirst, srcLast, di, dstIndex);
}

void TagItemModel::tagMgr_moved(TagItem */*srcParent*/,
        int /*srcFirst*/, int /*srcLast*/,
        TagItem */*dstParent*/, int /*dstIndex*/)
{
    endMoveRows();
}

void TagItemModel::tagMgr_dataChanged(TagItem *item)
{
    int index = item->index();
    emit dataChanged(createIndex(index, 0, item),
                     createIndex(index,  columnCount()-1, item));
}
