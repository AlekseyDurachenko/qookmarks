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
#include "cbookmarkitemmodel.h"
#include "cbookmarkmgr.h"
#include <QIcon>
#include <QDebug>

CBookmarkItemModel::CBookmarkItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_mgr = 0;
}

CBookmarkItemModel::CBookmarkItemModel(CBookmarkMgr *mgr, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_mgr = 0;
    setBookmarkMgr(mgr);
}

void CBookmarkItemModel::setBookmarkMgr(CBookmarkMgr *mgr)
{
    beginResetModel();

    if (m_mgr)
        disconnect(m_mgr, 0, this, 0);

    m_mgr = mgr;
    if (m_mgr)
    {
        connect(m_mgr, SIGNAL(bookmarkInserted(int,int)),
                this, SLOT(onBookmarkInserted(int,int)));
        connect(m_mgr, SIGNAL(bookmarkRemoved(int,int)),
                this, SLOT(onBookmarkRemoved(int,int)));
        connect(m_mgr, SIGNAL(bookmarkDataChanged(int,int)),
                this, SLOT(onBookmarkDataChanged(int,int)));
    }

    endResetModel();
}

QVariant CBookmarkItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_mgr)
        return QVariant();

    CBookmarkItem *bookmark = m_mgr->bookmarkAt(index.row());
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return bookmark->data().title();
        case 1:
            return bookmark->data().url();
        }
    }

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return QIcon(":/icons/bookmark-item.png");

    return QVariant();
}

Qt::ItemFlags CBookmarkItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant CBookmarkItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return tr("Title");
        case 1:
            return tr("Url");
        }
    }

    return QVariant();
}

QModelIndex CBookmarkItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_mgr)
        return QModelIndex();

    return createIndex(row, column, m_mgr->bookmarkAt(row));
}

QModelIndex CBookmarkItemModel::parent(const QModelIndex &/*index*/) const
{
    return QModelIndex();
}

int CBookmarkItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_mgr)
        return 0;

    return m_mgr->bookmarkCount();
}

int CBookmarkItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

void CBookmarkItemModel::onBookmarkInserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
    endInsertRows();
}

void CBookmarkItemModel::onBookmarkRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
    endRemoveRows();
}

void CBookmarkItemModel::onBookmarkDataChanged(int first, int last)
{
    emit dataChanged(createIndex(first, 0, m_mgr->bookmarkAt(first)),
                     createIndex(last,  2, m_mgr->bookmarkAt(last)));
}
