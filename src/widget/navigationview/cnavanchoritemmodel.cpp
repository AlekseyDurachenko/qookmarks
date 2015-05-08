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
#include "ctagitem.h"
#include "cbookmarkitem.h"
#include "inavigationactions.h"
#include "singleton.h"


CNavAnchorItemModel::CNavAnchorItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_navigationActions = 0;

    connect(GBookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(updateAnchorItems()));
    connect(GBookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(updateAnchorItems()));
    connect(GBookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*)),
            this, SLOT(updateAnchorItems()));
    connect(GBookmarkMgr(), SIGNAL(tagsChanged(CBookmarkItem*)),
            this, SLOT(updateAnchorItems()));

    initAnchorItems();

    beginResetModel();
    endResetModel();
}

void CNavAnchorItemModel::setNavigationActions(INavigationActions *interface)
{
    QObject *obj = dynamic_cast<QObject *>(m_navigationActions);
    if (obj)
        disconnect(obj, 0, this, 0);

    obj = dynamic_cast<QObject *>(interface);
    if (obj)
        connect(obj, SIGNAL(destroyed()),
                this, SLOT(navigationActions_destroyed()));

    m_navigationActions = interface;
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
        case Untagged:
            m_navigationActions->bookmarksClearTags(bookmarkList);
        case Favorites:
            m_navigationActions->bookmarksMarkFavorite(bookmarkList);
            break;
        case ReadItLater:
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

void CNavAnchorItemModel::updateAnchorItems()
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
            << Untagged
            << Favorites
            << ReadItLater
            << Rated
            << Trash;
}

QString CNavAnchorItemModel::anchorName(CNavAnchorItemModel::AnchorType type) const
{
    switch (type)
    {
    case All:
        return tr("All (%1)").arg(GBookmarkMgr()->count());
    case Untagged:
        return tr("Untagged (%1)").arg(GBookmarkMgr()->untaggedCount());
    case Favorites:
        return tr("Favorites (%1)").arg(GBookmarkMgr()->favoriteCount());
    case ReadItLater:
        return tr("Read it Later (%1)").arg(GBookmarkMgr()->readItLaterCount());
    case Rated:
        return tr("Rated (%1)").arg(GBookmarkMgr()->ratedCount());
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
        return QIcon(":/icons/anchor-all.png");
    case Untagged:
        return QIcon(":/icons/anchor-untagged.png");
    case Favorites:
        return QIcon(":/icons/anchor-favorite.png");
    case ReadItLater:
        return QIcon(":/icons/anchor-readitlater.png");
    case Rated:
        return QIcon(":/icons/anchor-rated.png");
    case Trash:
        return QIcon(":/icons/anchor-trash.png");
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
