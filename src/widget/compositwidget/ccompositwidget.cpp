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
#include "cnavigationview.h"
#include "cbookmarkview.h"
#include "cnavigationitemmodel.h"
#include "cbookmarkfilteritemmodel.h"
#include "cbookmarkfilterdatamodel.h"
#include "cbookmarkfilter.h"
#include "cmanager.h"
#include "ctagmgr.h"


CCompositWidget::CCompositWidget(CManager *manager, QWidget *parent) :
    QWidget(parent)
{
    m_manager = manager;

    m_filter = new CBookmarkFilter(m_manager, this);
    m_dataModel = new CBookmarkFilterDataModel(m_manager, this);
    m_dataModel->setFilter(m_filter);
    m_navigationItemModel = new CNavigationItemModel(m_manager, this);
    m_bookmarkItemModel = new CBookmarkFilteredItemModel(m_dataModel, this);

    m_navigationView = new CNavigationView(m_manager, this);
    m_navigationView->setModel(m_navigationItemModel);
    m_navigationItemModel->setNavigationActions(m_navigationView);
    connect(m_navigationView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(navigation_selection_selectionChanged()));
    m_bookmarkView = new CBookmarkView(this);
    m_bookmarkView->setModel(m_bookmarkItemModel);

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
                m_filter->setInclusiveOption(Bookmark::FilterOptions(
                            Bookmark::Favorite|Bookmark::NotTrash));
                break;
            case CNavigationItemModel::ReadLater:
                m_filter->setInclusiveOption(Bookmark::FilterOptions(
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
            default:
                ;
            }
        }

        m_filter->setTags(tags);
        m_filter->update();

        break; // only first selected item
    }
}
