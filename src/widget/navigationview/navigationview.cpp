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
#include "navigationview.h"
#include "bookmarkitem.h"
#include "bookmarkmgr.h"
#include "manager.h"
#include "prj.h"
#include "tagitem.h"
#include "tagmgr.h"
#include "singleton.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>


NavigationView::NavigationView(QWidget *parent) :
    QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    setContextMenuPolicy(Qt::CustomContextMenu);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
}

NavigationView::~NavigationView()
{

}

void NavigationView::actMoveTags(const QList<QStringList> &tags,
        const QStringList &parentTag)
{
    TagItem *parentItem = GTagMgr()->findByPath(parentTag);
    if (!parentItem)
        return;

    foreach (const QStringList &tag, tags)
    {
        TagItem *item = GTagMgr()->findByPath(tag);
        if (!item || item == parentItem || item->aboveOf(parentItem))
            continue;

        if (item->parent() != parentItem)
            item->moveTo(parentItem);
    }
}

void NavigationView::actSetTag(const QList<QUrl> &bookmarks,
        const QStringList &tag)
{
    TagItem *parentItem = GTagMgr()->findByPath(tag);
    if (!parentItem || parentItem == GTagMgr()->rootItem())
        return;

    QMessageBox msgBox(QMessageBox::Question, tr("Action"), tr("What do you want?"));
    QPushButton *moveButton = msgBox.addButton(tr("Move"), QMessageBox::ActionRole);
    QPushButton *copyButton = msgBox.addButton(tr("Copy"), QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Cancel);

    if (msgBox.exec() == QMessageBox::Cancel)
        return;

    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        if (msgBox.clickedButton() == moveButton)
        {
            foreach (TagItem *item, bookmarkItem->tags())
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

void NavigationView::actFavorite(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *item = GBookmarkMgr()->find(url);
        Bookmark data = item->data();
        data.setFavorite(true);
        item->setData(data);
    }
}

void NavigationView::actReadItLater(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *item = GBookmarkMgr()->find(url);
        Bookmark data = item->data();
        data.setReadItLater(true);
        item->setData(data);
    }
}

void NavigationView::actTrash(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Move to trash"), tr("Are you shure?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            BookmarkItem *item = GBookmarkMgr()->find(url);
            Bookmark data = item->data();
            data.setTrash(true);
            item->setData(data);
        }
    }
}

void NavigationView::actClearTags(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Clear tags"), tr("Are you shure?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
            foreach (TagItem *tagItem, bookmarkItem->tags())
                tagItem->bookmarkRemove(bookmarkItem);
        }
    }
}
