// Copyright 20142015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "cbookmarkfilteritemmodel.h"
#include "cbookmarkfilterdatamodel.h"
#include "cbookmarkitem.h"
#include "cprj.h"
#include "ciconmgr.h"
#include "singleton.h"
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

CBookmarkFilteredItemModel::CBookmarkFilteredItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_dataModel = 0;
}

CBookmarkFilteredItemModel::CBookmarkFilteredItemModel(CBookmarkFilterDataModel *dataModel, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_dataModel = 0;
    setDataModel(dataModel);
}

void CBookmarkFilteredItemModel::setDataModel(CBookmarkFilterDataModel *dataModel)
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

    reset();
}

QVariant CBookmarkFilteredItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_dataModel)
        return QVariant();

    CBookmarkItem *bookmark = m_dataModel->at(index.row());
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
            return bookmark->data().note();
        case 4:
            return joinSet(bookmark->data().keywords(), ",");
        case 5:
            return bookmark->data().isReadLater();
        case 6:
            return bookmark->data().isFavorite();
        case 7:
            return bookmark->data().isTrash();
        case 8:
            return bookmark->data().rating();
        case 9:
            return bookmark->data().createdDateTime();
        case 10:
            return bookmark->data().editedDateTime();
        case 11:
            return bookmark->data().lastVisitedDateTime();
        case 12:
            return bookmark->data().visitCount();
        case 13:
            return bookmark->data().httpResponseCode();
        case 14:
            return bookmark->data().httpResponseText();
        case 15:
            return bookmark->data().lastCheckDateTime();
        }
    }

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return singleton<CPrj>()->iconMgr()->icon(bookmark->data().url(), QIcon(":/icons/bookmark-item.png"));

    if (role == Qt::UserRole)
        return QVariant::fromValue((void *)m_dataModel->at(index.row()));

    return QVariant();
}

Qt::ItemFlags CBookmarkFilteredItemModel::flags(const QModelIndex &index) const
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

QMimeData *CBookmarkFilteredItemModel::mimeData(const QModelIndexList &indexes) const
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

QVariant CBookmarkFilteredItemModel::headerData(int section,
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
            return tr("Note");
        case 4:
            return tr("Keywords");
        case 5:
            //return QVariant();
            return tr("Read Later");
        case 6:
            //return QVariant();
            return tr("Favorite");
        case 7:
            //return QVariant();
            return tr("Trash");
        case 8:
            return tr("Rating");
        case 9:
            return tr("Created");
        case 10:
            return tr("Edited");
        case 11:
            return tr("Last Visited");
        case 12:
            return tr("Visit count");
        case 13:
            return tr("Http code");
        case 14:
            return tr("Http text");
        case 15:
            return tr("Check date");
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

QModelIndex CBookmarkFilteredItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_dataModel)
        return QModelIndex();

    return createIndex(row, column, m_dataModel->at(row));
}

QModelIndex CBookmarkFilteredItemModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int CBookmarkFilteredItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_dataModel)
        return 0;

    return m_dataModel->count();
}

int CBookmarkFilteredItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 16;
}

void CBookmarkFilteredItemModel::dataModel_aboutToBeInserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
}

void CBookmarkFilteredItemModel::dataModel_inserted(int first, int last)
{
    Q_UNUSED(first);
    Q_UNUSED(last);

    endInsertRows();
}

void CBookmarkFilteredItemModel::dataModel_aboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void CBookmarkFilteredItemModel::dataModel_removed(int first, int last)
{
    Q_UNUSED(first);
    Q_UNUSED(last);

    endRemoveRows();
}

void CBookmarkFilteredItemModel::dataModel_dataChanged(int first, int last)
{
    emit dataChanged(createIndex(first, 0),
                     createIndex(last, columnCount()-1));
}

void CBookmarkFilteredItemModel::dataModel_reseted()
{
    reset();
}

void CBookmarkFilteredItemModel::dataModel_destroyed()
{
    m_dataModel = 0;
    reset();
}
