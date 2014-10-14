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
#include <QMimeData>
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
                this, SLOT(slot_mgr_bookmarkInserted(int,int)));
        connect(m_mgr, SIGNAL(bookmarkRemoved(int,int)),
                this, SLOT(slot_mgr_bookmarkRemoved(int,int)));
        connect(m_mgr, SIGNAL(bookmarkDataChanged(int,int)),
                this, SLOT(slot_mgr_bookmarkDataChanged(int,int)));
    }

    endResetModel();
}

QVariant CBookmarkItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_mgr)
        return QVariant();

    CBookmarkItem *bookmark = m_mgr->bookmarkAt(index.row());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        switch (index.column())
        {
        case 0:
            return bookmark->data().title();
        case 1:
            return bookmark->data().url();
        case 2:
            return bookmark->data().description();
        case 3:
            return bookmark->data().comment();
        case 4:
            return bookmark->data().keywordList();
        case 5:
            return bookmark->data().isReadLater();
        case 6:
            return bookmark->data().isFavorite();
        case 7:
            return bookmark->data().rating();
        }
    }

    if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            if (bookmark->data().favicon().isNull())
                return QIcon(":/icons/bookmark-item.png");
            return bookmark->data().favicon();
        }
    }

    if (role == Qt::UserRole)
        return QVariant::fromValue((void *)m_mgr->bookmarkAt(index.row()));

    return QVariant();
}

Qt::ItemFlags CBookmarkItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags f = Qt::ItemIsEnabled
            |Qt::ItemIsSelectable
            |Qt::ItemIsDragEnabled;

//    if (index.column() == 5)
//        f |= Qt::ItemIsEditable;

    return f;
}

QStringList CBookmarkItemModel::mimeTypes() const
{
    return QStringList() << "application/CBookmarkItemList";
}

QMimeData *CBookmarkItemModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes)
        if (index.isValid() && index.column() == 0)
            stream << static_cast<CBookmarkItem *>(index.internalPointer())->data().url();

    mimeData->setData("application/CBookmarkItemList", encodedData);
    return mimeData;
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
        case 2:
            return tr("Description");
        case 3:
            return tr("Comment");
        case 4:
            return tr("Keywords");
        case 5:
            return QVariant();
            //return tr("Read Later");
        case 6:
            return QVariant();
            //return tr("Favorite");
        case 7:
            return tr("Rating");
        }
    }

//    if (orientation == Qt::Horizontal && role == Qt::DecorationRole)
//    {
//        switch (section)
//        {
//        case 5:
//            return QIcon(":/icons/bookmark-readlater.png");
//        default:
//            return QVariant();
//        }
//    }

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
    return 8;
}

void CBookmarkItemModel::slot_mgr_bookmarkInserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
    endInsertRows();
}

void CBookmarkItemModel::slot_mgr_bookmarkRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
    endRemoveRows();
}

void CBookmarkItemModel::slot_mgr_bookmarkDataChanged(int first, int last)
{
    emit dataChanged(createIndex(first, 0),
                     createIndex(last, columnCount()-1));
}

void CBookmarkItemModel::slot_mgr_destroyed()
{
    m_mgr = 0;
}
