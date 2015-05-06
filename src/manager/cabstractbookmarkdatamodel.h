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
#ifndef CABSTRACTBOOKMARKDATAMODEL_H
#define CABSTRACTBOOKMARKDATAMODEL_H

#include <QObject>
class CBookmarkItem;


class CAbstractBookmarkDataModel : public QObject
{
    Q_OBJECT
public:
    explicit CAbstractBookmarkDataModel(QObject *parent = 0);

    virtual int count() const = 0;
    virtual CBookmarkItem *at(int index) const = 0;
signals:
    void aboutToBeInserted(int first, int last);
    void inserted(int first, int last);
    void aboutToBeRemoved(int first, int last);
    void removed(int first, int last);
    void aboutToBeDataChanged(int first, int last);
    void dataChanged(int first, int last);
    void aboutToBeReseted();
    void reseted();
};


#endif // CABSTRACTBOOKMARKDATAMODEL_H
