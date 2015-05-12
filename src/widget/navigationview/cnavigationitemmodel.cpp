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
#include <typeinfo>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "inavigationactions.h"
#include "cprj.h"
#include "singleton.h"


CNavigationItemModel::CNavigationItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_navigationActions = 0;
    initTopLevelItems();
    initTopLevelCounters();

    connect(GTagMgr(), SIGNAL(aboutToBeInserted(CTagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeInserted(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(inserted(CTagItem*,int,int)),
            this, SLOT(tagMgr_inserted(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(aboutToBeRemoved(CTagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeRemoved(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(removed(CTagItem*,int,int)),
            this, SLOT(tagMgr_removed(CTagItem*,int,int)));
    connect(GTagMgr(), SIGNAL(aboutToBeMoved(CTagItem*,int,int,CTagItem*,int)),
            this, SLOT(tagMgr_aboutToBeMoved(CTagItem*,int,int,CTagItem*,int)));
    connect(GTagMgr(), SIGNAL(moved(CTagItem*,int,int,CTagItem*,int)),
            this, SLOT(tagMgr_moved(CTagItem*,int,int,CTagItem*,int)));
    connect(GTagMgr(), SIGNAL(dataChanged(CTagItem*)),
            this, SLOT(tagMgr_dataChanged(CTagItem*)));
    connect(GTagMgr(), SIGNAL(bookmarksChanged(CTagItem*)),
            this, SLOT(tagMgr_bookmarksChanged(CTagItem*)));
    connect(GBookmarkMgr(), SIGNAL(aboutToBeDataChanged(CBookmarkItem*)),
            this, SLOT(bookmarkMgr_aboutToBeDataChanged(CBookmarkItem*)));
    connect(GBookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*)),
            this, SLOT(bookmarkMgr_dataChanged(CBookmarkItem*)));
    connect(GBookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(bookmarkMgr_inserted(int,int)));
    connect(GBookmarkMgr(), SIGNAL(aboutToBeRemoved(int,int)),
            this, SLOT(bookmarkMgr_aboutToBeRemoved(int,int)));
    connect(GBookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(bookmarkMgr_removed()));
    connect(GBookmarkMgr(), SIGNAL(aboutToBeTagsChanged(CBookmarkItem*)),
            this, SLOT(bookmarkMgr_aboutToBeTagsChanged(CBookmarkItem*)));
    connect(GBookmarkMgr(), SIGNAL(tagsChanged(CBookmarkItem*)),
            this, SLOT(bookmarkMgr_tagsChanged(CBookmarkItem*)));

    recalcTopLevelCounters();
    beginResetModel();
    endResetModel();

}


CNavigationItemModel::~CNavigationItemModel()
{
}

void CNavigationItemModel::setNavigationActions(
        INavigationActions *managerActionInterface)
{
    QObject *obj = dynamic_cast<QObject *>(m_navigationActions);
    if (obj)
        disconnect(obj, 0, this, 0);

    obj = dynamic_cast<QObject *>(managerActionInterface);
    if (obj)
        connect(obj, SIGNAL(destroyed()),
                this, SLOT(navigationActions_destroyed()));

    m_navigationActions = managerActionInterface;
}

QVariant CNavigationItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (!index.parent().isValid())
        return topLevelData(index, role);

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return QString("%1 (%2)")
                    .arg(item->data().name())
                    .arg(bookmarkNotTrashedCount(item));

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

    CTagItem *parentItem = static_cast<CTagItem *>(parent.internalPointer());
    if (parentItem)
    {
        if (data->hasFormat("qookmarks/tag-list"))
            return dropMimeTagList(data, parentItem->path());
        else if (data->hasFormat("qookmarks/bookmark-list"))
            return dropMimeBookmarkList(data, parentItem->path());
    }
    else if (m_navigationActions)
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

        TopLevelItem type = m_topLevelItems[parent.row()];
        switch (type)
        {
        case Favorites:
            m_navigationActions->navActFavorite(bookmarkList);
            break;
        case ReadLater:
            m_navigationActions->navActReadItLater(bookmarkList);
            break;
        case Trash:
            m_navigationActions->navActTrash(bookmarkList);
            break;
        default:
            ;
        }
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
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
    {
        if (m_topLevelItems.at(row) == BookmarkRoot)
            return createIndex(row, column, GTagMgr()->rootItem());
        else
            return createIndex(row, column, (void *)0);
    }

    CTagItem *parentItem = static_cast<CTagItem *>(parent.internalPointer());
    return createIndex(row, column, parentItem->at(row));
}

QModelIndex CNavigationItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    if (index.internalPointer() == 0
            || index.internalPointer() == GTagMgr()->rootItem())
        return QModelIndex();

    CTagItem *childItem = static_cast<CTagItem *>(index.internalPointer());
    CTagItem *parentItem = childItem->parent();
    if (parentItem == GTagMgr()->rootItem())
        return createIndex(bookmarkRootIndex(), 0, parentItem);

    return createIndex(parentItem->index(), 0, parentItem);
}

int CNavigationItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_topLevelItems.count();

    if (parent.column() != 0)
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
    if (parent == GTagMgr()->rootItem())
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
    if (parent == GTagMgr()->rootItem())
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
    QModelIndex si = createIndex(bookmarkRootIndex(), 0, GTagMgr()->rootItem());
    QModelIndex di = createIndex(bookmarkRootIndex(), 0, GTagMgr()->rootItem());
    if (srcParent != GTagMgr()->rootItem())
        si = createIndex(srcParent->index(), 0, srcParent);
    if (dstParent != GTagMgr()->rootItem())
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

void CNavigationItemModel::bookmarkMgr_aboutToBeDataChanged(CBookmarkItem *item)
{
    m_oldBookmarkData = item->data();
}

void CNavigationItemModel::bookmarkMgr_dataChanged(CBookmarkItem *item)
{
    if (m_oldBookmarkData.isFavorite() == item->data().isFavorite()
            && m_oldBookmarkData.isReadItLater() == item->data().isReadItLater()
            && m_oldBookmarkData.isTrash() == item->data().isTrash()
            && m_oldBookmarkData.rating() == item->data().rating())
        return;

    if (m_oldBookmarkData.isTrash())
    {
        m_topLevelCounters[Trash] -= 1;
    }
    else
    {
        if (m_oldBookmarkData.isFavorite())
            m_topLevelCounters[Favorites] -= 1;
        if (m_oldBookmarkData.rating())
            m_topLevelCounters[Rated] -= 1;
        if (m_oldBookmarkData.isReadItLater())
            m_topLevelCounters[ReadLater] -= 1;
        if (item->tags().isEmpty())
            m_topLevelCounters[Untagged] -= 1;
    }

    if (item->data().isTrash())
    {
        m_topLevelCounters[Trash] += 1;
    }
    else
    {
        if (item->data().isFavorite())
            m_topLevelCounters[Favorites] += 1;
        if (item->data().rating())
            m_topLevelCounters[Rated] += 1;
        if (item->data().isReadItLater())
            m_topLevelCounters[ReadLater] += 1;
        if (item->tags().isEmpty())
            m_topLevelCounters[Untagged] -= 1;
    }

    emit dataChanged(createIndex(0, 0, (void *)0),
                     createIndex(m_topLevelItems.count(), 0, (void *)0));
}

void CNavigationItemModel::bookmarkMgr_inserted(int first, int last)
{
    for (int i = first; i <= last; ++i)
    {
        CBookmarkItem *item = GBookmarkMgr()->at(i);
        CBookmark data = item->data();
        if (data.isTrash())
        {
            m_topLevelCounters[Trash] += 1;
        }
        else
        {
            if (data.isFavorite())
                m_topLevelCounters[Favorites] += 1;
            if (data.rating())
                m_topLevelCounters[Rated] += 1;
            if (data.isReadItLater())
                m_topLevelCounters[ReadLater] += 1;
            if (item->tags().isEmpty())
                m_topLevelCounters[Untagged] += 1;
        }
    }

    emit dataChanged(createIndex(0, 0, (void *)0),
                     createIndex(m_topLevelItems.count(), 0, (void *)0));
}

void CNavigationItemModel::bookmarkMgr_aboutToBeRemoved(int first, int last)
{
    for (int i = first; i <= last; ++i)
    {
        CBookmarkItem *item = GBookmarkMgr()->at(i);
        CBookmark data = item->data();
        if (data.isTrash())
        {
            m_topLevelCounters[Trash] -= 1;
        }
        else
        {
            if (data.isFavorite())
                m_topLevelCounters[Favorites] -= 1;
            if (data.rating())
                m_topLevelCounters[Rated] -= 1;
            if (data.isReadItLater())
                m_topLevelCounters[ReadLater] -= 1;
            if (item->tags().isEmpty())
                m_topLevelCounters[Untagged] -= 1;
        }
    }

    emit dataChanged(createIndex(0, 0, (void *)0),
                     createIndex(m_topLevelItems.count(), 0, (void *)0));
}

void CNavigationItemModel::bookmarkMgr_removed()
{
    updateBookmarkRootItem();
}

void CNavigationItemModel::bookmarkMgr_aboutToBeTagsChanged(CBookmarkItem *item)
{
    if (item->tags().isEmpty() && !item->data().isTrash())
        m_topLevelCounters[Untagged] -= 1;

    emit dataChanged(createIndex(0, 0, (void *)0),
                     createIndex(m_topLevelItems.count(), 0, (void *)0));
}

void CNavigationItemModel::bookmarkMgr_tagsChanged(CBookmarkItem *item)
{
    if (item->tags().isEmpty() && !item->data().isTrash())
        m_topLevelCounters[Untagged] += 1;

    emit dataChanged(createIndex(0, 0, (void *)0),
                     createIndex(m_topLevelItems.count(), 0, (void *)0));
}

void CNavigationItemModel::navigationActions_destroyed()
{
    m_navigationActions = 0;
}

void CNavigationItemModel::initTopLevelItems()
{
    m_topLevelItems
            << Favorites
            << Rated
            << ReadLater
            << Untagged
            << BookmarkRoot
            << Trash;
}

void CNavigationItemModel::initTopLevelCounters()
{
    m_topLevelCounters[Favorites] = 0;
    m_topLevelCounters[Rated] = 0;
    m_topLevelCounters[ReadLater] = 0;
    m_topLevelCounters[Untagged] = 0;
    m_topLevelCounters[Trash] = 0;
}

int CNavigationItemModel::bookmarkRootIndex() const
{
    return m_topLevelItems.indexOf(BookmarkRoot);
}

int CNavigationItemModel::bookmarkRootCount() const
{
    return GBookmarkMgr()->count() - m_topLevelCounters[Trash];
}

int CNavigationItemModel::bookmarkNotTrashedCount(CTagItem *item,
        bool /*recursive*/) const
{
    int count = 0;
    foreach (const CBookmarkItem *bookmark, item->bookmarks())
        if (!bookmark->data().isTrash())
            ++count;

    return count;
}

void CNavigationItemModel::updateBookmarkRootItem()
{
    int index = bookmarkRootIndex();
    emit dataChanged(createIndex(index, 0, GTagMgr()->rootItem()),
                     createIndex(index,  columnCount()-1, GTagMgr()->rootItem()));
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

    if (role == Qt::UserRole)
        if (index.column() == 0)
            return m_topLevelItems.at(index.row());

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
    case Untagged:
        return tr("Untagged (%1)").arg(m_topLevelCounters[Untagged]);
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
    case Untagged:
        return QIcon(":/icons/bookmark-untagged.png");
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
    foreach (CBookmarkItem *item, GBookmarkMgr()->bookmarks())
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
        if (item->data().isReadItLater())
            m_topLevelCounters[ReadLater] += 1;
        if (item->tags().isEmpty())
            m_topLevelCounters[Untagged] += 1;
    }
}

bool CNavigationItemModel::dropMimeTagList(const QMimeData *data,
        const QStringList &parentTag)
{
    if (m_navigationActions)
        m_navigationActions->navActMoveTags(fromMimeTagList(data), parentTag);

    return true;
}

bool CNavigationItemModel::dropMimeBookmarkList(const QMimeData *data,
        const QStringList &parentTag)
{
    if (m_navigationActions)
        m_navigationActions->navActSetTag(fromMimeBookmarkList(data), parentTag);

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
