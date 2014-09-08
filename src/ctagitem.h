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

class CBookmarkMgr;
class CTagItem
{
    friend class CBookmarkMgr;
public:
    enum Type
    {
        RootItem    = -1,
        Tag         =  0,
        ReadLater   =  2,
        Favorites   =  3
    };
private:
    CTagItem(Type type, const CTagItemData &data, CBookmarkMgr *mgr,
            CTagItem *parent = 0);
    CTagItem(int id, Type type, const CTagItemData &data, CBookmarkMgr *mgr,
            CTagItem *parent = 0);
public:
    ~CTagItem();

    inline CBookmarkMgr *mgr() const;
    inline CTagItem *parent() const;
    inline int id() const;
    inline Type type() const;
    int row() const;

    inline const CTagItemData &data() const;
    void setData(const CTagItemData &data);

    int childCount() const;
    CTagItem *childAt(int row) const;
private:
    void addChild(CTagItem *item);
    CTagItem *takeChild(int row);
    int childIndexOf(CTagItem *item) const;
    void setParent(CTagItem *parent);
    void setId(int id);
private:
    int m_id;
    Type m_type;
    CTagItemData m_data;
    CBookmarkMgr *m_mgr;
    CTagItem *m_parent;
    QList<CTagItem *> m_childList;
};

CBookmarkMgr *CTagItem::mgr() const
{
    return m_mgr;
}

CTagItem *CTagItem::parent() const
{
    return m_parent;
}

int CTagItem::id() const
{
    return m_id;
}

CTagItem::Type CTagItem::type() const
{
    return m_type;
}

const CTagItemData &CTagItem::data() const
{
    return m_data;
}

#endif // CTAGITEM_H
