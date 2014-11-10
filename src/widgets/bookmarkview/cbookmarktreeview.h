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
#ifndef CBOOKMARKTREEVIEW_H
#define CBOOKMARKTREEVIEW_H

#include "cbookmarkmgr.h"
#include "cbookmarkitemmodel.h"
#include "cbookmarksortfilterproxymodel.h"
#include <QTreeView>


class CBookmarkTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CBookmarkTreeView(QWidget *parent = 0);

    inline CBookmarkMgr *mgr() const;
    void setBookmarkMgr(CBookmarkMgr *mgr);

    inline CBookmarkSortFilterProxyModel *softFilterProxyModel() const;

    inline QAction *actionBookmarkAdd() const;
    inline QAction *actionBookmarkEdit() const;
    inline QAction *actionBookmarkRemove() const;

    virtual int sizeHintForColumn(int column) const;
signals:
    void currentBookmarkChanged(CBookmarkItem *bookmark);
private slots:
    void onMgrDestroyed();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onActionBookmarkAddTriggered();
    void onActionBookmarkEditTriggered();
    void onActionBookmarkRemoveTriggered();
    void onActionHttpCheckTriggered();
    void onActionOpenUrlTriggered();
protected:
    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous);
private:
    void updateActions();
private:
    CBookmarkMgr *m_mgr;
    CBookmarkItemModel *m_bookmarkModel;
    CBookmarkSortFilterProxyModel *m_sortFilterProxyModel;
    QAction *m_actionBookmarkAdd;
    QAction *m_actionBookmarkEdit;
    QAction *m_actionBookmarkRemove;
    QAction *m_actionHttpCheck;
    QAction *m_actionOpenUrl;
};

CBookmarkMgr *CBookmarkTreeView::mgr() const
{
    return m_mgr;
}

CBookmarkSortFilterProxyModel *CBookmarkTreeView::softFilterProxyModel() const
{
    return m_sortFilterProxyModel;
}

QAction *CBookmarkTreeView::actionBookmarkAdd() const
{
    return m_actionBookmarkAdd;
}

QAction *CBookmarkTreeView::actionBookmarkEdit() const
{
    return m_actionBookmarkEdit;
}

QAction *CBookmarkTreeView::actionBookmarkRemove() const
{
    return m_actionBookmarkRemove;
}


#endif // CBOOKMARKTREEVIEW_H
