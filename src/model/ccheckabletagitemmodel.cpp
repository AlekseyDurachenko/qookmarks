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
#include "ccheckabletagitemmodel.h"
#include <ctagitem.h>
#include <QDebug>


CCheckableTagItemModel::CCheckableTagItemModel(QObject *parent) :
    CTagItemModel(parent)
{
}

CCheckableTagItemModel::CCheckableTagItemModel(CTagItem *rootItem,
        QObject *parent) : CTagItemModel(rootItem, parent)
{
}

QVariant CCheckableTagItemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole && index.column() == 0)
        return m_checkedList.contains(index) ? Qt::Checked : Qt::Unchecked;

    return CTagItemModel::data(index, role);
}

bool CCheckableTagItemModel::setData(const QModelIndex &index,
        const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        if (value == Qt::Unchecked)
            m_checkedList.removeOne(index);
        else if (value == Qt::Checked)
            m_checkedList.append(index);

        emit dataChanged(index, index);
        return true;
    }

    return CTagItemModel::setData(index, value, role);
}

Qt::ItemFlags CCheckableTagItemModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | CTagItemModel::flags(index);
}

void CCheckableTagItemModel::selectTag(CTagItem *item)
{
    for (int i = 0; i < rowCount(); ++i)
        if (selectTag(item, index(i, 0)))
            return;
}

bool CCheckableTagItemModel::selectTag(CTagItem *item,
        const QModelIndex &parent)
{
    if (parent.internalPointer() == item)
    {
        setData(parent, Qt::Checked, Qt::CheckStateRole);
        return true;
    }

    for (int i = 0; i < rowCount(parent); ++i)
        if (selectTag(item, index(i, 0, parent)))
            return true;

    return false;
}

QSet<CTagItem *> CCheckableTagItemModel::checkedTags()
{
    QSet<CTagItem *> list;
    foreach (const QPersistentModelIndex &i, m_checkedList)
        list << static_cast<CTagItem *>(i.internalPointer());
    return list;
}
