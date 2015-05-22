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
#include "bookmarkitemmodel.h"
#include "cabstractbookmarkdatamodel.h"
#include "cbookmarkitem.h"
#include "ciconmgr.h"
#include "consts.h"
#include "prj.h"
#include "icontheme.h"
#include "singleton.h"
#include <QIcon>
#include <QMimeData>
#include <QDebug>


BookmarkItemModel::BookmarkItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_dataModel = 0;
}

void BookmarkItemModel::setDataModel(CAbstractBookmarkDataModel *dataModel)
{
    if (m_dataModel)
        disconnect(m_dataModel, 0, this, 0);

    m_dataModel = dataModel;
    if (m_dataModel)
    {
        connect(m_dataModel, SIGNAL(aboutToBeInserted(int,int)),
                this, SLOT(dataModel_aboutToBeInserted(int,int)));
        connect(m_dataModel, SIGNAL(inserted(int,int)),
                this, SLOT(dataModel_inserted(int,int)));
        connect(m_dataModel, SIGNAL(aboutToBeRemoved(int,int)),
                this, SLOT(dataModel_aboutToBeRemoved(int,int)));
        connect(m_dataModel, SIGNAL(removed(int,int)),
                this, SLOT(dataModel_removed(int,int)));
        connect(m_dataModel, SIGNAL(reseted()),
                this, SLOT(dataModel_reseted()));
        connect(m_dataModel, SIGNAL(dataChanged(int,int)),
                this, SLOT(dataModel_dataChanged(int,int)));
        connect(m_dataModel, SIGNAL(destroyed()),
                this, SLOT(dataModel_destroyed()));
    }

    beginResetModel();
    endResetModel();
}

QVariant BookmarkItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_dataModel)
        return QVariant();

    CBookmarkItem *bookmark = m_dataModel->at(index.row());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        switch (index.column())
        {
        case 0:
            return QString(bookmark->data().title()).replace("\n", " ").replace("\r", " ");
        case 1:
            return bookmark->data().url().toString().replace("\n", " ").replace("\r", " ");
        case 2:
            return QString(bookmark->data().description()).replace("\n", " ").replace("\r", " ");
        case 3:
            return QString(bookmark->data().keywords()).replace("\n", " ").replace("\r", " ");
        case 4:
            return bookmark->data().rating();
        case 5:
            return bookmark->data().isFavorite();
        case 6:
            return bookmark->data().isReadItLater();
        case 7:
            return bookmark->data().isTrash();
        case 8:
            return bookmark->data().httpStatusCode();
        case 9:
            return QString(bookmark->data().httpReasonPhrase()).replace("\n", " ").replace("\r", " ");
        case 10:
            return bookmark->data().httpCheckDateTime().toString(Bookmark::DateTimeFormat);
        case 11:
            return bookmark->data().createdDateTime().toString(Bookmark::DateTimeFormat);
        case 12:
            return bookmark->data().editedDateTime().toString(Bookmark::DateTimeFormat);
        case 13:
            return bookmark->data().lastVisitedDateTime().toString(Bookmark::DateTimeFormat);
        case 14:
            return bookmark->data().visitCount();
        case 15:
            return QString(bookmark->data().notes()).replace("\n", " ").replace("\r", " ");
        }
    }

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return GIconMgr()->icon(bookmark->data().url(),
                                    IconTheme::icon("favicon-default"));

    if (role == Qt::UserRole)
        return CBookmarkItem::variantFromPtr(m_dataModel->at(index.row()));

    return QVariant();
}

Qt::ItemFlags BookmarkItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags f = Qt::ItemIsEnabled
            |Qt::ItemIsSelectable
            |Qt::ItemIsDragEnabled;

    return f;
}

QMimeData *BookmarkItemModel::mimeData(const QModelIndexList &indexes) const
{
    QList<QUrl> bookmarkUrls;
    foreach (QModelIndex index, indexes)
        if (index.isValid() && index.column() == 0)
            bookmarkUrls << CBookmarkItem::variantToPtr(index.data(Qt::UserRole))->data().url();

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << bookmarkUrls;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("qookmarks/bookmark-list", encodedData);
    return mimeData;
}

QVariant BookmarkItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::TextAlignmentRole)
    {
        return Qt::AlignLeft;
    }

    if (orientation == Qt::Horizontal && role == Qt::DecorationRole)
    {
        switch (section)
        {
        case 5:
            return IconTheme::icon("anchor-bookmark-favorite");
        case 6:
            return IconTheme::icon("anchor-bookmark-readitlater");
        case 7:
            return IconTheme::icon("anchor-bookmark-trash");
        default:
            return QVariant();
        }
    }

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
            return tr("Keywords");
        case 4:
            return tr("Rating");
        case 5:
            return QVariant();
        case 6:
            return QVariant();
        case 7:
            return QVariant();
        case 8:
            return tr("Http status code");
        case 9:
            return tr("Http response phase");
        case 10:
            return tr("Http check datetime");
        case 11:
            return tr("Created datetime");
        case 12:
            return tr("Edited datetime");
        case 13:
            return tr("Last Visited datetime");
        case 14:
            return tr("Visit count");
        case 15:
            return tr("Note");
        }
    }

    return QVariant();
}

QModelIndex BookmarkItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_dataModel)
        return QModelIndex();

    return createIndex(row, column, m_dataModel->at(row));
}

QModelIndex BookmarkItemModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int BookmarkItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_dataModel)
        return 0;

    return m_dataModel->count();
}

int BookmarkItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 16;
}

Qt::DropActions BookmarkItemModel::supportedDropActions() const
{
    return Qt::CopyAction|Qt::MoveAction;
}

void BookmarkItemModel::dataModel_aboutToBeInserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
}

void BookmarkItemModel::dataModel_inserted(int /*first*/, int /*last*/)
{
    endInsertRows();
}

void BookmarkItemModel::dataModel_aboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void BookmarkItemModel::dataModel_removed(int /*first*/, int /*last*/)
{
    endRemoveRows();
}

void BookmarkItemModel::dataModel_dataChanged(int first, int last)
{
    emit dataChanged(createIndex(first, 0),
                     createIndex(last, columnCount()-1));
}

void BookmarkItemModel::dataModel_reseted()
{
    beginResetModel();
    endResetModel();
}

void BookmarkItemModel::dataModel_destroyed()
{
    m_dataModel = 0;
    beginResetModel();
    endResetModel();
}
