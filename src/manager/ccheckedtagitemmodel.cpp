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
#include "ccheckedtagitemmodel.h"


CCheckedTagItemModel::CCheckedTagItemModel(QObject *parent) :
    CTagItemModel(parent)
{
}

void CCheckedTagItemModel::setCheckedTags(const QSet<CTagItem *> &checkedTags)
{
    m_checkedTags = checkedTags;
    beginResetModel();
    endResetModel();
}

QVariant CCheckedTagItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (role == Qt::CheckStateRole && index.column() == 0)
        return m_checkedTags.contains(item) ? Qt::Checked : Qt::Unchecked;

    return CTagItemModel::data(index, role);
}

bool CCheckedTagItemModel::setData(const QModelIndex &index,
        const QVariant &/*value*/, int role)
{
    if (role != Qt::CheckStateRole || index.column() != 0)
        return false;

    CTagItem *item = static_cast<CTagItem *>(index.internalPointer());
    if (m_checkedTags.contains(item))
        m_checkedTags.remove(item);
    else
        m_checkedTags.insert(item);

    return true;
}

Qt::ItemFlags CCheckedTagItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

   Qt::ItemFlags flags = CTagItemModel::flags(index);

   if (index.column() == 0)
       flags |= Qt::ItemIsUserCheckable;

   return flags;
}
