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
#include "cnavtagitemmodel.h"
#include <QMimeData>
#include "cbookmarkitem.h"
#include "inavigationactions.h"
#include "singleton.h"


CNavTagItemModel::CNavTagItemModel(QObject *parent) :
    CTagItemModel(parent)
{
    m_navigationActions = 0;

    connect(GTagMgr(), SIGNAL(bookmarksChanged(CTagItem*)),
            this, SLOT(tagMgr_dataChanged(CTagItem*)));
}

void CNavTagItemModel::setNavigationActions(INavigationActions *interface)
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

QVariant CNavTagItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        if (index.column() == 0)
            return QString("%1 (%2)")
                    .arg(item->data().name())
                    .arg(bookmarkNotTrashedCount(item));

    return CTagItemModel::data(index, role);
}

Qt::ItemFlags CNavTagItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return CTagItemModel::flags(index)
            |Qt::ItemIsDragEnabled
            |Qt::ItemIsDropEnabled;
}

QStringList CNavTagItemModel::mimeTypes() const
{
    return QStringList() << "qookmarks/tag-list"
                         << "qookmarks/bookmark-list";
}

QMimeData *CNavTagItemModel::mimeData(const QModelIndexList &indexes) const
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

bool CNavTagItemModel::dropMimeData(const QMimeData *data,
        Qt::DropAction /*action*/, int /*row*/, int /*column*/,
        const QModelIndex &parent)
{
    CTagItem *parentItem = static_cast<CTagItem *>(parent.internalPointer());
    QStringList path = (parentItem ? parentItem->path() : QStringList());
    if (data->hasFormat("qookmarks/tag-list"))
        return dropMimeTagList(data, path);
    else if (data->hasFormat("qookmarks/bookmark-list"))
        return dropMimeBookmarkList(data, path);

    return false;
}

void CNavTagItemModel::navigationActions_destroyed()
{
    m_navigationActions = 0;
}

int CNavTagItemModel::bookmarkNotTrashedCount(CTagItem *item,
        bool /*recursive*/) const
{
    int count = 0;
    foreach (const CBookmarkItem *bookmark, item->bookmarks())
        if (!bookmark->data().isTrash())
            ++count;

    return count;
}

bool CNavTagItemModel::dropMimeTagList(const QMimeData *data,
        const QStringList &parentTag)
{
    if (m_navigationActions)
        m_navigationActions->tagsCopyOrMove(fromMimeTagList(data), parentTag);

    return true;
}

bool CNavTagItemModel::dropMimeBookmarkList(const QMimeData *data,
        const QStringList &parentTag)
{
    if (m_navigationActions)
        m_navigationActions->bookmarksAssignTag(fromMimeBookmarkList(data), parentTag);

    return true;
}

QList<QStringList> CNavTagItemModel::fromMimeTagList(const QMimeData *data)
{
    QByteArray encodedData = data->data("qookmarks/tag-list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QStringList> tagPaths;
    stream >> tagPaths;

    return tagPaths;
}

QList<QUrl> CNavTagItemModel::fromMimeBookmarkList(const QMimeData *data)
{
    QByteArray encodedData = data->data("qookmarks/bookmark-list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<QUrl> bookmarkUrls;
    stream >> bookmarkUrls;

    return bookmarkUrls;
}
