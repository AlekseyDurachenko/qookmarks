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
#include "cbookmarktreeview.h"
#include "cbookmarkeditdialog.h"
#include "cbookmarkreadlaterdelegate.h"
#include "cbookmarkfavoritesdelegate.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>


static CBookmarkItem *indexToItem(const QModelIndex &index)
{
    return static_cast<CBookmarkItem *>(index.data(Qt::UserRole).value<void *>());
}

CBookmarkTreeView::CBookmarkTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setRootIsDecorated(false);
    setSortingEnabled(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);

    // configure context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onCustomContextMenuRequested(QPoint)));

    // delegates
    setItemDelegateForColumn(5, new CBookmarkReadLaterDelegate(this));
    setItemDelegateForColumn(6, new CBookmarkFavoritesDelegate(this));

    m_actionBookmarkAdd = new QAction(tr("Add..."), this);
    m_actionBookmarkEdit = new QAction(tr("Edit..."), this);
    m_actionBookmarkRemove = new QAction(tr("Remove..."), this);
    connect(m_actionBookmarkAdd, SIGNAL(triggered()),
            this, SLOT(onActionBookmarkAddTriggered()));
    connect(m_actionBookmarkEdit, SIGNAL(triggered()),
            this, SLOT(onActionBookmarkEditTriggered()));
    connect(m_actionBookmarkRemove, SIGNAL(triggered()),
            this, SLOT(onActionBookmarkRemoveTriggered()));

    m_mgr = 0;
    m_bookmarkModel = new CBookmarkItemModel(this);
    m_softFilterProxyModel = new CBookmarkSortFilterProxyModel(this);
    m_softFilterProxyModel->setSourceModel(m_bookmarkModel);
    m_softFilterProxyModel->setDynamicSortFilter(true);
    setModel(m_softFilterProxyModel);

    updateActions();
}

void CBookmarkTreeView::setBookmarkMgr(CBookmarkMgr *mgr)
{
    if (m_mgr)
    {
        disconnect(m_mgr, 0, this, 0);
        m_bookmarkModel->setBookmarkMgr(0);
    }

    m_mgr = mgr;
    if (m_mgr)
    {
        connect(m_mgr, SIGNAL(destroyed()), this, SLOT(onMgrDestroyed()));
        m_bookmarkModel->setBookmarkMgr(mgr);
    }

    updateActions();
}

int CBookmarkTreeView::sizeHintForColumn(int column) const
{
    // Костыль! да еще и не рабочий!
    if (column == 5)
        return 32;
    if (column == 6)
        return 32;

    return QTreeView::sizeHintForColumn(column);
}

void CBookmarkTreeView::onMgrDestroyed()
{
    m_mgr = 0;
}

void CBookmarkTreeView::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionBookmarkAdd);
    menu.addAction(m_actionBookmarkEdit);
    menu.addAction(m_actionBookmarkRemove);
    menu.exec(viewport()->mapToGlobal(pos));
}

void CBookmarkTreeView::onActionBookmarkAddTriggered()
{
    CBookmarkEditDialog dlg(this);
    dlg.setWindowTitle(tr("Create new bookmark"));
    if (dlg.exec() == QDialog::Accepted)
    {
        CBookmarkItemData data = dlg.toData();
        if (m_mgr->bookmarkFind(data.url()))
            QMessageBox::warning(this, tr("Warning"), tr("The bookmark with "
                    "the url \"%1\" is already exists")
                            .arg(data.url().toString()));
        else
            m_mgr->bookmarkAdd(dlg.toData());
    }
}

void CBookmarkTreeView::onActionBookmarkEditTriggered()
{
    CBookmarkItem *bookmark = static_cast<CBookmarkItem *>
            (currentIndex().data(Qt::UserRole).value<void *>());

    CBookmarkEditDialog dlg(this);
    dlg.setWindowTitle(tr("Edit bookmark"));
    dlg.setData(bookmark->data());
    if (dlg.exec() == QDialog::Accepted)
        bookmark->setData(dlg.toData());
}

void CBookmarkTreeView::onActionBookmarkRemoveTriggered()
{
    if (QMessageBox::question(this, tr("Remove the bookmark"),
            tr("Are you sure you want to remove the selected bookmark(s)?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        QModelIndexList indexList = selectionModel()->selectedRows(0);

        QList<CBookmarkItem *> bookmarkList;
        foreach (QModelIndex index, indexList)
            bookmarkList.push_back(indexToItem(index));

        if (bookmarkList.count() == 0 && currentIndex().isValid())
            bookmarkList.push_back(indexToItem(currentIndex()));

        foreach (CBookmarkItem *bookmark, bookmarkList)
            m_mgr->bookmarkRemove(bookmark);
    }
}

void CBookmarkTreeView::currentChanged(const QModelIndex & /*current*/,
        const QModelIndex & /*previous*/)
{
    updateActions();
    emit currentBookmarkChanged(indexToItem(currentIndex()));
}

void CBookmarkTreeView::updateActions()
{
    if (!m_mgr)
    {
        m_actionBookmarkAdd->setEnabled(false);
        m_actionBookmarkEdit->setEnabled(false);
        m_actionBookmarkRemove->setEnabled(false);
        return;
    }

    QModelIndexList rows = selectionModel()->selectedRows(0);
    m_actionBookmarkAdd->setEnabled(true);
    m_actionBookmarkEdit->setEnabled
            (rows.count() == 1 || (!rows.count() && currentIndex().isValid()));
    m_actionBookmarkRemove->setEnabled
            (rows.count() || (!rows.count() && currentIndex().isValid()));
}