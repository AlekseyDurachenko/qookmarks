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
#include "cbookmarkitem.h"
#include <QDebug>

CTagItem::CTagItem(int id, Type type, const QString &tagName,
        CTagItemCallBackInterface *callback, CTagItem *parent)
{
    m_row = -1;
    m_id = id;
    m_type = type;
    m_tagName = tagName;
    m_callback = callback;
    m_parent = parent;
}

CTagItem::~CTagItem()
{
    while (m_bookmarkList.count())
        m_bookmarkList.first()->tagRemove(this);

    foreach (CTagItem *item, m_childList)
        delete item;
}

void CTagItem::setTitle(const QString &tagName)
{
    m_tagName = tagName;
    CStorage::updateTagName(m_id, tagName);
    //m_callback->rowChange(m_parent, m_row, m_row);
}

void CTagItem::bookmarkAdd(CBookmarkItem *item)
{
    qDebug() << title() << "CTagItem::bookMarkAdd" << item->title();
    m_bookmarkList.push_back(item);
}

void CTagItem::bookmarkRemove(CBookmarkItem *item)
{
    qDebug() << title() << "CTagItem::bookMarkRemove" << item->title();
    m_bookmarkList.removeAll(item);
}

CTagItem *CTagItem::childAt(int row) const
{
    return m_childList[row];
}

void CTagItem::addChild(CTagItem *item)
{
    int row = m_childList.count();
    item->setRow(row);
    m_childList.push_back(item);
    //m_callback->rowInsert(this, row, row);
}

CTagItem *CTagItem::takeChild(int row)
{
    CTagItem *item = m_childList.takeAt(row);
    item->setParent(0);
    return item;
}

void CTagItem::removeAt(int row)
{
    delete m_childList[row];
    m_childList.removeAt(row);
}

CTagItem *CTagItem::create(const QString &tagName,
        CTagItemCallBackInterface *callback, CTagItem *parent)
{
    int parentId = ((parent) ? (parent->id()) : (-1));
    int id = CStorage::insertTag(parentId, tagName, Tag);
    return new CTagItem(id, Tag, tagName, callback, parent);
}

void CTagItem::readItemTree(CTagItem::Type type,
        CTagItemCallBackInterface *callback, CTagItem *parent)
{
    QSqlQuery query = CStorage::selectTags(type, parent->id());
    while (query.next())
    {
        int id = query.value(0).toInt();
        QString tagName = query.value(1).toString();

        CTagItem *item = new CTagItem(id, type, tagName, callback, parent);
        readItemTree(type, callback, item);

        parent->addChild(item);
    }
}

CTagItem *CTagItem::create(CTagItem::Type type,
        CTagItemCallBackInterface *callback, CTagItem *parent)
{
    if (type == RootItem)
    {
        return new CTagItem(-1, type, "", callback, parent);
    }

    QSqlQuery query = CStorage::selectTags(type);
    if (query.next())
    {
        int id = query.value(0).toInt();

        CTagItem *item = new CTagItem(id, type, "", callback, parent);
        readItemTree(type, callback, item);

        return item;
    }

    int id = CStorage::insertTag(-1, "", type);
    return new CTagItem(id, type, "", callback, parent);
}

void CTagItem::setParent(CTagItem *parent)
{
    m_parent = parent;
}

void CTagItem::setRow(int row)
{
    m_row = row;
}
