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
#include "ctagitem.h"

CTagItem::CTagItem(Type type, int row, QObject *parent) : QObject(parent)
{
    m_row = row;
    m_type = type;
}

QVariant CTagItem::headerData(int section,
        Qt::Orientation orientation, int role) const
{
}

QVariant CTagItem::data(int column, int role) const
{
}

CTagItem *CTagItem::child(int row)
{
}

void CTagItem::add(CTagItem *item)
{
}
