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
#ifndef BOOKMARKFILTER_H
#define BOOKMARKFILTER_H

#include "abstractbookmarkfilter.h"
#include "consts.h"
#include <QSet>
#include <QSharedDataPointer>
class Manager;
class TagItem;


class BookmarkFilter : public AbstractBookmarkFilter
{
    Q_OBJECT
public:
    enum FilterOption
    {
        Any             = 0xFFFF,
        ReadItLater     = 0x0001,
        NotReadItLater  = 0x0100,
        Favorite        = 0x0002,
        NotFavorite     = 0x0200,
        Trash           = 0x0004,
        NotTrash        = 0x0400
    };
    Q_DECLARE_FLAGS(FilterOptions, FilterOption)

    BookmarkFilter(QObject *parent = 0);
    virtual ~BookmarkFilter();

    inline const QSet<TagItem *> tags() const;
    void setTags(const QSet<TagItem *> &tags);
    void setTag(TagItem *tag);
    void clearTags();

    inline const FilterOptions &inclusiveOption() const;
    void setInclusiveOption(const BookmarkFilter::FilterOptions &options);

    inline int minRating() const;
    inline int maxRating() const;
    void setRatingRange(int min, int max);

    virtual bool validate(const BookmarkItem *item) const;
private slots:
    void tagMgr_aboutToBeRemoved(TagItem *parent, int first, int last);
private:
    QSet<TagItem *> m_tags;
    FilterOptions m_inclusiveFilter;
    int m_minRating;
    int m_maxRating;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(BookmarkFilter::FilterOptions)


const QSet<TagItem *> BookmarkFilter::tags() const
{
    return m_tags;
}

const BookmarkFilter::FilterOptions &BookmarkFilter::inclusiveOption() const
{
    return m_inclusiveFilter;
}

int BookmarkFilter::minRating() const
{
    return m_minRating;
}

int BookmarkFilter::maxRating() const
{
    return m_maxRating;
}


#endif // BOOKMARKFILTER_H
