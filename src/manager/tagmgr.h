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
#ifndef TAGMGR_H
#define TAGMGR_H

#include "tag.h"
#include <QAbstractItemModel>
class BookmarkItem;
class Manager;
class TagItem;


class TagMgr : public QObject
{
    Q_OBJECT

    friend class TagItem;
    friend class Manager;
private:
    explicit TagMgr(Manager *mgr = 0);
    virtual ~TagMgr();
public:
    inline Manager *mgr() const;
    inline TagItem *rootItem() const;
    TagItem *findByPath(const QStringList &path) const;
signals:
    void aboutToBeInserted(TagItem *parent ,int first, int last);
    void inserted(TagItem *parent, int first, int last);
    void aboutToBeRemoved(TagItem *parent, int first, int last);
    void removed(TagItem *parent, int first, int last);
    void aboutToBeMoved(TagItem *sourceParent, int sourceFirst, int sourceLast,
                        TagItem *destinationParent, int destinationIndex);
    void moved(TagItem *sourceParent, int sourceFirst, int sourceLast,
               TagItem *destinationParent, int destinationIndex);
    void aboutToBeDataChanged(TagItem *item);
    void dataChanged(TagItem *item);
    void bookmarksChanged(TagItem *item);
private:
    void callbackAboutToBeInserted(TagItem *parent ,int first, int last);
    void callbackInserted(TagItem *parent, int first, int last);
    void callbackAboutToBeRemoved(TagItem *parent, int first, int last);
    void callbackRemoved(TagItem *parent, int first, int last);
    void callbackAboutToBeMoved(TagItem *item);
    void callbackAboutToBeMoved(TagItem *sourceParent, int sourceFirst,
                                int sourceLast, TagItem *destinationParent,
                                int destinationIndex);
    void callbackMoved(TagItem *sourceParent, int sourceFirst, int sourceLast,
                       TagItem *destinationParent, int destinationIndex);
    void callbackMoved(TagItem *item);
    void callbackAboutToBeDataChanged(TagItem *item);
    void callbackDataChanged(TagItem *item);
    void callbackBookmarksChanged(TagItem *item);
private:
    Manager *m_mgr;
    TagItem *m_rootItem;
};

Manager *TagMgr::mgr() const
{
    return m_mgr;
}

TagItem *TagMgr::rootItem() const
{
    return m_rootItem;
}


#endif // TAGMGR_H
