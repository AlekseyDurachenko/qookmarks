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
#include "cbookmarkitemmodel.h"
#include "cabstractbookmarkdatamodel.h"
#include "cbookmarkitem.h"
#include "cprj.h"
#include "ciconmgr.h"
#include "singleton.h"
#include "consts.h"
#include <QIcon>
#include <QMimeData>
#include <QDebug>


static QString joinSet(const QSet<QString> &set, const QString &sep)
{
    QStringList list = set.toList();
    list.sort();

    QString result;
    for (int i = 0; i < list.count()-1; ++i)
        result += list.at(i) + sep;
    if (!list.isEmpty())
        result += list.last();

    return result;
}

CBookmarkItemModel::CBookmarkItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_dataModel = 0;
}

CBookmarkItemModel::CBookmarkItemModel(
        CAbstractBookmarkDataModel *dataModel, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_dataModel = 0;
    setDataModel(dataModel);
}

void CBookmarkItemModel::setDataModel(CAbstractBookmarkDataModel *dataModel)
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

QVariant CBookmarkItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_dataModel)
        return QVariant();

    CBookmarkItem *bookmark = m_dataModel->at(index.row());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    {
        switch (index.column())
        {
        case 0:
            return QString(bookmark->data().title()).replace("\n", " ");
        case 1:
            return bookmark->data().url().toString().replace("\n", " ");
        case 2:
            return QString(bookmark->data().description()).replace("\n", " ");
        case 3:
            return joinSet(bookmark->data().keywords(), ",").replace("\n", " ");
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
            return QString(bookmark->data().httpReasonPhrase()).replace("\n", " ");;
        case 10:
            return bookmark->data().httpCheckDateTime().toString(gDateTimeFormat);
        case 11:
            return bookmark->data().createdDateTime().toString(gDateTimeFormat);
        case 12:
            return bookmark->data().editedDateTime().toString(gDateTimeFormat);
        case 13:
            return bookmark->data().lastVisitedDateTime().toString(gDateTimeFormat);
        case 14:
            return bookmark->data().visitCount();
        case 15:
            return QString(bookmark->data().notes()).replace("\n", " ");
        }
    }

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return GIconMgr()->icon(bookmark->data().url(), QIcon(":/icons/bookmark-item.png"));

    if (role == Qt::UserRole)
        return QVariant::fromValue((void *)m_dataModel->at(index.row()));

    return QVariant();
}

Qt::ItemFlags CBookmarkItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags f = Qt::ItemIsEnabled
            |Qt::ItemIsSelectable
            |Qt::ItemIsDragEnabled;

    return f;
}

QMimeData *CBookmarkItemModel::mimeData(const QModelIndexList &indexes) const
{
    QList<QUrl> bookmarkUrls;
    foreach (QModelIndex index, indexes)
        if (index.isValid() && index.column() == 0)
            bookmarkUrls << static_cast<CBookmarkItem *>(index.internalPointer())->data().url();

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << bookmarkUrls;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("qookmarks/bookmark-list", encodedData);
    return mimeData;
}

QVariant CBookmarkItemModel::headerData(int section,
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
            return QIcon(":/icons/bookmark-favorites.png");
        case 6:
            return QIcon(":/icons/bookmark-readlater.png");
        case 7:
            return QIcon(":/icons/bookmark-trash.png");
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

QModelIndex CBookmarkItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_dataModel)
        return QModelIndex();

    return createIndex(row, column, m_dataModel->at(row));
}

QModelIndex CBookmarkItemModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int CBookmarkItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_dataModel)
        return 0;

    return m_dataModel->count();
}

int CBookmarkItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 16;
}

void CBookmarkItemModel::dataModel_aboutToBeInserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
}

void CBookmarkItemModel::dataModel_inserted(int /*first*/, int /*last*/)
{
    endInsertRows();
}

void CBookmarkItemModel::dataModel_aboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void CBookmarkItemModel::dataModel_removed(int /*first*/, int /*last*/)
{
    endRemoveRows();
}

void CBookmarkItemModel::dataModel_dataChanged(int first, int last)
{
    emit dataChanged(createIndex(first, 0),
                     createIndex(last, columnCount()-1));
}

void CBookmarkItemModel::dataModel_reseted()
{
    beginResetModel();
    endResetModel();
}

void CBookmarkItemModel::dataModel_destroyed()
{
    m_dataModel = 0;
    beginResetModel();
    endResetModel();
}