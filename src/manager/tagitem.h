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
#ifndef TAGITEM_H
#define TAGITEM_H

#include "tag.h"
#include <QSet>
#include <QVariant>
class BookmarkItem;
class TagMgr;


class TagItem
{
    friend class BookmarkItem;
    friend class TagMgr;
private:
    TagItem(const Tag &data, TagMgr *tagMgr, TagItem *parent = 0);
    ~TagItem();
public:
    inline TagMgr *tagMgr() const;
    inline TagItem *parent() const;
    inline int index() const;
    inline bool isRoot() const;

    inline const QSet<BookmarkItem *> &bookmarks() const;
    QSet<BookmarkItem *> bookmarksRecursively() const;
    void bookmarkAdd(BookmarkItem *item);
    void bookmarkRemove(BookmarkItem *item);

    inline int count();
    inline int indexOf(TagItem *item) const;
    int indexOf(const QString &name) const;
    inline const QList<TagItem *> &children() const;
    QSet<TagItem *> recursiveChildren() const;
    inline TagItem *at(int index) const;
    TagItem *find(const QString &name) const;
    bool aboveOf(TagItem *item) const;
    QStringList path() const;
    TagItem *add(const Tag &data);
    TagItem *replace(const Tag &data);
    void removeAt(int index);
    void removeAll();
    bool moveTo(TagItem *newParent);

    inline const Tag &data() const;
    bool setData(const Tag &data);
public:
    /*!
     * \brief checkIntersection check the intersection between two sets
     * \param a set one
     * \param b set two
     * \retval true if intersection exists
     * \retval otherwise
     */
    static bool checkIntersection(const QSet<TagItem *> &a,
                                  const QSet<TagItem *> &b);
public:
    static QVariant variantFromPtr(TagItem *item);
    static TagItem *variantToPtr(const QVariant &data);
private:
    void setParent(TagItem *parent);
    void add(TagItem *item);
    TagItem *takeAt(int index);
private:
    void notifyBookmarkAboutDestroyed();
private:
    void callbackBookmarkDestroyed(BookmarkItem *bookmark);
private:
    Tag m_data;
    TagMgr *m_tagMgr;
    TagItem *m_parent;
    QList<TagItem *> m_children;
    QSet<BookmarkItem *> m_bookmarks;
};

TagMgr *TagItem::tagMgr() const
{
    return m_tagMgr;
}

TagItem *TagItem::parent() const
{
    return m_parent;
}

int TagItem::index() const
{
    return (m_parent) ? (m_parent->indexOf(const_cast<TagItem *>(this))) : (0);
}

bool TagItem::isRoot() const
{
    return (m_parent) ? (false) : (true);
}

const QSet<BookmarkItem *> &TagItem::bookmarks() const
{
    return m_bookmarks;
}

int TagItem::count()
{
    return m_children.count();
}

int TagItem::indexOf(TagItem *item) const
{
    return m_children.indexOf(item);
}

TagItem *TagItem::at(int index) const
{
    return m_children.value(index);
}

const QList<TagItem *> &TagItem::children() const
{
    return m_children;
}

const Tag &TagItem::data() const
{
    return m_data;
}


#endif // TAGITEM_H
