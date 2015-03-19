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
#ifndef CTAG_H
#define CTAG_H

#include <QSharedDataPointer>
#include <QString>


class CTagData : public QSharedData
{
public:
    QString name;
};


class CTag
{
public:
    CTag();
    CTag(const CTag &);
    CTag &operator=(const CTag &);
    ~CTag();

    inline const QString &name() const;
    void setName(const QString &name);

    inline bool operator ==(const CTag &other);
    inline bool operator !=(const CTag &other);
private:
    QSharedDataPointer<CTagData> data;
};

const QString &CTag::name() const
{
    return data->name;
}

bool CTag::operator ==(const CTag &other)
{
    return (data->name == other.data->name);
}

bool CTag::operator !=(const CTag &other)
{
    return (data->name != other.data->name);
}


#endif // CTAG_H
