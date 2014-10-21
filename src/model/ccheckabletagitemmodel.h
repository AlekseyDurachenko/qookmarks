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
#ifndef CCHECKABLETAGITEMMODEL_H
#define CCHECKABLETAGITEMMODEL_H

#include "ctagitemmodel.h"


class CCheckableTagItemModel : public CTagItemModel
{
    Q_OBJECT
public:
    explicit CCheckableTagItemModel(QObject *parent = 0);
    explicit CCheckableTagItemModel(CTagItem *rootItem, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    void selectTag(CTagItem *item);
    bool selectTag(CTagItem *item, const QModelIndex &parent);
    QSet<CTagItem *> checkedTags();
private:
    QList <QPersistentModelIndex> m_checkedList;
};


#endif // CCHECKABLETAGITEMMODEL_H
