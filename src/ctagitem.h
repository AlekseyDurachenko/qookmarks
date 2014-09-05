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
#include <QString>
#include <QList>

class CBookmarkItem;
class CTagItem
{
    friend class CBookmarkMgr;
    friend class CBookmarkItem;
public:
    enum Type
    {
        RootItem    = -1,
        Tag         =  0,
        Untagged    =  1,
        ReadLater   =  2,
        Favorites   =  3
    };
private:
    CTagItem(int id, Type type, const QString &title,
            CTagItemCallBackInterface *callback, CTagItem *parent = 0);
public:
    virtual ~CTagItem();

    inline int row() const;
    inline int id() const;
    inline Type type() const;
    inline CTagItem *parent() const;

    inline const QString &title() const;
    void setTitle(const QString &title);

    inline int childCount() const;
    CTagItem *childAt(int row) const;
    void add(CTagItem *item);
    CTagItem *takeAt(int row);
public:
    static CTagItem *create(const QString &title,
            CTagItemCallBackInterface *callback, CTagItem *parent = 0);
    // This method creates the root item (without a parent),
    // if root item is present in the database, tag will be readed
    // from the database (if type == Normal, all tag tree will be readed)
    static CTagItem *create(CTagItem::Type type,
            CTagItemCallBackInterface *callback, CTagItem *parent = 0);
private:
    void setParent(CTagItem *parent);
    void removeAt(int row);
    void bookmarkAdd(CBookmarkItem *item);
    void bookmarkRemove(CBookmarkItem *item);
    void setRow(int row);
    static void readItemTree(CTagItem::Type type,
            CTagItemCallBackInterface *callback, CTagItem *parent);
private:
    int m_row;
    int m_id;
    Type m_type;
    QString m_tagName;
    CTagItemCallBackInterface *m_callback;
    CTagItem *m_parent;
    QList<CTagItem *> m_childList;
    QList<CBookmarkItem *> m_bookmarkList;
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

const QString &CTagItem::title() const
{
    return m_tagName;
}


int CTagItem::childCount() const
{
    return m_childList.count();
}

#endif // CTAGITEM_H
