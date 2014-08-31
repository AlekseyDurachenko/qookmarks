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
#include "ctagitemmodel.h"
#include <QDebug>

CTagItemModel::CTagItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = CTagItem::create("/", this);
    connect(m_rootItem, SIGNAL(rowInserted(CTagItem *,int,int)),
            this, SLOT(slot_rowInserted(CTagItem *,int,int)));
    connect(m_rootItem, SIGNAL(rowRemoved(CTagItem *,int,int)),
            this, SLOT(slot_rowRemoved(CTagItem *,int,int)));
    connect(m_rootItem, SIGNAL(dataChanged(CTagItem *,int,int)),
            this, SLOT(slot_dataChanged(CTagItem *,int,int)));
}

QVariant CTagItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootItem)
    {
        return QVariant();
    }

    return static_cast<CTagItem *>
            (index.internalPointer())->data(index.column(), role);
}

Qt::ItemFlags CTagItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant CTagItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (!m_rootItem)
    {
        return QVariant();
    }

    return m_rootItem->headerData(section, orientation, role);
}

QModelIndex CTagItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_rootItem)
    {
        return QModelIndex();
    }

    CTagItem *parentItem = m_rootItem;
    if (parent.isValid())
    {
        parentItem = static_cast<CTagItem *>(parent.internalPointer());
    }

    return createIndex(row, column, parentItem->child(row));
}

QModelIndex CTagItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || !m_rootItem)
    {
        return QModelIndex();
    }

    CTagItem *childItem = static_cast<CTagItem*>(index.internalPointer());
    CTagItem *parentItem = static_cast<CTagItem*>(childItem->parent());

    if (parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int CTagItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || !m_rootItem)
    {
        return 0;
    }

    CTagItem *parentItem = m_rootItem;
    if (parent.isValid())
    {
        parentItem = static_cast<CTagItem *>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int CTagItemModel::columnCount(const QModelIndex &parent) const
{
    if (!m_rootItem)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        return m_rootItem->columnCount();
    }

    return static_cast<CTagItem *>(parent.internalPointer())->columnCount();
}

void CTagItemModel::slot_rowInserted(CTagItem *parent, int first, int last)
{
    if (parent == m_rootItem)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else
    {
        beginInsertRows(createIndex(parent->row(), 0, parent), first, last);
    }
    endInsertRows();
}

void CTagItemModel::slot_rowRemoved(CTagItem *parent, int first, int last)
{
    if (parent == m_rootItem)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else
    {
        beginRemoveRows(createIndex(parent->row(), 0, parent), first, last);
    }
    endRemoveRows();
}

void CTagItemModel::slot_dataChanged(CTagItem *parent, int first, int last)
{
    emit dataChanged(createIndex(first, 0, parent->parent()),
            createIndex(last, parent->columnCount(), parent->parent()));
}
