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
    CTagItem *tagOtherItem() const;
    CTagItem *tagUntaggedItem() const;
    CTagItem *tagReadLaterItem() const;
    CTagItem *tagFavoritesItem() const;

    CTagItem *tagFind(CTagItem *parentItem, const QString &name);
    CTagItem *tagAdd(CTagItem *parentItem, const CTagItemData &data);
    void tagMove(CTagItem *newParent, CTagItem *item);
    void tagRemove(CTagItem *item);

    int bookmarkCount() const;
    CBookmarkItem *bookmarkAt(int index) const;
    CBookmarkItem *bookmarkFind(const QUrl &url);
    CBookmarkItem *bookmarkAdd(const CBookmarkItemData &data);
    void bookmarkRemove(CBookmarkItem *bookmark);

    void bookmarkAddTag(CBookmarkItem *bookmark, CTagItem *tag);
    void bookmarkRemoveTag(CBookmarkItem *bookmark, CTagItem *tag);
    void bookmarkClearTag(CBookmarkItem *bookmark);
protected:
    virtual void callbackBookmarkDataChanged(CBookmarkItem *bookmark);
    virtual void callbackTagDataChanged(CTagItem *tag);
    virtual void callbackTagInserted(CTagItem *parent, int first, int last);
    virtual void callbackTagRemoved(CTagItem *parent, int first, int last);
private:
    void tagInit();
    CTagItem *createTopLevelTag(CTagItem::Type type);
    void recursiveTagRead(CTagItem *parent);
    void bookmarkInit();
    QSet<CTagItem *> bookmarkTagRead(CBookmarkItem *bookmark);
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
    CTagItem *m_tagOtherItem;
    CTagItem *m_tagUntaggedItem;
    CTagItem *m_tagReadLaterItem;
    CTagItem *m_tagFavoritesItem;
};


#endif // CBOOKMARKMGR_H
