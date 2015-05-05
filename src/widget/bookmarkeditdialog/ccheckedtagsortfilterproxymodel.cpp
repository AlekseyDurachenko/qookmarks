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
#include "ccheckedtagsortfilterproxymodel.h"
#include <QDebug>


CCheckedTagSortFilterProxyModel::CCheckedTagSortFilterProxyModel(
        QObject *parent) :QSortFilterProxyModel(parent)
{
}

// this solution from: http://stackoverflow.com/questions/250890/using-qsortfilterproxymodel-with-a-tree-model
bool CCheckedTagSortFilterProxyModel::filterAcceptsRow(int source_row,
        const QModelIndex &source_parent) const
{
    if(filterRegExp().isEmpty() == false)
    {
        QModelIndex source_index = sourceModel()->index(source_row, filterKeyColumn(), source_parent) ;
        if(source_index.isValid())
        {
            int nb = sourceModel()->rowCount(source_index) ;
            for(int i = 0; i < nb; ++i)
                if(filterAcceptsRow(i, source_index))
                    return true ;

            QString key = sourceModel()->data(source_index, filterRole()).toString();
            return key.contains(filterRegExp()) ;
        }
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent) ;
}
