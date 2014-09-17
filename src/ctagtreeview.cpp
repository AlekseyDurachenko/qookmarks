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
#include <QMenu>
#include <QMessageBox>
#include <QDebug>


CTagTreeView::CTagTreeView(QWidget *parent) :
    QTreeView(parent)
{
    // configure context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onCustomContextMenuRequested(QPoint)));

    m_actionTagAdd = new QAction(tr("Add..."), this);
    m_actionTagEdit = new QAction(tr("Edit..."), this);
    m_actionTagRemove = new QAction(tr("Remove..."), this);
    connect(m_actionTagAdd, SIGNAL(triggered()),
            this, SLOT(onActionTagAddTriggered()));
    connect(m_actionTagEdit, SIGNAL(triggered()),
            this, SLOT(onActionTagEditTriggered()));
    connect(m_actionTagRemove, SIGNAL(triggered()),
            this, SLOT(onActionTagRemoveTriggered()));

    m_mgr = 0;
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
        connect(m_mgr, SIGNAL(destroyed()), this, SLOT(onMgrDestroyed()));
        m_tagModel->setRootItem(m_mgr->tagRootItem());
    }

    updateActions();
}

void CTagTreeView::onMgrDestroyed()
{
    m_mgr = 0;
}

void CTagTreeView::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionTagAdd);
    menu.addAction(m_actionTagEdit);
    menu.addAction(m_actionTagRemove);
    menu.exec(viewport()->mapToGlobal(pos));
}

void CTagTreeView::onActionTagAddTriggered()
{
    CTagEditDialog dlg(this);
    dlg.setWindowTitle(tr("Create new tag"));
    if (dlg.exec() == QDialog::Accepted)
    {
        CTagItem *tag = static_cast<CTagItem *>
                (currentIndex().data(Qt::UserRole).value<void *>());

        CTagItemData data = dlg.toData();
        if (m_mgr->tagFind(tag, data.name()))
            QMessageBox::warning(this, tr("Warning"),
                    tr("The tag \"%1\" is already exists").arg(data.name()));
        else
            m_mgr->tagAdd(tag, dlg.toData());
    }
}

void CTagTreeView::onActionTagEditTriggered()
{
    CTagItem *tag = static_cast<CTagItem *>
            (currentIndex().data(Qt::UserRole).value<void *>());

    CTagEditDialog dlg(this);
    dlg.setWindowTitle(tr("Edit tag"));
    dlg.setData(tag->data());
    if (dlg.exec() == QDialog::Accepted)
        tag->setData(dlg.toData());
}

void CTagTreeView::onActionTagRemoveTriggered()
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

void CTagTreeView::currentChanged(const QModelIndex &current,
        const QModelIndex & /*previous*/)
{
    CTagItem *tag = static_cast<CTagItem *>
            (current.data(Qt::UserRole).value<void *>());

    emit currentTagChanged(tag);
    emit currentTagChanged(tag->fetchAllSubtags().toSet());

    updateActions();
}

void CTagTreeView::updateActions()
{
    if (m_mgr)
    {
        if (currentIndex().isValid())
        {
            CTagItem *tag = static_cast<CTagItem *>
                    (currentIndex().data(Qt::UserRole).value<void *>());

            m_actionTagAdd->setEnabled(tag->type() == CTagItem::Tag
                                    || tag->type() == CTagItem::Other);
            m_actionTagEdit->setEnabled(tag->type() == CTagItem::Tag);
            m_actionTagRemove->setEnabled(tag->type() == CTagItem::Tag);
        }
        else
        {
            m_actionTagAdd->setEnabled(false);
            m_actionTagEdit->setEnabled(false);
            m_actionTagRemove->setEnabled(false);
        }
    }
    else
    {
        m_actionTagAdd->setEnabled(false);
        m_actionTagEdit->setEnabled(false);
        m_actionTagRemove->setEnabled(false);
    }
}
