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
#ifndef CBOOKMARKITEM_H
#define CBOOKMARKITEM_H

#include <QSet>
#include "cbookmark.h"
class CTagItem;
class CBookmarkMgr;


class CBookmarkItem
{
    friend class CBookmarkMgr;
    friend class CTagItem;
private:
    CBookmarkItem(const CBookmark &data, CBookmarkMgr *bookmarkMgr);
    ~CBookmarkItem();
public:
    inline CBookmarkMgr *bookmarkMgr() const;
    int index() const;

    inline const CBookmark &data() const;
    bool setData(const CBookmark &data);

    inline const QSet<CTagItem *> &tags() const;
private:
    void notifyTagAboutDestroyed();
private:
    void callbackTagAdd(CTagItem *tag);
    void callbackTagRemove(CTagItem *tag);
private:
    CBookmark m_data;
    CBookmarkMgr *m_bookmarkMgr;
    QSet<CTagItem *> m_tags;
};

CBookmarkMgr *CBookmarkItem::bookmarkMgr() const
{
    return m_bookmarkMgr;
}

const CBookmark &CBookmarkItem::data() const
{
    return m_data;
}

const QSet<CTagItem *> &CBookmarkItem::tags() const
{
    return m_tags;
}


#endif // CBOOKMARKITEM_H
