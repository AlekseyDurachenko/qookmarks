// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef CCHECKEDTAGITEMMODEL_H
#define CCHECKEDTAGITEMMODEL_H

#include "ctagitemmodel.h"


class CCheckedTagItemModel : public CTagItemModel
{
    Q_OBJECT
public:
    explicit CCheckedTagItemModel(QObject *parent = 0);

    inline const QSet<CTagItem *> &checkedTags() const;
    void setCheckedTags(const QSet<CTagItem *> &checkedTags);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QSet<CTagItem *> m_checkedTags;
};

const QSet<CTagItem *> &CCheckedTagItemModel::checkedTags() const
{
    return m_checkedTags;
}


#endif // CCHECKEDTAGITEMMODEL_H
