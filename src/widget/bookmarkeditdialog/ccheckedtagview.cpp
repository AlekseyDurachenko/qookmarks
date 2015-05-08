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
#include "ccheckedtagview.h"
#include <QSortFilterProxyModel>
#include "ccheckedtagitemmodel.h"
#include "ctagsortfilterproxymodel.h"


CCheckedTagView::CCheckedTagView(QWidget *parent) : QTreeView(parent)
{
    m_itemModel = new CCheckedTagItemModel(this);
    m_sortFilterProxyModel = new CTagSortFilterProxyModel(this);
    m_sortFilterProxyModel->setSourceModel(m_itemModel);
    m_sortFilterProxyModel->setDynamicSortFilter(true);
    m_sortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    setModel(m_sortFilterProxyModel);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    setHeaderHidden(true);
}

CCheckedTagView::~CCheckedTagView()
{
}
