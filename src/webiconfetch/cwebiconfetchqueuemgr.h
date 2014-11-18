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
#ifndef CWEBICONFETCHQUEUEMGR_H
#define CWEBICONFETCHQUEUEMGR_H

#include "cwebiconfetchqueueitem.h"
#include "cwebiconfetchjob.h"
#include <QNetworkAccessManager>
class CBookmarkMgr;

class CWebIconFetchQueueMgr : public QObject
{
    Q_OBJECT
public:
    explicit CWebIconFetchQueueMgr(CBookmarkMgr *mgr, QObject *parent = 0);
    inline CBookmarkMgr *mgr() const;

    inline int count() const;
    inline const CWebIconFetchQueueItem &itemAt(int row) const;
    int indexOf(CBookmarkItem *item);
    bool add(CBookmarkItem *item);
    void removeAt(int row);
    void removeAll();
private slots:
    void slot_mgr_destroyed();
    void slot_mgr_bookmarkRemoved(CBookmarkItem *item);
    void slot_mgr_bookmarkDataChanged(CBookmarkItem *item);
    void slot_job_finished();
signals:
    void itemInserted(int first, int last);
    void itemRemoved(int first, int last);
    void itemDataChanged(int first, int last);
private:
    void next();
private:
    CBookmarkMgr *m_mgr;
    QNetworkAccessManager *m_network;
    CWebIconFetchJob *m_job;
    QList<CWebIconFetchQueueItem> m_itemList;
};

CBookmarkMgr *CWebIconFetchQueueMgr::mgr() const
{
    return m_mgr;
}

int CWebIconFetchQueueMgr::count() const
{
    return m_itemList.count();
}

const CWebIconFetchQueueItem &CWebIconFetchQueueMgr::itemAt(int row) const
{
    return m_itemList.at(row);
}


#endif // CWEBICONFETCHQUEUEMGR_H
