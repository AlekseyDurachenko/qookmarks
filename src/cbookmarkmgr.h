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
#ifndef CBOOKMARKMGR_H
#define CBOOKMARKMGR_H

#include "cbookmarkitem.h"
#include "ctagitem.h"
#include <QObject>


class CBookmarkMgr : public QObject
{
    Q_OBJECT
    friend class CBookmarkItem;
    friend class CTagItem;
public:
    explicit CBookmarkMgr(QObject *parent = 0);
    virtual ~CBookmarkMgr();

    CTagItem *tagRootItem() const;
    CTagItem *tagTagRootItem() const;
    CTagItem *tagReadLaterItem() const;
    CTagItem *tagFavoritesItem() const;

    CTagItem *tagAdd(CTagItem *parent, const CTagItemData &data);

    int bookmarkCount() const;
    CBookmarkItem *bookmarkAt(int index) const;
    CBookmarkItem *bookmarkAdd(const CBookmarkItemData &data);
    void bookmarkRemove(CBookmarkItem *bookmark);
protected:
    virtual void callbackBookmarkDataChanged(CBookmarkItem *bookmark);
    virtual void callbackTagDataChanged(CTagItem *tag);
private:
    void tagInit();
    CTagItem *createTopLevelTag(CTagItem::Type type);
    void bookmarkInit();
signals:
    void bookmarkInserted(int first, int last);
    void bookmarkRemoved(int first, int last);
    void bookmarkDataChanged(int first, int last);

    void tagInserted(CTagItem *parent, int first, int last);
    void tagRemoved(CTagItem *parent, int first, int last);
    void tagDataChanged(CTagItem *parent, int first, int last);
private:
    QList<CBookmarkItem *> m_bookmarkList;
    CTagItem *m_tagRootItem;
    CTagItem *m_tagTagRootItem;
    CTagItem *m_tagReadLaterItem;
    CTagItem *m_tagFavoritesItem;
};


#endif // CBOOKMARKMGR_H
