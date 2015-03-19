// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef CTAGMGR_H
#define CTAGMGR_H

#include <QObject>
#include "ctag.h"
class CTagItem;
class CBookmarkItem;
class CManager;


class CTagMgr : public QObject
{
    Q_OBJECT

    friend class CTagItem;
    friend class CManager;
private:
    explicit CTagMgr(CManager *mgr = 0);
    virtual ~CTagMgr();
public:
    inline CManager *mgr() const;
    inline CTagItem *rootItem() const;
    CTagItem *findByPath(const QStringList &path) const;
signals:
    void aboutToBeInserted(CTagItem *parent ,int first, int last);
    void inserted(CTagItem *parent, int first, int last);
    void aboutToBeRemoved(CTagItem *parent, int first, int last);
    void removed(CTagItem *parent, int first, int last);
    void aboutToBeMoved(CTagItem *sourceParent, int sourceFirst, int sourceLast,
                        CTagItem *destinationParent, int destinationIndex);
    void moved(CTagItem *sourceParent, int sourceFirst, int sourceLast,
               CTagItem *destinationParent, int destinationIndex);
    void dataChanged(CTagItem *item, const CTag &oldTag, const CTag &newTag);
    void bookmarksChanged(CTagItem *item);
private:
    void callbackAboutToBeInserted(CTagItem *parent ,int first, int last);
    void callbackInserted(CTagItem *parent, int first, int last);
    void callbackAboutToBeRemoved(CTagItem *parent, int first, int last);
    void callbackRemoved(CTagItem *parent, int first, int last);
    void callbackAboutToBeMoved(CTagItem *item);
    void callbackAboutToBeMoved(CTagItem *sourceParent, int sourceFirst,
                                int sourceLast, CTagItem *destinationParent,
                                int destinationIndex);
    void callbackMoved(CTagItem *sourceParent, int sourceFirst, int sourceLast,
                       CTagItem *destinationParent, int destinationIndex);
    void callbackMoved(CTagItem *item);
    void callbackDataChanged(CTagItem *item, const CTag &oldTag,
                             const CTag &newTag);
    void callbackBookmarksChanged(CTagItem *item);
private:
    CManager *m_mgr;
    CTagItem *m_rootItem;
};

CManager *CTagMgr::mgr() const
{
    return m_mgr;
}

CTagItem *CTagMgr::rootItem() const
{
    return m_rootItem;
}


#endif // CTAGMGR_H
