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
#ifndef CTAGITEMDATA_H
#define CTAGITEMDATA_H

#include <QString>
#include <QUrl>

class CTagItemData
{
public:
    CTagItemData();

    inline const QString &name() const;
    void setName(const QString &name);
private:
    QString m_name;
};

const QString &CTagItemData::name() const
{
    return m_name;
}


#endif // CTAGITEMDATA_H
