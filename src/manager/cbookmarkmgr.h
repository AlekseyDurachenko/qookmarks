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
#include "cwebcheckerqueuemgr.h"
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

    bool tagCanMove(CTagItem *newParent, CTagItem *item);
    CTagItem *tagFind(CTagItem *parentItem, const QString &name);
    CTagItem *tagFind(const QStringList &path);
    CTagItem *tagAdd(CTagItem *parentItem, const CTagItemData &data);
    bool tagMove(CTagItem *newParentItem, CTagItem *item);
    void tagRemove(CTagItem *item);
    void tagRemoveAll();

    int bookmarkCount() const;
    CBookmarkItem *bookmarkAt(int index) const;
    CBookmarkItem *bookmarkFind(const QUrl &url);
    CBookmarkItem *bookmarkAdd(const CBookmarkItemData &data);
    void bookmarkRemove(CBookmarkItem *bookmark);
    void bookmarkRemoveAt(int index);
    void bookmarkRemoveAll();

    inline CWebCheckerQueueMgr *webChecker() const;
protected:
    void callbackBookmarkDataChanged(CBookmarkItem *bookmark);
    void callbackTagDataChanged(CTagItem *tag);
    void callbackTagInserted(CTagItem *tag);
    void callbackTagRemoved(CTagItem *tag);
    void callbackTagInserted(CTagItem *parent, int first, int last);
    void callbackTagRemoved(CTagItem *parent, int first, int last);
private:
    void tagHierarchyCreate();
signals:
    // this signals are used to update the bookmark model
    void bookmarkInserted(int first, int last);
    void bookmarkRemoved(int first, int last);
    void bookmarkDataChanged(int first, int last);
    // this signals are used to update the tag model
    void tagInserted(CTagItem *parent, int first, int last);
    void tagRemoved(CTagItem *parent, int first, int last);
    void tagDataChanged(CTagItem *parent, int first, int last);
    // this signals are emited before the previouse
    void tagInserted(CTagItem *item);
    void tagRemoved(CTagItem *item);
    void tagDataChanged(CTagItem *item);
    void bookmarkInserted(CBookmarkItem *item);
    void bookmarkRemoved(CBookmarkItem *item);
    void bookmarkDataChanged(CBookmarkItem *item);
private:
    QList<CBookmarkItem *> m_bookmarkList;
    QHash<QUrl, CBookmarkItem *> m_bookmarkUrlHash;
    CTagItem *m_tagRootItem;
    CTagItem *m_tagFavoritesItem;   // favorites flag
    CTagItem *m_tagRatedItem;       // rate > 0
    CTagItem *m_tagReadLaterItem;   // read it later flag
    CTagItem *m_tagBookmarksItem;   // all bookmarks
    CTagItem *m_tagTrashItem;       // deleted flag
    //temporary
    CWebCheckerQueueMgr *m_webChecker;
};

CWebCheckerQueueMgr *CBookmarkMgr::webChecker() const
{
    return m_webChecker;
}


#endif // CBOOKMARKMGR_H
