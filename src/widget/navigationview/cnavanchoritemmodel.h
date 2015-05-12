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
#ifndef CNAVANCHORITEMMODEL_H
#define CNAVANCHORITEMMODEL_H

#include <QAbstractItemModel>
#include <QVector>
class INavigationActions;


class CNavAnchorItemModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_FLAGS(AnchorType)
public:
    enum AnchorType
    {
        All             = 0x00,
        Untagged        = 0x01,
        Favorites       = 0x02,
        ReadItLater     = 0x03,
        Rated           = 0x04,
        Trash           = 0x05
    };
public:
    explicit CNavAnchorItemModel(QObject *parent = 0);

    inline INavigationActions *navigationActions() const;
    void setNavigationActions(INavigationActions *interface);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QStringList mimeTypes() const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
private slots:
    void updateAnchorItems();
    void navigationActions_destroyed();
private:
    void initAnchorItems();
    QString anchorName(CNavAnchorItemModel::AnchorType type) const;
    QIcon anchorIcon(CNavAnchorItemModel::AnchorType type) const;
    QList<QStringList> fromMimeTagList(const QMimeData *data);
    QList<QUrl> fromMimeBookmarkList(const QMimeData *data);
private:
    INavigationActions *m_navigationActions;
    QVector<AnchorType> m_anchorItems;
};

INavigationActions *CNavAnchorItemModel::navigationActions() const
{
    return m_navigationActions;
}


#endif // CNAVANCHORITEMMODEL_H
