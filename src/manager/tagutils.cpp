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
#include "tagutils.h"
#include "ctagitem.h"


bool tagCheckIntersection(const QSet<CTagItem *> &a, const QSet<CTagItem *> &b)
{
    if (a.count() < b.count()) // optimization
    {
        foreach (CTagItem *tag, a)
            if (b.contains(tag))
                return true;
    }
    else
    {
        foreach (CTagItem *tag, b)
            if (a.contains(tag))
                return true;
    }

    return false;
}


QList<CTagItem *> tagRecursiveFetch(CTagItem *root, bool useRoot)
{
    QList<CTagItem *> tagList;

    foreach (CTagItem *item, root->children())
        tagList << tagRecursiveFetch(item, true);

    if (useRoot)
        tagList << root;

    return tagList;
}
