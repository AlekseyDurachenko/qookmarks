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
#include "cwebiconfetchqueuemgr.h"
#include "cbookmarkmgr.h"
#include <QDebug>


CWebIconFetchQueueMgr::CWebIconFetchQueueMgr(CBookmarkMgr *mgr,
        QObject *parent) : QObject(parent)
{
    m_mgr = mgr;
    if (m_mgr)
    {
        connect(m_mgr, SIGNAL(destroyed()), this, SLOT(slot_mgr_destroyed()));
        connect(m_mgr, SIGNAL(bookmarkRemoved(CBookmarkItem*)),
                this, SLOT(slot_mgr_bookmarkRemoved(CBookmarkItem*)));
        connect(m_mgr, SIGNAL(bookmarkDataChanged(CBookmarkItem*)),
                this, SLOT(slot_mgr_bookmarkDataChanged(CBookmarkItem*)));
    }

    m_network = new QNetworkAccessManager(this);
    m_job = new CWebIconFetchJob(m_network, this);
    connect(m_job, SIGNAL(finished()), this, SLOT(slot_job_finished()));
}

int CWebIconFetchQueueMgr::indexOf(CBookmarkItem *item)
{
    for (int i = 0; i < m_itemList.count(); ++i)
        if (m_itemList.at(i).bookmark() == item)
            return i;
    return -1;
}

bool CWebIconFetchQueueMgr::add(CBookmarkItem *bookmark)
{
    foreach (const CWebIconFetchQueueItem &item, m_itemList)
        if (item.bookmark() == bookmark)
            return false;

    // TODO: reoreder by lastCheckDateTime?
    m_itemList << CWebIconFetchQueueItem(bookmark, CWebIconFetchQueueItem::Queue);
    emit itemInserted(m_itemList.count()-1, m_itemList.count()-1);

    next();

    return true;
}

void CWebIconFetchQueueMgr::removeAt(int row)
{
    m_itemList.removeAt(row);
    emit itemRemoved(row, row);
}

void CWebIconFetchQueueMgr::removeAll()
{
    int count = m_itemList.count();
    m_itemList.clear();
    emit itemRemoved(0, count-1);
}

void CWebIconFetchQueueMgr::slot_mgr_destroyed()
{
    m_mgr = 0;
}

void CWebIconFetchQueueMgr::slot_mgr_bookmarkRemoved(CBookmarkItem *item)
{
    int row = indexOf(item);
    if (row == -1)
        return;

    removeAt(row);
    // TODO: abort the jobs
}

void CWebIconFetchQueueMgr::slot_mgr_bookmarkDataChanged(CBookmarkItem *item)
{
    int row = indexOf(item);
    if (row == -1)
        return;

    emit itemDataChanged(row, row);
    // TODO: abort the jobs if url is changed
}

void CWebIconFetchQueueMgr::slot_job_finished()
{
    int row = -1;
    for (int i = 0; i < m_itemList.count(); ++i)
        if (m_itemList.at(i).bookmark()->data().url() == m_job->url())
            row = i;

    if (row != -1)
    {
        CBookmarkItemData data = m_itemList.at(row).bookmark()->data();
        data.setFavicon(m_job->icon());
        //data.setHttpResponseCode(m_job->httpError());
        //data.setHttpResponseText(m_job->httpErrorString());
        m_itemList.at(row).bookmark()->setData(data);
        removeAt(row);
    }

    next();
}

void CWebIconFetchQueueMgr::next()
{
    if (m_itemList.count() == 0 || m_job->isRunning())
        return;

    m_itemList.first().setStatus(CWebIconFetchQueueItem::Processing);
    m_job->start(m_itemList.first().bookmark()->data().url());
}
