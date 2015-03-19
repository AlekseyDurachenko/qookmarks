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
#ifndef CTAGITEM_H
#define CTAGITEM_H

#include <QSet>
#include "ctag.h"
class CTagMgr;
class CBookmarkItem;


class CTagItem
{
    friend class CBookmarkItem;
    friend class CTagMgr;
private:
    CTagItem(const CTag &data, CTagMgr *tagMgr, CTagItem *parent = 0);
    ~CTagItem();
public:
    inline CTagMgr *tagMgr() const;
    inline CTagItem *parent() const;
    inline int index() const;
    inline bool isRoot() const;

    inline const QSet<CBookmarkItem *> &bookmarks() const;
    QSet<CBookmarkItem *> bookmarksRecursively() const;
    void bookmarkAdd(CBookmarkItem *item);
    void bookmarkRemove(CBookmarkItem *item);

    inline int count();
    inline int indexOf(CTagItem *item) const;
    int indexOf(const QString &name) const;
    inline const QList<CTagItem *> &children() const;
    inline CTagItem *at(int index) const;
    CTagItem *find(const QString &name) const;
    bool aboveOf(CTagItem *item) const;
    QStringList path() const;
    CTagItem *add(const CTag &data);
    CTagItem *replace(const CTag &data);
    void removeAt(int index);
    void removeAll();
    bool moveTo(CTagItem *newParent);

    inline const CTag &data() const;
    bool setData(const CTag &data);
public:
    /*!
     * \brief checkIntersection check the intersection between two sets
     * \param a set one
     * \param b set two
     * \retval true if intersection exists
     * \retval otherwise
     */
    static bool checkIntersection(const QSet<CTagItem *> &a,
                                  const QSet<CTagItem *> &b);
private:
    void setParent(CTagItem *parent);
    void add(CTagItem *item);
    CTagItem *takeAt(int index);
private:
    void notifyBookmarkAboutDestroyed();
private:
    void callbackBookmarkDestroyed(CBookmarkItem *bookmark);
private:
    CTag m_data;
    CTagMgr *m_tagMgr;
    CTagItem *m_parent;
    QList<CTagItem *> m_children;
    QSet<CBookmarkItem *> m_bookmarks;
};

CTagMgr *CTagItem::tagMgr() const
{
    return m_tagMgr;
}

CTagItem *CTagItem::parent() const
{
    return m_parent;
}

int CTagItem::index() const
{
    return (m_parent) ? (m_parent->indexOf(const_cast<CTagItem *>(this))) : (0);
}

bool CTagItem::isRoot() const
{
    return (m_parent) ? (false) : (true);
}

const QSet<CBookmarkItem *> &CTagItem::bookmarks() const
{
    return m_bookmarks;
}

int CTagItem::count()
{
    return m_children.count();
}

int CTagItem::indexOf(CTagItem *item) const
{
    return m_children.indexOf(item);
}

CTagItem *CTagItem::at(int index) const
{
    return m_children.value(index);
}

const QList<CTagItem *> &CTagItem::children() const
{
    return m_children;
}

const CTag &CTagItem::data() const
{
    return m_data;
}


#endif // CTAGITEM_H
