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
#ifndef TAGUTILS_H
#define TAGUTILS_H

#include <QSet>
class CTagItem;


/*!
 * \brief tagCheckIntersection check the intersection between two sets
 * \param a set one
 * \param b set two
 * \retval true if intersection exists
 * \retval otherwise
 */
bool tagCheckIntersection(const QSet<CTagItem *> &a, const QSet<CTagItem *> &b);


/*!
 * \brief tagRecursiveFetch fetch children recursive from the tag
 * \param root the root item
 * \param useRoot add the root item to the result
 * \return list of tags
 */
QList<CTagItem *> tagRecursiveFetch(CTagItem *root, bool useRoot = true);


#endif // TAGUTILS_H
