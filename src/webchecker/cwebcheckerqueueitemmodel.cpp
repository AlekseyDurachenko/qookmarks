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
#include "cwebcheckerqueueitemmodel.h"


CWebCheckerQueueItemModel::CWebCheckerQueueItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_mgr = 0;
}

CWebCheckerQueueItemModel::CWebCheckerQueueItemModel(CWebCheckerQueueMgr *mgr,
        QObject *parent) : QAbstractItemModel(parent)
{
    m_mgr = 0;
    setMgr(mgr);
}

void CWebCheckerQueueItemModel::setMgr(CWebCheckerQueueMgr *mgr)
{
    beginResetModel();

    if (m_mgr)
        disconnect(m_mgr, 0, this, 0);

    m_mgr = mgr;
    if (m_mgr)
    {
        connect(m_mgr, SIGNAL(itemInserted(int,int)),
                this, SLOT(slot_mgr_inserted(int,int)));
        connect(m_mgr, SIGNAL(itemRemoved(int,int)),
                this, SLOT(slot_mgr_removed(int,int)));
        connect(m_mgr, SIGNAL(itemDataChanged(int,int)),
                this, SLOT(slot_mgr_dataChanged(int,int)));
    }

    endResetModel();
}

QVariant CWebCheckerQueueItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_mgr)
        return QVariant();

    const CWebCheckerQueueItem &item = m_mgr->itemAt(index.row());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        switch (index.column())
        {
        case 0:
            return item.status();
        case 1:
            return item.bookmark()->data().url();
        }
    }

    return QVariant();
}

Qt::ItemFlags CWebCheckerQueueItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags f = Qt::ItemIsEnabled|Qt::ItemIsSelectable;

    return f;
}

QVariant CWebCheckerQueueItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return tr("Status");
        case 1:
            return tr("Url");
        }
    }

    return QVariant();
}

QModelIndex CWebCheckerQueueItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_mgr)
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex CWebCheckerQueueItemModel::parent(const QModelIndex &/*index*/) const
{
    return QModelIndex();
}

int CWebCheckerQueueItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_mgr)
        return 0;

    return m_mgr->count();
}

int CWebCheckerQueueItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

void CWebCheckerQueueItemModel::slot_mgr_inserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
    endInsertRows();
}

void CWebCheckerQueueItemModel::slot_mgr_removed(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
    endRemoveRows();
}

void CWebCheckerQueueItemModel::slot_mgr_dataChanged(int first, int last)
{
    emit dataChanged(createIndex(first, 0),
                     createIndex(last, columnCount()-1));
}

void CWebCheckerQueueItemModel::slot_mgr_destroyed()
{
    m_mgr = 0;
}
