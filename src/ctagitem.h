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
#ifndef CTAGITEM_H
#define CTAGITEM_H

#include "ctagitemcallbackinterface.h"
#include <QVariant>

class CTagItem
{
public:
    enum Type
    {
        RootItem    = -1,
        Normal      = 0,
        Untagged    = 1,
        ReadLater   = 2,
        Favorites   = 3
    };
private:
    CTagItem(int id, Type type, const QString &tagName,
            CTagItemCallBackInterface *callback, CTagItem *parent = 0);
public:
    virtual ~CTagItem();

    inline int row() const;
    inline int id() const;
    inline Type type() const;
    inline CTagItem *parent() const;

    inline const QString &tagName() const;
    void setTagName(const QString &tagName);

    inline int columnCount() const;
    inline int childCount() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(int column, int role) const;
    CTagItem *child(int row);
    void add(CTagItem *item);
public:
    static CTagItem *create(const QString &tagName,
            CTagItemCallBackInterface *callback, CTagItem *parent = 0);
    // This method creates the root item (without a parent),
    // if root item is present in the database, tag will be readed
    // from the database (if type == Normal, all tag tree will be readed)
    static CTagItem *create(CTagItem::Type type,
            CTagItemCallBackInterface *callback, CTagItem *parent = 0);
private:
    void setRow(int row);
    static void createItemTree(CTagItem::Type type,
            CTagItemCallBackInterface *callback, CTagItem *parent);
private:
    int m_row;
    int m_id;
    Type m_type;
    QString m_tagName;
    CTagItemCallBackInterface *m_callback;
    CTagItem *m_parent;
    QList<CTagItem *> m_childList;
};

int CTagItem::row() const
{
    return m_row;
}

int CTagItem::id() const
{
    return m_id;
}

CTagItem::Type CTagItem::type() const
{
    return m_type;
}

CTagItem *CTagItem::parent() const
{
    return m_parent;
}

const QString &CTagItem::tagName() const
{
    return m_tagName;
}

int CTagItem::columnCount() const
{
    return 1;
}

int CTagItem::childCount() const
{
    return m_childList.count();
}

#endif // CTAGITEM_H
