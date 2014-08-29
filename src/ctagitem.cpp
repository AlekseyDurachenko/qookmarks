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
#include "ctagitem.h"
#include <QVariant>

CTagItem::CTagItem(Type type, int row, QObject *parent) : QObject(parent)
{
    m_row = row;
    m_type = type;
}

QVariant CTagItem::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return tr("Tag name");
        }
    }

    return QVariant();
}

QVariant CTagItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
        {
            return m_tagName;
        }
    }

    return QVariant();
}

CTagItem *CTagItem::child(int row)
{
    return m_childList[row];
}

CTagItem *CTagItem::add(const QString &tagName)
{
    CTagItem *item = new CTagItem(Normal, m_childList.count(), this);
    item->setTagName(tagName);
    m_childList.push_back(item);
    emit inserted(this, item->row(), item->row());
    return item;
}

void CTagItem::setTagName(const QString &tagName)
{
    m_tagName = tagName;
    emit changed(qobject_cast<CTagItem *>(parent()), row(), row());
}
