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
#include "ctagtreeview.h"
#include "ctageditdialog.h"
#include "tagutils.h"
#include <QMenu>
#include <QMessageBox>
#include <QDebug>


static CTagItem *indexToItem(const QModelIndex &index)
{
    return static_cast<CTagItem *>(index.data(Qt::UserRole).value<void *>());
}

CTagTreeView::CTagTreeView(QWidget *parent) :
    QTreeView(parent)
{
    m_mgr = 0;

    // configure drag and drop
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

    // configure context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slot_customContextMenuRequested(QPoint)));

    m_actionTagAdd = new QAction(tr("Add..."), this);
    m_actionTagEdit = new QAction(tr("Edit..."), this);
    m_actionTagRemove = new QAction(tr("Remove..."), this);
    m_actionEmptyFavorites = new QAction(tr("Empty Favorites..."), this);
    m_actionUnrateBookmakrs = new QAction(tr("Unrate bookmakrs..."), this);
    m_actionEmptyReadLater = new QAction(tr("Empty Read Later..."), this);
    m_actionEmptyBookmarks = new QAction(tr("Empty bookmarks..."), this);
    m_actionEmptyTrash = new QAction(tr("Empty Trash..."), this);
    connect(m_actionTagAdd, SIGNAL(triggered()),
            this, SLOT(slot_action_tagAdd_triggered()));
    connect(m_actionTagEdit, SIGNAL(triggered()),
            this, SLOT(slot_action_tagEdit_triggered()));
    connect(m_actionTagRemove, SIGNAL(triggered()),
            this, SLOT(slot_action_tagRemove_triggered()));
    connect(m_actionEmptyFavorites, SIGNAL(triggered()),
            this, SLOT(slot_action_emptyFavorites_tirggered()));
    connect(m_actionUnrateBookmakrs, SIGNAL(triggered()),
            this, SLOT(slot_action_unrateBookmarks_triggered()));
    connect(m_actionEmptyReadLater, SIGNAL(triggered()),
            this, SLOT(slot_action_emptyReadLater_triggered()));
    connect(m_actionEmptyBookmarks, SIGNAL(triggered()),
            this, SLOT(slot_action_emptyBookmakrs_triggered()));
    connect(m_actionEmptyTrash, SIGNAL(triggered()),
            this, SLOT(slot_action_emptyTrash_triggered()));

    m_tagModel = new CTagItemModel(this);
    setModel(m_tagModel);

    updateActions();
}

void CTagTreeView::setBookmarkMgr(CBookmarkMgr *mgr)
{
    if (m_mgr)
    {
        disconnect(m_mgr, 0, this, 0);
        m_tagModel->setRootItem(0);
    }

    m_mgr = mgr;
    if (m_mgr)
    {
        connect(m_mgr, SIGNAL(destroyed()), this, SLOT(slot_mgr_destroyed()));
        m_tagModel->setRootItem(m_mgr->tagRootItem());
    }

    updateActions();
}

CTagItem *CTagTreeView::currentTag() const
{
    if (!currentIndex().isValid())
        return 0;

    return indexToItem(currentIndex());
}

void CTagTreeView::slot_customContextMenuRequested(const QPoint &pos)
{
    CTagItem *tag = currentTag();
    if (!tag)
        return;

    QMenu menu(this);
    switch (tag->type())
    {
    case CTagItem::Bookmarks:
        menu.addAction(m_actionTagAdd);
        menu.addAction(m_actionEmptyBookmarks);
        break;
    case CTagItem::Tag:
        menu.addAction(m_actionTagAdd);
        menu.addAction(m_actionTagEdit);
        menu.addAction(m_actionTagRemove);
        break;
    case CTagItem::Favorites:
        menu.addAction(m_actionEmptyFavorites);
        break;
    case CTagItem::Rated:
        menu.addAction(m_actionUnrateBookmakrs);
        break;
    case CTagItem::ReadLater:
        menu.addAction(m_actionEmptyReadLater);
        break;
    case CTagItem::Trash:
        menu.addAction(m_actionEmptyTrash);
        break;
    default:
        return;
    }

    menu.exec(viewport()->mapToGlobal(pos));
}

void CTagTreeView::slot_mgr_destroyed()
{
    m_mgr = 0;
}

void CTagTreeView::slot_action_tagAdd_triggered()
{
    CTagItem *parentTag = indexToItem(currentIndex());
    CTagEditDialog dlg(CTagEditDialog::New, parentTag, this);
    if (dlg.exec() == QDialog::Accepted)
        m_mgr->tagAdd(parentTag, dlg.toData());
}

void CTagTreeView::slot_action_tagEdit_triggered()
{
    CTagItem *tag = indexToItem(currentIndex());
    CTagEditDialog dlg(CTagEditDialog::Edit, tag->parent(), this);
    dlg.setData(tag->data());
    if (dlg.exec() == QDialog::Accepted)
        tag->setData(dlg.toData());
}

void CTagTreeView::slot_action_tagRemove_triggered()
{
    if (QMessageBox::question(this, tr("Remove the tag"),
            tr("Are you sure you want to remove the tag?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
    {
        CTagItem *tag = static_cast<CTagItem *>
                (currentIndex().data(Qt::UserRole).value<void *>());
        m_mgr->tagRemove(tag);
    }
}

void CTagTreeView::slot_action_emptyFavorites_tirggered()
{
    if (QMessageBox::question(this, tr("Empty Favorites"),
            tr("Are you sure to empty Favorites?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = 0; i < m_mgr->bookmarkCount(); ++i)
    {
        if (m_mgr->bookmarkAt(i)->data().isFavorite())
        {
            CBookmarkItemData data = m_mgr->bookmarkAt(i)->data();
            data.setFavorite(false);
            m_mgr->bookmarkAt(i)->setData(data);
        }
    }

    updateActions();
}

void CTagTreeView::slot_action_unrateBookmarks_triggered()
{
    if (QMessageBox::question(this, tr("Unread Bookmakrs"),
            tr("Are you sure to unread bookmakrs?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = 0; i < m_mgr->bookmarkCount(); ++i)
    {
        if (m_mgr->bookmarkAt(i)->data().rating() > 0)
        {
            CBookmarkItemData data = m_mgr->bookmarkAt(i)->data();
            data.setRating(0);
            m_mgr->bookmarkAt(i)->setData(data);
        }
    }

    updateActions();
}

void CTagTreeView::slot_action_emptyReadLater_triggered()
{
    if (QMessageBox::question(this, tr("Empty Read Later"),
            tr("Are you sure to empty Read Later?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = 0; i < m_mgr->bookmarkCount(); ++i)
    {
        if (m_mgr->bookmarkAt(i)->data().isReadLater())
        {
            CBookmarkItemData data = m_mgr->bookmarkAt(i)->data();
            data.setReadLater(false);
            m_mgr->bookmarkAt(i)->setData(data);
        }
    }

    updateActions();
}

void CTagTreeView::slot_action_emptyBookmakrs_triggered()
{
    if (QMessageBox::question(this, tr("Empty Bookmarks"),
            tr("Are you sure you want to empty bookmakrs?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (CTagItem *tag, m_mgr->tagBookmarksItem()->children())
        m_mgr->tagRemove(tag);
}

void CTagTreeView::slot_action_emptyTrash_triggered()
{
    if (QMessageBox::question(this, tr("Empty Trash"),
            tr("Are you sure to empty Trash?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = 0; i < m_mgr->bookmarkCount(); ++i)
        if (m_mgr->bookmarkAt(i)->data().isDeleted())
            m_mgr->bookmarkRemoveAt(i);

    updateActions();
}

void CTagTreeView::currentChanged(const QModelIndex &/*current*/,
        const QModelIndex & /*previous*/)
{
    updateActions();
    emit currentTagChanged();
}

void CTagTreeView::updateActions()
{
    if (!m_mgr)
    {
        m_actionTagAdd->setEnabled(false);
        m_actionTagEdit->setEnabled(false);
        m_actionTagRemove->setEnabled(false);
        m_actionEmptyTrash->setEnabled(false);
        return;
    }

    CTagItem *tag = currentTag();
    // TODO: disable if <item type root> is empty
    m_actionEmptyFavorites->setEnabled(true);
    m_actionUnrateBookmakrs->setEnabled(true);
    m_actionEmptyReadLater->setEnabled(true);
    m_actionEmptyBookmarks->setEnabled(true);
    m_actionEmptyTrash->setEnabled(true);
    m_actionTagAdd->setEnabled(tag && (tag->type() == CTagItem::Tag
                                       || tag->type() == CTagItem::Bookmarks));
    m_actionTagEdit->setEnabled(tag && tag->type() == CTagItem::Tag);
    m_actionTagRemove->setEnabled(tag && tag->type() == CTagItem::Tag);
}
