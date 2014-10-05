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

#include "ctagitemdata.h"
#include <QString>
#include <QList>
#include <QIcon>


class CBookmarkMgr;
class CTagItem
{
    friend class CBookmarkMgr;
public:
    enum Type
    {
        RootItem    = -1,
        Tag         =  0,
        Favorites   =  1,
        Rated       =  2,
        ReadLater   =  3,
        Bookmarks   =  4,
        Trash       =  5
    };
private:
    CTagItem(Type type, CBookmarkMgr *mgr, CTagItem *parent = 0);
    CTagItem(const CTagItemData &data, CBookmarkMgr *mgr, CTagItem *parent = 0);
public:
    virtual ~CTagItem();

    inline CBookmarkMgr *mgr() const;
    inline CTagItem *parent() const;
    inline Type type() const;
    int row() const;

    inline int childCount() const;
    inline int childIndexOf(CTagItem *item) const;
    inline CTagItem *childAt(int row) const;
    inline CTagItem *findChild(const QString &name) const;
    inline const QList<CTagItem *> &children() const;

    QIcon icon() const;

    inline const CTagItemData &data() const;
    bool canSetData(const CTagItemData &data);
    bool setData(const CTagItemData &data);
private:
    void setParent(CTagItem *parent);
    void addChild(CTagItem *item);
    CTagItem *takeChild(int row);
private:
    Type m_type;
    CTagItemData m_data;
    CBookmarkMgr *m_mgr;
    CTagItem *m_parent;
    QList<CTagItem *> m_childList;
    QHash<QString, CTagItem *> m_searchHash;    // quick search by name
};

CBookmarkMgr *CTagItem::mgr() const
{
    return m_mgr;
}

CTagItem *CTagItem::parent() const
{
    return m_parent;
}

CTagItem::Type CTagItem::type() const
{
    return m_type;
}

const CTagItemData &CTagItem::data() const
{
    return m_data;
}

const QList<CTagItem *> &CTagItem::children() const
{
    return m_childList;
}

int CTagItem::childCount() const
{
    return m_childList.count();
}

CTagItem *CTagItem::childAt(int row) const
{
    return m_childList.at(row);
}

CTagItem *CTagItem::findChild(const QString &name) const
{
    return m_searchHash.value(name, 0);
}

int CTagItem::childIndexOf(CTagItem *item) const
{
    return m_childList.indexOf(item);
}


#endif // CTAGITEM_H
