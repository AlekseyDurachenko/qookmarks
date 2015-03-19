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
#include "cnavigationitemmodel.h"
#include <QMimeData>
#include <QStringList>
#include <QIcon>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include <QDebug>


CNavigationItemModel::CNavigationItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_manager = 0;
    initTopLevelItems();
    initTopLevelCounters();
}

CNavigationItemModel::CNavigationItemModel(CManager *manager, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_manager = 0;
    initTopLevelItems();
    initTopLevelCounters();
    setManager(manager);
}

CNavigationItemModel::~CNavigationItemModel()
{
}

void CNavigationItemModel::setManager(CManager *manager)
{
    if (m_manager)
    {
        disconnect(m_manager->tagMgr(), 0, this, 0);
        disconnect(m_manager->bookmarkMgr(), 0, this, 0);
    }

    m_manager = manager;
    if (m_manager)
    {
        connect(m_manager->tagMgr(), SIGNAL(aboutToBeInserted(CTagItem*,int,int)),
                this, SLOT(tagMgr_aboutToBeInserted(CTagItem*,int,int)));
        connect(m_manager->tagMgr(), SIGNAL(inserted(CTagItem*,int,int)),
                this, SLOT(tagMgr_inserted(CTagItem*,int,int)));
        connect(m_manager->tagMgr(), SIGNAL(aboutToBeRemoved(CTagItem*,int,int)),
                this, SLOT(tagMgr_aboutToBeRemoved(CTagItem*,int,int)));
        connect(m_manager->tagMgr(), SIGNAL(removed(CTagItem*,int,int)),
                this, SLOT(tagMgr_removed(CTagItem*,int,int)));
        connect(m_manager->tagMgr(), SIGNAL(aboutToBeMoved(CTagItem*,int,int,CTagItem*,int)),
                this, SLOT(tagMgr_aboutToBeMoved(CTagItem*,int,int,CTagItem*,int)));
        connect(m_manager->tagMgr(), SIGNAL(moved(CTagItem*,int,int,CTagItem*,int)),
                this, SLOT(tagMgr_moved(CTagItem*,int,int,CTagItem*,int)));
        connect(m_manager->tagMgr(), SIGNAL(dataChanged(CTagItem*,CTag,CTag)),
                this, SLOT(tagMgr_dataChanged(CTagItem*)));
        connect(m_manager->tagMgr(), SIGNAL(bookmarksChanged(CTagItem*)),
                this, SLOT(tagMgr_bookmarksChanged(CTagItem*)));
        connect(m_manager->bookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*,CBookmark,CBookmark)),
                this, SLOT(bookmarkMgr_dataChanged(CBookmarkItem*,CBookmark, CBookmark)));
        connect(m_manager->bookmarkMgr(), SIGNAL(inserted(int,int)),
                this, SLOT(bookmarkMgr_inserted()));
        connect(m_manager->bookmarkMgr(), SIGNAL(removed(int,int)),
                this, SLOT(bookmarkMgr_removed()));
        connect(m_manager->tagMgr(), SIGNAL(destroyed()),
                this, SLOT(manager_destroyed()));
    }

    recalcTopLevelCounters();
    reset();
}

QVariant CNavigationItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_manager)
        return QVariant();

    if (!index.parent().isValid())
        return topLevelData(index, role);

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return QString("%1 (%2)")
                    .arg(item->data().name())
                    .arg(item->bookmarks().count());

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return QIcon(":/icons/bookmark-tag.png");

    if (role == Qt::UserRole)
        return QVariant::fromValue(index.internalPointer());

    return QVariant();
}

Qt::ItemFlags CNavigationItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (!index.parent().isValid())
        return Qt::ItemIsEnabled
                |Qt::ItemIsSelectable
                |Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled
            |Qt::ItemIsSelectable
            |Qt::ItemIsDragEnabled
            |Qt::ItemIsDropEnabled;
}

QStringList CNavigationItemModel::mimeTypes() const
{
    return QStringList() << "qookmarks/tag-list"
                         << "qookmarks/bookmark-list";
}

QMimeData *CNavigationItemModel::mimeData(const QModelIndexList &indexes) const
{
    QList<QStringList> tagPaths;
    foreach (const QModelIndex &index, indexes)
    {
        if (!index.isValid())
            continue;

        CTagItem *tagItem = static_cast<CTagItem *>(index.internalPointer());
        if (!tagItem)
            continue;

        tagPaths.push_back(tagItem->path());
    }

    if (tagPaths.isEmpty())
        return 0;

    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << tagPaths;

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("qookmarks/tag-list", encodedData);
    return mimeData;
}

bool CNavigationItemModel::dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);

    if (!m_manager)
        return false;

    CTagItem *parentItem = static_cast<CTagItem *>(parent.internalPointer());
    if (parentItem)
    {
        if (data->hasFormat("qookmarks/tag-list"))
            return dropMimeTagList(data, parentItem->path());
        else if (data->hasFormat("qookmarks/bookmark-list"))
            return dropMimeBookmarkList(data, parentItem->path());
    }
    else
    {
//        TopLevelItem type = m_topLevelItems[parent.row()];
//        if (type == Favorites)
//        {
//            emit bookmarksFavoriteMarking(
//        }
    }

    return false;
}

QVariant CNavigationItemModel::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal
            && role == Qt::DisplayRole
            && section == 0)
        return tr("Name");

    return QVariant();
}

QModelIndex CNavigationItemModel::index(int row, int column,
        const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_manager)
        return QModelIndex();

    if (!parent.isValid())
    {
        if (m_topLevelItems.at(row) == BookmarkRoot)
            return createIndex(row, column, m_manager->tagMgr()->rootItem());
        else
            return createIndex(row, column, 0);
    }

    CTagItem *parentItem = static_cast<CTagItem *>(parent.internalPointer());
    return createIndex(row, column, parentItem->at(row));
}

QModelIndex CNavigationItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || !m_manager)
        return QModelIndex();

    if (index.internalPointer() == 0
            || index.internalPointer() == m_manager->tagMgr()->rootItem())
        return QModelIndex();

    CTagItem *childItem = static_cast<CTagItem *>(index.internalPointer());
    CTagItem *parentItem = childItem->parent();
    if (parentItem == m_manager->tagMgr()->rootItem())
        return createIndex(bookmarkRootIndex(), 0, parentItem);

    return createIndex(parentItem->index(), 0, parentItem);
}

int CNavigationItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_topLevelItems.count();

    if (parent.column() != 0 || !m_manager)
        return 0;

    CTagItem *parentItem = static_cast<CTagItem *>(parent.internalPointer());
    if (!parentItem)
        return 0;

    return parentItem->count();
}

int CNavigationItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

void CNavigationItemModel::tagMgr_aboutToBeInserted(CTagItem *parent,
        int first, int last)
{
    if (parent == m_manager->tagMgr()->rootItem())
        beginInsertRows(createIndex(bookmarkRootIndex(), 0, parent), first, last);
    else
        beginInsertRows(createIndex(parent->index(), 0, parent), first, last);
}

void CNavigationItemModel::tagMgr_inserted(CTagItem *parent,
        int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    endInsertRows();
    updateBookmarkRootItem();
}

void CNavigationItemModel::tagMgr_aboutToBeRemoved(CTagItem *parent,
        int first, int last)
{
    if (parent == m_manager->tagMgr()->rootItem())
        beginRemoveRows(createIndex(bookmarkRootIndex(), 0, parent), first, last);
    else
        beginRemoveRows(createIndex(parent->index(), 0, parent), first, last);
}

void CNavigationItemModel::tagMgr_removed(CTagItem *parent,
        int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    endRemoveRows();
    updateBookmarkRootItem();
}

void CNavigationItemModel::tagMgr_aboutToBeMoved(CTagItem *srcParent,
        int srcFirst, int srcLast, CTagItem *dstParent, int dstIndex)
{
    QModelIndex si = createIndex(bookmarkRootIndex(), 0, m_manager->tagMgr()->rootItem());
    QModelIndex di = createIndex(bookmarkRootIndex(), 0, m_manager->tagMgr()->rootItem());
    if (srcParent != m_manager->tagMgr()->rootItem())
        si = createIndex(srcParent->index(), 0, srcParent);
    if (dstParent != m_manager->tagMgr()->rootItem())
        di = createIndex(dstParent->index(), 0, dstParent);
    beginMoveRows(si, srcFirst, srcLast, di, dstIndex);
}

void CNavigationItemModel::tagMgr_moved(CTagItem *srcParent, int srcFirst,
        int srcLast, CTagItem *dstParent, int dstIndex)
{
    Q_UNUSED(srcParent);
    Q_UNUSED(srcFirst);
    Q_UNUSED(srcLast);
    Q_UNUSED(dstParent);
    Q_UNUSED(dstIndex);

    endMoveRows();
}

void CNavigationItemModel::tagMgr_dataChanged(CTagItem *item)
{
    if (item->isRoot())
    {
        int index = bookmarkRootIndex();
        emit dataChanged(createIndex(index, 0, item),
                         createIndex(index,  columnCount()-1, item));
    }
    else
    {
        int index = item->index();
        emit dataChanged(createIndex(index, 0, item),
                         createIndex(index,  columnCount()-1, item));
    }
}

void CNavigationItemModel::tagMgr_bookmarksChanged(CTagItem *item)
{
    tagMgr_dataChanged(item);
}

void CNavigationItemModel::bookmarkMgr_dataChanged(CBookmarkItem *item,
        const CBookmark &oldData, const CBookmark &newData)
{
    Q_UNUSED(item);

    if (oldData.isFavorite() == newData.isFavorite()
            && oldData.isReadLater() == newData.isReadLater()
            && oldData.isTrash() == newData.isTrash()
            && oldData.rating() == newData.rating())
        return;

    if (oldData.isTrash())
    {
        m_topLevelCounters[Trash] -= 1;
    }
    else
    {
        if (oldData.isFavorite())
            m_topLevelCounters[Favorites] -= 1;
        if (oldData.rating())
            m_topLevelCounters[Rated] -= 1;
        if (oldData.isReadLater())
            m_topLevelCounters[ReadLater] -= 1;
    }

    if (newData.isTrash())
    {
        m_topLevelCounters[Trash] += 1;
    }
    else
    {
        if (newData.isFavorite())
            m_topLevelCounters[Favorites] += 1;
        if (newData.rating())
            m_topLevelCounters[Rated] += 1;
        if (newData.isReadLater())
            m_topLevelCounters[ReadLater] += 1;
    }

    emit dataChanged(createIndex(0, 0, 0),
                     createIndex(m_topLevelItems.count(), 0, 0));
}

void CNavigationItemModel::bookmarkMgr_inserted()
{
    updateBookmarkRootItem();
}

void CNavigationItemModel::bookmarkMgr_removed()
{
    updateBookmarkRootItem();
}

void CNavigationItemModel::manager_destroyed()
{
    m_manager = 0;
}

void CNavigationItemModel::initTopLevelItems()
{
    m_topLevelItems
            << Favorites
            << Rated
            << ReadLater
            << BookmarkRoot
            << Trash;
}

void CNavigationItemModel::initTopLevelCounters()
{
    m_topLevelCounters[Favorites] = 0;
    m_topLevelCounters[Rated] = 0;
    m_topLevelCounters[ReadLater] = 0;
    m_topLevelCounters[Trash] = 0;
}

int CNavigationItemModel::bookmarkRootIndex() const
{
    return m_topLevelItems.indexOf(BookmarkRoot);
}

int CNavigationItemModel::bookmarkRootCount() const
{
    return m_manager->bookmarkMgr()->count() - m_topLevelCounters[Trash];
}

void CNavigationItemModel::updateBookmarkRootItem()
{
    int index = bookmarkRootIndex();
    emit dataChanged(createIndex(index, 0, m_manager->tagMgr()->rootItem()),
                     createIndex(index,  columnCount()-1, m_manager->tagMgr()->rootItem()));
}

QVariant CNavigationItemModel::topLevelData(const QModelIndex &index,
        int role) const
{
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return topLevelName(m_topLevelItems.at(index.row()));

    if (role == Qt::DecorationRole)
        if (index.column() == 0)
            return topLevelIcon(m_topLevelItems.at(index.row()));

    return QVariant();
}

QString CNavigationItemModel::topLevelName(CNavigationItemModel::TopLevelItem item) const
{
    switch (item)
    {
    case Favorites:
        return tr("Favorites (%1)").arg(m_topLevelCounters[Favorites]);
    case Rated:
        return tr("Rated Bookmarks (%1)").arg(m_topLevelCounters[Rated]);
    case ReadLater:
        return tr("Read it Later (%1)").arg(m_topLevelCounters[ReadLater]);
    case BookmarkRoot:
        return tr("Bookmarks (%1)").arg(bookmarkRootCount());
    case Trash:
        return tr("Trash (%1)").arg(m_topLevelCounters[Trash]);
    }

    return QString();
}

QIcon CNavigationItemModel::topLevelIcon(CNavigationItemModel::TopLevelItem item) const
{
    switch (item)
    {
    case Favorites:
        return QIcon(":/icons/bookmark-favorites.png");
    case Rated:
        return QIcon(":/icons/bookmark-rated.png");
    case ReadLater:
        return QIcon(":/icons/bookmark-readlater.png");
    case BookmarkRoot:
        return QIcon(":/icons/bookmark-bookmarks.png");
    case Trash:
        return QIcon(":/icons/bookmark-trash.png");
    }

    return QIcon();
}

void CNavigationItemModel::recalcTopLevelCounters()
{
    initTopLevelCounters();
    foreach (CBookmarkItem *item, m_manager->bookmarkMgr()->bookmarks())
    {
        if (item->data().isTrash())
        {
            m_topLevelCounters[Trash] += 1;
            continue;
        }

        if (item->data().isFavorite())
            m_topLevelCounters[Favorites] += 1;
        if (item->data().rating())
            m_topLevelCounters[Rated] += 1;
        if (item->data().isReadLater())
            m_topLevelCounters[ReadLater] += 1;
    }
}

bool CNavigationItemModel::dropMimeTagList(const QMimeData *data,
        const QStringList &parentTag)
{
    emit tagsNeedMoving(fromMimeTagList(data), parentTag);
    return true;
}

bool CNavigationItemModel::dropMimeBookmarkList(const QMimeData *data,
        const QStringList &parentTag)
{
    emit bookmarksNeedTagging(fromMimeBookmarkList(data), parentTag);
    return true;
}

QList<QStringList> CNavigationItemModel::fromMimeTagList(const QMimeData *data)
{
    QByteArray encodedData = data->data("qookmarks/tag-list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QStringList> tagPaths;
    stream >> tagPaths;

    return tagPaths;
}

QList<QUrl> CNavigationItemModel::fromMimeBookmarkList(const QMimeData *data)
{
    QByteArray encodedData = data->data("qookmarks/bookmark-list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QUrl> bookmarkUrls;
    stream >> bookmarkUrls;

    return bookmarkUrls;
}
