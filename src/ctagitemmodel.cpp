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
#include "cbookmarkmgr.h"
#include <QIcon>
#include <QDebug>


CTagItemModel::CTagItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_mgr = 0;
}

CTagItemModel::CTagItemModel(CBookmarkMgr *mgr, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_mgr = 0;
    setBookmarkMgr(mgr);
}

void CTagItemModel::setBookmarkMgr(CBookmarkMgr *mgr)
{
    beginResetModel();

    if (m_mgr)
        disconnect(m_mgr, 0, this, 0);

    m_mgr = mgr;
    if (m_mgr)
    {
        connect(m_mgr, SIGNAL(tagInserted(CTagItem *,int,int)),
                this, SLOT(onTagInserted(CTagItem *,int,int)));
        connect(m_mgr, SIGNAL(tagRemoved(CTagItem *,int,int)),
                this, SLOT(onTagRemoved(CTagItem *,int,int)));
        connect(m_mgr, SIGNAL(tagDataChanged(CTagItem *,int,int)),
                this, SLOT(onTagDataChanged(CTagItem *,int,int)));
    }

    endResetModel();
}

QVariant CTagItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_mgr)
        return QVariant();

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            if (item->parent() && item->parent()->id() == -1)
            {
                switch(item->type())
                {
                case CTagItem::RootItem:
                    return QObject::tr("/");
                case CTagItem::Tag:
                    return QObject::tr("Tags");
                case CTagItem::ReadLater:
                    return QObject::tr("Read it later");
                case CTagItem::Favorites:
                    return QObject::tr("Favorites");
                }
            }

            return item->data().title();
        }
    }

    if (role == Qt::DecorationRole)
    {
        if (index.column() == 0)
        {
            switch(item->type())
            {
            case CTagItem::Tag:
                return QIcon(":/icons/bookmark-tag.png");
            case CTagItem::ReadLater:
                return QIcon(":/icons/bookmark-readlater.png");
            case CTagItem::Favorites:
                return QIcon(":/icons/bookmark-favorites.png");
            default:
                return QVariant();
            }
        }
    }

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
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return QObject::tr("Title");
        }
    }

    return QVariant();
}

QModelIndex CTagItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_mgr)
        return QModelIndex();

    CTagItem *parentItem = m_mgr->tagRootItem();
    if (parent.isValid())
        parentItem = static_cast<CTagItem *>(parent.internalPointer());

    return createIndex(row, column, parentItem->childAt(row));
}

QModelIndex CTagItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || !m_mgr)
        return QModelIndex();

    CTagItem *childItem = static_cast<CTagItem*>(index.internalPointer());
    CTagItem *parentItem = static_cast<CTagItem*>(childItem->parent());

    if (parentItem == m_mgr->tagRootItem())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CTagItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || !m_mgr)
        return 0;

    CTagItem *parentItem = m_mgr->tagRootItem();
    if (parent.isValid())
        parentItem = static_cast<CTagItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int CTagItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

void CTagItemModel::onTagInserted(CTagItem *parent, int first, int last)
{
    if (parent == m_mgr->tagRootItem())
        beginInsertRows(QModelIndex(), first, last);
    else
        beginInsertRows(createIndex(parent->row(), 0, parent), first, last);
    endInsertRows();
}

void CTagItemModel::onTagRemoved(CTagItem *parent, int first, int last)
{
    if (parent == m_mgr->tagRootItem())
        beginRemoveRows(QModelIndex(), first, last);
    else
        beginRemoveRows(createIndex(parent->row(), 0, parent), first, last);
    endRemoveRows();
}

void CTagItemModel::onTagDataChanged(CTagItem *parent, int first, int last)
{
    emit dataChanged(createIndex(first, 0, parent->parent()),
                     createIndex(last,  1, parent->parent()));
}