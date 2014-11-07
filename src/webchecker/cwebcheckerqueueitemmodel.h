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
#ifndef CWEBCHECKERQUEUEITEMMODEL_H
#define CWEBCHECKERQUEUEITEMMODEL_H

#include "cwebcheckerqueuemgr.h"
#include <QAbstractItemModel>


class CWebCheckerQueueItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CWebCheckerQueueItemModel(QObject *parent = 0);
    explicit CWebCheckerQueueItemModel(CWebCheckerQueueMgr *mgr,
                                       QObject *parent = 0);

    inline CWebCheckerQueueMgr *mgr() const;
    void setMgr(CWebCheckerQueueMgr *mgr);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
private slots:
    void slot_mgr_inserted(int first, int last);
    void slot_mgr_removed(int first, int last);
    void slot_mgr_dataChanged(int first, int last);
    void slot_mgr_destroyed();
private:
    CWebCheckerQueueMgr *m_mgr;
};

CWebCheckerQueueMgr *CWebCheckerQueueItemModel::mgr() const
{
    return m_mgr;
}


#endif // CWEBCHECKERQUEUEITEMMODEL_H
