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
#include "ccompositwidget.h"
#include <QHBoxLayout>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include "cnavigationview.h"
#include "cbookmarkview.h"
#include "cnavigationitemmodel.h"
#include "cbookmarkfilter.h"
#include "cbookmarkfilteritemmodel.h"
#include "cbookmarkfilterdatamodel.h"
#include "cmanager.h"
#include "ctagmgr.h"
#include "cbookmarkitem.h"
#include "cbookmarkmgr.h"


CCompositWidget::CCompositWidget(CManager *manager, QWidget *parent) :
    QWidget(parent)
{
    m_manager = manager;

    m_filter = new CBookmarkFilter(m_manager, this);
    m_dataModel = new CBookmarkFilterDataModel(m_manager, this);
    m_dataModel->setFilter(m_filter);
    m_bookmarkItemModel = new CBookmarkFilteredItemModel(m_dataModel, this);
    m_bookmarkView = new CBookmarkView(this);
    m_bookmarkView->setModel(m_bookmarkItemModel);
    connect(m_bookmarkView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bookmarkView_showContextMenu(QPoint)));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateActions()));


    m_navigationItemModel = new CNavigationItemModel(m_manager, this);
    m_navigationView = new CNavigationView(m_manager, this);
    m_navigationView->setModel(m_navigationItemModel);
    m_navigationItemModel->setNavigationActions(m_navigationView);
    connect(m_navigationView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(navigation_selection_selectionChanged()));

    m_actionBookmarkAdd = new QAction(tr("Add..."), this);
    m_actionBookmarkEdit = new QAction(tr("Edit..."), this);
    m_actionBookmarkRemove = new QAction(tr("Remove..."), this);

    connect(m_actionBookmarkAdd, SIGNAL(triggered()),
            this, SLOT(actionBookmarkAdd_triggered()));
    connect(m_actionBookmarkEdit, SIGNAL(triggered()),
            this, SLOT(actionBookmarkEdit_triggered()));
    connect(m_actionBookmarkRemove, SIGNAL(triggered()),
            this, SLOT(actionBookmarkRemove_triggered()));

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_navigationView);
    splitter->addWidget(m_bookmarkView);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(splitter);
    setLayout(hbox);
}

CCompositWidget::~CCompositWidget()
{
}

void CCompositWidget::navigation_selection_selectionChanged()
{
    foreach (const QModelIndex &index,
             m_navigationView->selectionModel()->selectedRows())
    {
        m_filter->setInclusiveOption(~Bookmark::FilterOptions(Bookmark::Trash));
        m_filter->setRatingRange(Bookmark::MinRating, Bookmark::MaxRating);

        QSet<CTagItem *> tags;
        if (index.internalPointer()
                && index.internalPointer() != m_manager->tagMgr()->rootItem())
        {
            tags.insert(static_cast<CTagItem *>(index.internalPointer()));
        }
        else
        {
            CNavigationItemModel::TopLevelItem type =
                    static_cast<CNavigationItemModel::TopLevelItem>(
                        index.data(Qt::UserRole).toInt());

            switch (type)
            {
            case CNavigationItemModel::Favorites:
                m_filter->setInclusiveOption(
                            Bookmark::FilterOptions(
                                Bookmark::Favorite|Bookmark::NotTrash));
                break;
            case CNavigationItemModel::ReadLater:
                m_filter->setInclusiveOption(
                            Bookmark::FilterOptions(
                                Bookmark::ReadLater|Bookmark::NotTrash));
                break;
            case CNavigationItemModel::Rated:
                m_filter->setRatingRange(
                            Bookmark::MinRating+1, Bookmark::MaxRating);
                break;
            case CNavigationItemModel::Trash:
                m_filter->setInclusiveOption(
                            Bookmark::FilterOptions(Bookmark::Trash));
                break;
            case CNavigationItemModel::Untagged:
                tags.insert(m_manager->tagMgr()->rootItem());
                break;
            default:
                ;
            }
        }

        m_filter->setTags(tags);
        m_filter->update();

        break; // only first selected item
    }
}

void CCompositWidget::actionBookmarkAdd_triggered()
{

}

void CCompositWidget::actionBookmarkEdit_triggered()
{

}

void CCompositWidget::actionBookmarkRemove_triggered()
{
    if (QMessageBox::question(this, tr("Question"), tr("Remove bookmark(s)?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    QList<int> bookmarkIndexes;
    foreach (const QModelIndex &index,
             m_bookmarkView->selectionModel()->selectedRows())
    {
        CBookmarkItem *item = reinterpret_cast<CBookmarkItem *>(
                    index.internalPointer());
        CBookmark data = item->data();
        data.setTrash(true);
        item->setData(data);
    }
}

void CCompositWidget::bookmarkView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionBookmarkAdd);
    menu.addAction(m_actionBookmarkEdit);
    menu.addAction(m_actionBookmarkRemove);
    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
}

void CCompositWidget::updateActions()
{
    int selectedCount = m_bookmarkView->selectionModel()->selectedRows().count();
    m_actionBookmarkEdit->setEnabled((selectedCount == 1));
    m_actionBookmarkRemove->setEnabled(selectedCount);
}
