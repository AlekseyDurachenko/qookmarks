// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "cnavanchoritemmodel.h"
#include <QMimeData>
#include <QStringList>
#include <QIcon>
#include <typeinfo>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "inavigationactions.h"
#include "cprj.h"
#include "singleton.h"


CNavAnchorItemModel::CNavAnchorItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_navigationActions = 0;
    initAnchorItems();

    connect(GBookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(bookmarkMgr_inserted()));
    connect(GBookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(bookmarkMgr_removed()));
    connect(GBookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*)),
            this, SLOT(bookmarkMgr_dataChanged()));
    connect(GBookmarkMgr(), SIGNAL(tagsChanged(CBookmarkItem*)),
            this, SLOT(bookmarkMgr_tagsChanged()));

    beginResetModel();
    endResetModel();
}

void CNavAnchorItemModel::setNavigationActions(
        INavigationActions *navigationActionInterface)
{
    QObject *obj = dynamic_cast<QObject *>(m_navigationActions);
    if (obj)
        disconnect(obj, 0, this, 0);

    obj = dynamic_cast<QObject *>(navigationActionInterface);
    if (obj)
        connect(obj, SIGNAL(destroyed()),
                this, SLOT(navigationActions_destroyed()));

    m_navigationActions = navigationActionInterface;
}

QVariant CNavAnchorItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return anchorName(m_anchorItems.at(index.row()));

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return anchorIcon(m_anchorItems.at(index.row()));

    if (role == Qt::UserRole)
        if (index.column() == 0)
            return m_anchorItems.at(index.row());

    return QVariant();
}

Qt::ItemFlags CNavAnchorItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled|Qt::ItemIsSelectable;
    if (index.internalId() != All
            && index.internalId() != Rated)
        flags |= Qt::ItemIsDropEnabled;

    return flags;
}

QStringList CNavAnchorItemModel::mimeTypes() const
{
    return QStringList() << "qookmarks/tag-list"
                         << "qookmarks/bookmark-list";
}

bool CNavAnchorItemModel::dropMimeData(const QMimeData *data,
        Qt::DropAction /*action*/, int /*row*/, int /*column*/,
        const QModelIndex &parent)
{
    if (m_navigationActions)
    {
        QList<QUrl> bookmarkList;
        if (data->hasFormat("qookmarks/bookmark-list"))
        {
            bookmarkList = fromMimeBookmarkList(data);
        }
        else if (data->hasFormat("qookmarks/tag-list"))
        {
            foreach (const QStringList &path, fromMimeTagList(data))
                foreach (CBookmarkItem *bookmarkItem,
                         GTagMgr()->findByPath(path)->bookmarks())
                    bookmarkList.push_back(bookmarkItem->data().url());
        }

        AnchorType type = m_anchorItems.at(parent.row());
        switch (type)
        {
        case Favorites:
            m_navigationActions->bookmarksMarkFavorite(bookmarkList);
            break;
        case ReadLater:
            m_navigationActions->bookmarksMarkReadLater(bookmarkList);
            break;
        case Trash:
            m_navigationActions->bookmarksMarkTrash(bookmarkList);
            break;
        default:
            ;
        }
    }

    return false;
}

QVariant CNavAnchorItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal
            && role == Qt::DisplayRole
            && section == 0)
        return tr("Name");

    return QVariant();
}

QModelIndex CNavAnchorItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column, m_anchorItems.at(row));
}

QModelIndex CNavAnchorItemModel::parent(const QModelIndex &/*index*/) const
{
    return QModelIndex();
}

int CNavAnchorItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_anchorItems.count();
}

int CNavAnchorItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

void CNavAnchorItemModel::bookmarkMgr_dataChanged()
{
    emit dataChanged(createIndex(0, 0),
                     createIndex(m_anchorItems.count()-1, 0));
}

void CNavAnchorItemModel::bookmarkMgr_inserted()
{
    emit dataChanged(createIndex(0, 0),
                     createIndex(m_anchorItems.count()-1, 0));
}

void CNavAnchorItemModel::bookmarkMgr_removed()
{
    emit dataChanged(createIndex(0, 0),
                     createIndex(m_anchorItems.count()-1, 0));
}

void CNavAnchorItemModel::bookmarkMgr_tagsChanged()
{
    emit dataChanged(createIndex(0, 0),
                     createIndex(m_anchorItems.count()-1, 0));
}

void CNavAnchorItemModel::navigationActions_destroyed()
{
    m_navigationActions = 0;
}

void CNavAnchorItemModel::initAnchorItems()
{
    m_anchorItems
            << All
            << Favorites
            << Rated
            << ReadLater
            << Untagged
            << Trash;
}

QString CNavAnchorItemModel::anchorName(CNavAnchorItemModel::AnchorType type) const
{
    switch (type)
    {
    case All:
        return tr("Bookmarks (%1)").arg(GBookmarkMgr()->count());
    case Favorites:
        return tr("Favorites (%1)").arg(GBookmarkMgr()->favoriteCount());
    case Rated:
        return tr("Rated Bookmarks (%1)").arg(GBookmarkMgr()->ratedCount());
    case ReadLater:
        return tr("Read it Later (%1)").arg(GBookmarkMgr()->readItLaterCount());
    case Untagged:
        return tr("Untagged (%1)").arg(GBookmarkMgr()->untaggedCount());
    case Trash:
        return tr("Trash (%1)").arg(GBookmarkMgr()->trashCount());
    }

    return QString();
}

QIcon CNavAnchorItemModel::anchorIcon(CNavAnchorItemModel::AnchorType type) const
{
    switch (type)
    {
    case All:
        return QIcon(":/icons/bookmark-bookmarks.png");
    case Favorites:
        return QIcon(":/icons/bookmark-favorites.png");
    case Rated:
        return QIcon(":/icons/bookmark-rated.png");
    case ReadLater:
        return QIcon(":/icons/bookmark-readlater.png");
    case Untagged:
        return QIcon(":/icons/bookmark-untagged.png");
    case Trash:
        return QIcon(":/icons/bookmark-trash.png");
    }

    return QIcon();
}

QList<QStringList> CNavAnchorItemModel::fromMimeTagList(const QMimeData *data)
{
    QByteArray encodedData = data->data("qookmarks/tag-list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QStringList> tagPaths;
    stream >> tagPaths;

    return tagPaths;
}

QList<QUrl> CNavAnchorItemModel::fromMimeBookmarkList(const QMimeData *data)
{
    QByteArray encodedData = data->data("qookmarks/bookmark-list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QUrl> bookmarkUrls;
    stream >> bookmarkUrls;

    return bookmarkUrls;
}
