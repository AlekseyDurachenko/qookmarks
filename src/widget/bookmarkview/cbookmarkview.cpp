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
#include "cbookmarkview.h"
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include "cbookmarkbooleanicondelegate.h"
#include "cbookmarkratingdelegate.h"
#include "cbookmarkheaderview.h"
#include "cbookmarkitemmodel.h"
#include "cbookmarkitem.h"


CBookmarkView::CBookmarkView(QWidget *parent) :
    QTreeView(parent)
{
    setSortingEnabled(true);
    setRootIsDecorated(false);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::DragOnly);

    // first bookmark item model is not setted
    m_bookmarkItemModel = 0;

    // for sort and quick filter
    m_sortFilterProxyModel = new QSortFilterProxyModel(this);
    m_sortFilterProxyModel->setDynamicSortFilter(true);
    m_sortFilterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    setModel(m_sortFilterProxyModel);

    // header view
    setHeader(new CBookmarkHeaderView(Qt::Horizontal, this));
    header()->setMinimumSectionSize(10);
#if QT_VERSION >= 0x050000
    header()->setSectionsMovable(true);
#else
    header()->setMovable(true);
#endif

    // rating
    setItemDelegateForColumn(4, new CBookmarkRatingDelegate(this));

    // favorite
    CBookmarkBooleanIconDelegate *favoriteItemDelegate = new CBookmarkBooleanIconDelegate(this);
    favoriteItemDelegate->setIconOn(QIcon(":/icons/bookmark-favorites.png"));
    favoriteItemDelegate->setIconOff(QIcon(":/icons/bookmark-noflag.png"));
    setItemDelegateForColumn(5, favoriteItemDelegate);

    // read it later
    CBookmarkBooleanIconDelegate *readItLaterItemDelegate = new CBookmarkBooleanIconDelegate(this);
    readItLaterItemDelegate->setIconOn(QIcon(":/icons/bookmark-readlater.png"));
    readItLaterItemDelegate->setIconOff(QIcon(":/icons/bookmark-noflag.png"));
    setItemDelegateForColumn(6, readItLaterItemDelegate);

    // trash
    CBookmarkBooleanIconDelegate *trashItemDelegate = new CBookmarkBooleanIconDelegate(this);
    trashItemDelegate->setIconOn(QIcon(":/icons/bookmark-trash.png"));
    trashItemDelegate->setIconOff(QIcon(":/icons/bookmark-noflag.png"));
    setItemDelegateForColumn(7, trashItemDelegate);
}

void CBookmarkView::setBookmarkModel(CBookmarkItemModel *model)
{
    m_bookmarkItemModel = model;
    m_sortFilterProxyModel->setSourceModel(model);

#if QT_VERSION >= 0x050000
    header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
#else
    header()->setResizeMode(4, QHeaderView::ResizeToContents);
    header()->setResizeMode(5, QHeaderView::ResizeToContents);
    header()->setResizeMode(6, QHeaderView::ResizeToContents);
    header()->setResizeMode(7, QHeaderView::ResizeToContents);
#endif
}

QList<CBookmarkItem *> CBookmarkView::selectedBookmarks() const
{
    QList<CBookmarkItem *> bookmarks;
    foreach (const QModelIndex &index, selectionModel()->selectedRows())
        bookmarks << CBookmarkItem::variantToPtr(index.data(Qt::UserRole));

    return bookmarks;
}

void CBookmarkView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
}
