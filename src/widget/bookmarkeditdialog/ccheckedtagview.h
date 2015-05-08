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
#ifndef CCHECKEDTAGVIEW_H
#define CCHECKEDTAGVIEW_H

#include <QTreeView>
class CCheckedTagItemModel;
class CTagSortFilterProxyModel;


class CCheckedTagView : public QTreeView
{
public:
    explicit CCheckedTagView(QWidget *parent = 0);
    virtual ~CCheckedTagView();

    inline CCheckedTagItemModel *checkedTagItemModel() const;
    inline CTagSortFilterProxyModel *sortFilterProxyModel() const;
private:
    CCheckedTagItemModel *m_itemModel;
    CTagSortFilterProxyModel *m_sortFilterProxyModel;
};

CCheckedTagItemModel *CCheckedTagView::checkedTagItemModel() const
{
    return m_itemModel;
}

CTagSortFilterProxyModel *CCheckedTagView::sortFilterProxyModel() const
{
    return m_sortFilterProxyModel;
}


#endif // CCHECKEDTAGVIEW_H
