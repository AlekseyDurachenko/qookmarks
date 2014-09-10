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
#ifndef CBOOKMARKTAGPROXYMODEL_H
#define CBOOKMARKTAGPROXYMODEL_H

#include "ctagitem.h"
#include <QSortFilterProxyModel>
#include <QSet>

class CBookmarkTagProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CBookmarkTagProxyModel(QObject *parent = 0);
    void setTags(const QSet<CTagItem *> &tags);
    void setIncludeChildren(bool state);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
private:
    QSet<CTagItem *> m_tags;
    bool m_includeChildren;
};

#endif // CBOOKMARKTAGPROXYMODEL_H
