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
#include "cstorage.h"
#include <QVariant>

CTagItem::CTagItem(int id, Type type, const QString &tagName,
        QObject *parent) : QObject(parent)
{
    m_row = -1;
    m_id = id;
    m_type = type;
    m_tagName = tagName;
}

void CTagItem::setTagName(const QString &tagName)
{
    m_tagName = tagName;
    CStorage::updateTagName(m_id, tagName);
    emit dataChanged(qobject_cast<CTagItem *>(parent()), row(), row());
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
            if (parent()
                    && parent()->metaObject()->className() == QByteArray("CTagItem")
                    && qobject_cast<CTagItem *>(parent())->id() == -1)
            {
                switch(m_type)
                {
                case RootItem:
                    return tr("/");
                case Normal:
                    return tr("Tags");
                case Untagged:
                    return tr("Untagged");
                case ReadLater:
                    return tr("Read it later");
                case Favorites:
                    return tr("Favorites");
                }
            }
            return m_tagName;
        }
    }

    return QVariant();
}

CTagItem *CTagItem::child(int row)
{
    return m_childList[row];
}

void CTagItem::add(CTagItem *item)
{
    int row = m_childList.count();
    item->setRow(row);
    m_childList.push_back(item);
    emit rowInserted(this, row, row);
}

CTagItem *CTagItem::create(const QString &tagName, QObject *parent)
{
    int parentId = -1;
    if (parent && parent->metaObject()->className() == QByteArray("CTagItem"))
    {
        parentId = qobject_cast<CTagItem *>(parent)->id();
    }
    int id = CStorage::insertTag(parentId, tagName, Normal);
    return new CTagItem(id, Normal, tagName, parent);
}

void CTagItem::createItemTree(CTagItem::Type type, CTagItem *parent)
{
    QSqlQuery query = CStorage::selectTags(type, parent->id());
    while (query.next())
    {
        int id = query.value(0).toInt();
        QString tagName = query.value(1).toString();
        CTagItem *item = new CTagItem(id, type, tagName, parent);
        parent->add(item);
        createItemTree(type, item);
    }
}

CTagItem *CTagItem::create(CTagItem::Type type, QObject *parent)
{
    if (type == RootItem)
    {
        return new CTagItem(-1, type, "", parent);
    }

    QSqlQuery query = CStorage::selectTags(type);
    if (query.next())
    {
        int id = query.value(0).toInt();
        CTagItem *item = new CTagItem(id, type, "", parent);
        createItemTree(type, item);
        return item;
    }

    int id = CStorage::insertTag(-1, "", type);
    return new CTagItem(id, type, "", parent);
}

void CTagItem::setRow(int row)
{
    m_row = row;
}
