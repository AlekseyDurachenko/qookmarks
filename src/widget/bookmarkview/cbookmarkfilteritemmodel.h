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
#ifndef CBOOKMARKFILTERITEMMODEL_H
#define CBOOKMARKFILTERITEMMODEL_H

#include <QAbstractItemModel>
class CBookmarkFilterDataModel;


class CBookmarkFilteredItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CBookmarkFilteredItemModel(QObject *parent = 0);
    explicit CBookmarkFilteredItemModel(CBookmarkFilterDataModel *dataModel,
                                        QObject *parent = 0);

    inline CBookmarkFilterDataModel *dataModel() const;
    void setDataModel(CBookmarkFilterDataModel *dataModel);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
private slots:
    void dataModel_aboutToBeInserted(int first, int last);
    void dataModel_inserted(int first, int last);
    void dataModel_aboutToBeRemoved(int first, int last);
    void dataModel_removed(int first, int last);
    void dataModel_dataChanged(int first, int last);
    void dataModel_reseted();
    void dataModel_destroyed();
private:
    CBookmarkFilterDataModel *m_dataModel;
};

CBookmarkFilterDataModel *CBookmarkFilteredItemModel::dataModel() const
{
    return m_dataModel;
}


#endif // CBOOKMARKFILTERITEMMODEL_H
