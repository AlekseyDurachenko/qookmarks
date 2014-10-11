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
#ifndef CTAGTREEVIEW_H
#define CTAGTREEVIEW_H

#include "ctagitemmodel.h"
#include "cbookmarkmgr.h"
#include <QTreeView>


class CTagTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CTagTreeView(QWidget *parent = 0);

    inline CBookmarkMgr *mgr() const;
    void setBookmarkMgr(CBookmarkMgr *mgr);

    inline QAction *actionTagAdd() const;
    inline QAction *actionTagEdit() const;
    inline QAction *actionTagRemove() const;
    inline QAction *actionEmptyFavorites() const;
    inline QAction *actionUnrateBookmakrs() const;
    inline QAction *actionEmptyReadLater() const;
    inline QAction *actionEmptyBookmakrs() const;
    inline QAction *actionEmptyTrash() const;

    CTagItem *currentTag() const;
signals:
    void currentTagChanged();
private slots:
    void slot_customContextMenuRequested(const QPoint &pos);
    void slot_mgr_destroyed();
    void slot_action_tagAdd_triggered();
    void slot_action_tagEdit_triggered();
    void slot_action_tagRemove_triggered();
    void slot_action_emptyFavorites_tirggered();
    void slot_action_unrateBookmarks_triggered();
    void slot_action_emptyReadLater_triggered();
    void slot_action_emptyBookmakrs_triggered();
    void slot_action_emptyTrash_triggered();
protected:
    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous);
private:
    void updateActions();
private:
    CTagItemModel *m_tagModel;
    CBookmarkMgr *m_mgr;
    QAction *m_actionTagAdd;
    QAction *m_actionTagEdit;
    QAction *m_actionTagRemove;
    QAction *m_actionEmptyFavorites;
    QAction *m_actionUnrateBookmakrs;
    QAction *m_actionEmptyReadLater;
    QAction *m_actionEmptyBookmarks;
    QAction *m_actionEmptyTrash;
};

CBookmarkMgr *CTagTreeView::mgr() const
{
    return m_mgr;
}

QAction *CTagTreeView::actionTagAdd() const
{
    return m_actionTagAdd;
}

QAction *CTagTreeView::actionTagEdit() const
{
    return m_actionTagEdit;
}

QAction *CTagTreeView::actionTagRemove() const
{
    return m_actionTagRemove;
}

QAction *CTagTreeView::actionEmptyFavorites() const
{
    return m_actionEmptyFavorites;
}

QAction *CTagTreeView::actionUnrateBookmakrs() const
{
    return m_actionUnrateBookmakrs;
}

QAction *CTagTreeView::actionEmptyReadLater() const
{
    return m_actionEmptyReadLater;
}

QAction *CTagTreeView::actionEmptyBookmakrs() const
{
    return m_actionEmptyBookmarks;
}

QAction *CTagTreeView::actionEmptyTrash() const
{
    return m_actionEmptyTrash;
}


#endif // CTAGTREEVIEW_H
