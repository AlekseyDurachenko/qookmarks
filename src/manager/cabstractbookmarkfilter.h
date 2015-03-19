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
#ifndef CABSTRACTBOOKMARKFILTER_H
#define CABSTRACTBOOKMARKFILTER_H

#include <QObject>
class CBookmarkItem;


class CAbstractBookmarkFilter : public QObject
{
    Q_OBJECT
public:
    explicit CAbstractBookmarkFilter(QObject *parent = 0);
    virtual ~CAbstractBookmarkFilter();

    virtual bool validate(const CBookmarkItem *item) const = 0;
    void update();
signals:
    void changed();
};


#endif // CABSTRACTBOOKMARKFILTER_H
