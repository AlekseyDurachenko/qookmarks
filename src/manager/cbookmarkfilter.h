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
#ifndef CBOOKMARKFILTER_H
#define CBOOKMARKFILTER_H

#include "cbookmarkitem.h"
#include "ctagitem.h"


class CBookmarkFilter
{
public:
    CBookmarkFilter();

    inline const QSet<CTagItem *> &tagFilter() const;
    void setTagFilter(const QSet<CTagItem *> tagFilter);

    inline const CTagItem::Types &acceptTypes() const;
    void setAcceptTypes(const CTagItem::Types &acceptTypes);

    bool testBookmark(CBookmarkItem *bookmark) const;

    inline bool operator == (const CBookmarkFilter &other);
    inline bool operator != (const CBookmarkFilter &other);
private:
    QSet<CTagItem *> m_tags;
    CTagItem::Types m_acceptTypes;
};


const QSet<CTagItem *> &CBookmarkFilter::tagFilter() const
{
    return m_tags;
}

const CTagItem::Types &CBookmarkFilter::acceptTypes() const
{
    return m_acceptTypes;
}

bool CBookmarkFilter::operator == (const CBookmarkFilter &other)
{
    return (m_tags == other.m_tags
            && m_acceptTypes == other.m_acceptTypes);
}

bool CBookmarkFilter::operator != (const CBookmarkFilter &other)
{
    return (m_tags != other.m_tags
            || m_acceptTypes != other.m_acceptTypes);
}


#endif // CBOOKMARKFILTER_H
