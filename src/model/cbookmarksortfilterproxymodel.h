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
#ifndef CBOOKMARKSORTFILTERPROXYMODEL_H
#define CBOOKMARKSORTFILTERPROXYMODEL_H

#include "ctagitem.h"
#include <QSortFilterProxyModel>
#include <QSet>


class CBookmarkSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CBookmarkSortFilterProxyModel(QObject *parent = 0);
    inline const QSet<CTagItem *> &tagFilter() const;
public slots:
    void setTagFilter(const QSet<CTagItem *> &tagFilter);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
private:
    QSet<CTagItem *> m_tags;
};

const QSet<CTagItem *> &CBookmarkSortFilterProxyModel::tagFilter() const
{
    return m_tags;
}


#endif // CBOOKMARKSORTFILTERPROXYMODEL_H
