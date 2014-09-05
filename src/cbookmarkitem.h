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

#include <QString>
#include <QList>
class CTagItem;
class CBookmarkMgr;

class CBookmarkItem
{
    friend class CBookmarkMgr;
private:
    CBookmarkItem(int id, const QString &title, CBookmarkMgr *mgr,
            CBookmarkItem *parent = 0);
public:
    ~CBookmarkItem();

    inline CBookmarkItem *parent() const;
    inline int row() const;
    inline int id() const;

    inline const QString &title() const;
    void setTitle(const QString &title);

    inline int tagCount() const;
    inline CTagItem *tagAt(int i) const;
    bool tagContains(CTagItem *item) const;
    void tagAdd(CTagItem *item);
    void tagRemove(CTagItem *item);

    inline int childCount() const;
    inline CBookmarkItem *childAt(int row) const;
    void addChild(CBookmarkItem *item);
    void remove(int row);
private:
    void setRow(int row);
private:
    int m_row;
    int m_id;
    QString m_title;
    CBookmarkMgr *m_mgr;
    CBookmarkItem *m_parent;
    QList<CBookmarkItem *> m_childList;
    QList<CTagItem *> m_tagList;
};

CBookmarkItem *CBookmarkItem::parent() const
{
    return m_parent;
}

int CBookmarkItem::row() const
{
    return m_row;
}

int CBookmarkItem::id() const
{
    return m_id;
}

const QString &CBookmarkItem::title() const
{
    return m_title;
}

int CBookmarkItem::tagCount() const
{
    return m_tagList.count();
}

CTagItem *CBookmarkItem::tagAt(int i) const
{
    return m_tagList[i];
}

int CBookmarkItem::childCount() const
{
    return m_childList.count();
}

CBookmarkItem *CBookmarkItem::childAt(int row) const
{
    return m_childList[row];
}

#endif // CBOOKMARKITEM_H
