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
#include "cnavigationview.h"
#include <QMessageBox>
#include <QPushButton>
#include "cmanager.h"
#include "ctagitem.h"
#include "ctagmgr.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include <QDebug>


CNavigationView::CNavigationView(CManager *manager, QWidget *parent) :
    QTreeView(parent)
{
    m_manager = manager;

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
}

CNavigationView::~CNavigationView()
{

}

void CNavigationView::tagsCopyOrMove(const QList<QStringList> &tags,
        const QStringList &parentTag)
{
    CTagItem *parentItem = m_manager->tagMgr()->findByPath(parentTag);
    if (!parentItem)
        return;

    foreach (const QStringList &tag, tags)
    {
        CTagItem *item = m_manager->tagMgr()->findByPath(tag);
        if (!item || item == parentItem || item->aboveOf(parentItem))
            continue;

        item->moveTo(parentItem);
    }
}

void CNavigationView::bookmarksAssignTag(const QList<QUrl> &bookmarks,
        const QStringList &tag)
{
    CTagItem *parentItem = m_manager->tagMgr()->findByPath(tag);
    if (!parentItem)
        return;

    QMessageBox msgBox(QMessageBox::Question, "Action", "What do you want?");
    QPushButton *moveButton = msgBox.addButton("Move", QMessageBox::ActionRole);
    QPushButton *copyButton = msgBox.addButton("Copy", QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::Cancel)
        return;

    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *bookmarkItem = m_manager->bookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        if (msgBox.clickedButton() == moveButton)
        {
            foreach (CTagItem *item, bookmarkItem->tags())
                if (parentItem != item)
                    item->bookmarkRemove(bookmarkItem);

            parentItem->bookmarkAdd(bookmarkItem);
        }
        else if (msgBox.clickedButton() == copyButton)
        {
            parentItem->bookmarkAdd(bookmarkItem);
        }
    }
}

void CNavigationView::bookmarksMarkFavorite(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *item = m_manager->bookmarkMgr()->find(url);
        CBookmark data = item->data();
        data.setFavorite(true);
        item->setData(data);
    }
}

void CNavigationView::bookmarksMarkReadLater(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *item = m_manager->bookmarkMgr()->find(url);
        CBookmark data = item->data();
        data.setReadLater(true);
        item->setData(data);
    }
}

void CNavigationView::bookmarksMarkTrash(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Move to trash"), tr("Are you shure?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            CBookmarkItem *item = m_manager->bookmarkMgr()->find(url);
            CBookmark data = item->data();
            data.setTrash(true);
            item->setData(data);
        }
    }
}
