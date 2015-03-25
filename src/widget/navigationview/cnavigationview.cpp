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
#include "cmanager.h"
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

void CNavigationView::tagsCopyOrMove(const QList<QStringList> &tags, const QStringList &parentTag)
{

}

void CNavigationView::bookmarksAssignTag(const QList<QUrl> &bookmarks, const QStringList &tag)
{

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
