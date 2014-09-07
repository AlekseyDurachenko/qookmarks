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
#ifndef CBOOKMARKITEM_H
#define CBOOKMARKITEM_H

#include "cbookmarkitemdata.h"
#include <QString>
#include <QList>
class CTagItem;
class CBookmarkMgr;


class CBookmarkItem
{
    friend class CBookmarkMgr;
private:
    CBookmarkItem(const CBookmarkItemData &data, CBookmarkMgr *mgr);
    CBookmarkItem(int id, const CBookmarkItemData &data, CBookmarkMgr *mgr);
public:
    inline CBookmarkMgr *mgr() const;
    inline int id() const;

    inline const CBookmarkItemData &data() const;
    void setData(const CBookmarkItemData &data);
private:
    void setId(int id);
private:
    int m_id;
    CBookmarkItemData m_data;
    CBookmarkMgr *m_mgr;
};

CBookmarkMgr *CBookmarkItem::mgr() const
{
    return m_mgr;
}

int CBookmarkItem::id() const
{
    return m_id;
}

const CBookmarkItemData &CBookmarkItem::data() const
{
    return m_data;
}


#endif // CBOOKMARKITEM_H
