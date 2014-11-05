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
#ifndef CWEBCHECKERQUEUEMGR_H
#define CWEBCHECKERQUEUEMGR_H

#include "cbookmarkmgr.h"
#include "cwebcheckerqueueitem.h"
#include "cwebcheckerjob.h"
#include <QNetworkAccessManager>


class CWebCheckerQueueMgr : public QObject
{
    Q_OBJECT
public:
    explicit CWebCheckerQueueMgr(CBookmarkMgr *mgr, QObject *parent = 0);
    inline CBookmarkMgr *mgr() const;

    inline int count() const;
    inline const CWebCheckerQueueItem &itemAt(int row) const;
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
    CWebCheckerJob *m_job;
    QList<CWebCheckerQueueItem> m_itemList;
};

CBookmarkMgr *CWebCheckerQueueMgr::mgr() const
{
    return m_mgr;
}

int CWebCheckerQueueMgr::count() const
{
    return m_itemList.count();
}

const CWebCheckerQueueItem &CWebCheckerQueueMgr::itemAt(int row) const
{
    return m_itemList.at(row);
}


#endif // CWEBCHECKERQUEUEMGR_H
