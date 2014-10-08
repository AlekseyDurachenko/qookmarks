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
#include "cbookmarkfilter.h"
#include "tagutils.h"


CBookmarkFilter::CBookmarkFilter()
{
}

void CBookmarkFilter::setTagFilter(const QSet<CTagItem *> tagFilter)
{
    m_tags = tagFilter;
}

void CBookmarkFilter::setAcceptTypes(const CTagItem::Types &acceptTypes)
{
    m_acceptTypes = acceptTypes;
}

bool CBookmarkFilter::testBookmark(CBookmarkItem *bookmark) const
{
    bool contains = true;
    if (!m_tags.isEmpty())
        contains = tagCheckIntersection(bookmark->tags(), m_tags);

    if (m_acceptTypes & CTagItem::Trash)
        return contains && bookmark->data().isDeleted();

    if (bookmark->data().isDeleted())
        return false;

    if (m_acceptTypes == CTagItem::Tag)
        return contains;

    if (m_acceptTypes & CTagItem::Favorites)
        return contains && bookmark->data().isFavorite();

    if (m_acceptTypes & CTagItem::Rated)
        return contains && (bookmark->data().rating() > 0);

    if (m_acceptTypes & CTagItem::ReadLater)
        return contains && bookmark->data().isReadLater();

    if (m_acceptTypes & CTagItem::Bookmarks)
        return contains && bookmark->tags().isEmpty();

    return false;
}
