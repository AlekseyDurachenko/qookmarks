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
#include "cbookmarktagproxymodel.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include <QDebug>

CBookmarkTagProxyModel::CBookmarkTagProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void CBookmarkTagProxyModel::setTagFilter(const QSet<CTagItem *> &tags)
{
    if (m_tags != tags)
    {
        m_tags = tags;
        invalidateFilter();
    }
}

bool CBookmarkTagProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    CBookmarkItem *item = static_cast<CBookmarkItem *>(index.internalPointer());

    if (m_tags.contains(item->mgr()->tagUntaggedItem())
            && item->tags().isEmpty())
        return true;

    return item->isTagsIntersected(m_tags);
}