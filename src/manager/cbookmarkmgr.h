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
#include <QHash>

class CBookmarkMgr : public QObject
{
    Q_OBJECT
    friend class CBookmarkItem;
    friend class CTagItem;
public:
    explicit CBookmarkMgr(QObject *parent = 0);
    virtual ~CBookmarkMgr();

    CTagItem *tagRootItem() const;
    CTagItem *tagFavoritesItem() const;
    CTagItem *tagRatedItem() const;
    CTagItem *tagReadLaterItem() const;
    CTagItem *tagBookmarksItem() const;
    CTagItem *tagTrashItem() const;

    CTagItem *tagFind(CTagItem *parentItem, const QString &name);
    CTagItem *tagAdd(CTagItem *parentItem, const CTagItemData &data);
    bool tagMove(CTagItem *newParent, CTagItem *item);
    void tagRemove(CTagItem *item);

    int bookmarkCount() const;
    CBookmarkItem *bookmarkAt(int index) const;
    CBookmarkItem *bookmarkFind(const QUrl &url);
    CBookmarkItem *bookmarkAdd(const CBookmarkItemData &data);
    void bookmarkRemove(CBookmarkItem *bookmark);
    void bookmarkRemoveAt(int index);
    void bookmarkRemoveAll();
protected:
    virtual void callbackBookmarkDataChanged(CBookmarkItem *bookmark);
    virtual void callbackTagDataChanged(CTagItem *tag);
    virtual void callbackTagInserted(CTagItem *parent, int first, int last);
    virtual void callbackTagRemoved(CTagItem *parent, int first, int last);
private:
    void tagInit();
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
    QHash<QUrl, CBookmarkItem *> m_bookmarkSearchHash;
    CTagItem *m_tagRootItem;
    CTagItem *m_tagFavoritesItem;   // favorites flag
    CTagItem *m_tagRatedItem;       // rate > 0
    CTagItem *m_tagReadLaterItem;   // read it later flag
    CTagItem *m_tagBookmarksItem;   // all bookmarks
    CTagItem *m_tagTrashItem;       // deleted flag
};


#endif // CBOOKMARKMGR_H
