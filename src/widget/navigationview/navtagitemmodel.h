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
#ifndef NAVTAGITEMMODEL_H
#define NAVTAGITEMMODEL_H

#include "tagitemmodel.h"
class ActInterface;


class NavTagItemModel : public TagItemModel
{
    Q_OBJECT
public:
    explicit NavTagItemModel(QObject *parent = 0);

    inline ActInterface *navigationActions() const;
    void setNavigationActions(ActInterface *navigationActions);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);
#if QT_VERSION >= 0x050000
    virtual Qt::DropActions supportedDropActions() const;
#endif
private slots:
    void navigationActions_destroyed();
private:
    int bookmarkNotTrashedCount(TagItem *item, bool recursive = true) const;
    bool dropMimeTagList(const QMimeData *data,
                         const QStringList &parentTag);
    bool dropMimeBookmarkList(const QMimeData *data,
                              const QStringList &parentTag,
                              Qt::DropAction action);
    QList<QStringList> fromMimeTagList(const QMimeData *data);
    QList<QUrl> fromMimeBookmarkList(const QMimeData *data);
private:
    ActInterface *m_navigationActions;
};

ActInterface *NavTagItemModel::navigationActions() const
{
    return m_navigationActions;
}


#endif // NAVTAGITEMMODEL_H
