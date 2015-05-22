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
#ifndef CHECKEDTAGITEMMODEL_H
#define CHECKEDTAGITEMMODEL_H

#include "tagitemmodel.h"


class CheckedTagItemModel : public TagItemModel
{
    Q_OBJECT
public:
    explicit CheckedTagItemModel(QObject *parent = 0);

    inline const QSet<TagItem *> &checkedTags() const;
    void setCheckedTags(const QSet<TagItem *> &checkedTags);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
protected slots:
    virtual void tagMgr_aboutToBeRemoved(TagItem *parent, int first, int last);
private:
    QSet<TagItem *> m_checkedTags;
};

const QSet<TagItem *> &CheckedTagItemModel::checkedTags() const
{
    return m_checkedTags;
}


#endif // CHECKEDTAGITEMMODEL_H
