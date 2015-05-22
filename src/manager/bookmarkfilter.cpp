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
#include "bookmarkfilter.h"
#include "manager.h"
#include "tagmgr.h"
#include "bookmarkmgr.h"
#include "tagitem.h"
#include "bookmarkitem.h"
#include "prj.h"
#include "singleton.h"
#include <QDebug>


BookmarkFilter::BookmarkFilter(QObject *parent) :
    AbstractBookmarkFilter(parent)
{
    m_inclusiveFilter = ~BookmarkFilter::FilterOptions(BookmarkFilter::Trash);
    m_minRating = BookmarkMinRating;
    m_maxRating = BookmarkMaxRating;

    connect(GTagMgr(), SIGNAL(aboutToBeRemoved(TagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeRemoved(TagItem*,int,int)));
}

BookmarkFilter::~BookmarkFilter()
{
}

void BookmarkFilter::setTags(const QSet<TagItem *> &tags)
{
    m_tags = tags;
}

void BookmarkFilter::setTag(TagItem *tag)
{
    m_tags.clear();
    m_tags.insert(tag);
}

void BookmarkFilter::clearTags()
{
    m_tags.clear();
}

void BookmarkFilter::setInclusiveOption(const BookmarkFilter::FilterOptions &options)
{
    m_inclusiveFilter = options;
}

void BookmarkFilter::setRatingRange(int min, int max)
{
    m_minRating = qMin(min, max);
    m_maxRating = qMax(min, max);
}

bool BookmarkFilter::validate(const BookmarkItem *item) const
{
    if (!m_tags.isEmpty())
    {
        if (m_tags.contains(GTagMgr()->rootItem()))
        {
            if (!item->tags().isEmpty())
                return false;
        }
        else if (!TagItem::checkIntersection(m_tags, item->tags()))
        {
            return false;
        }
    }


    if (item->data().rating() < m_minRating
            || item->data().rating() > m_maxRating)
        return false;

    if (m_inclusiveFilter.testFlag(BookmarkFilter::Favorite)
            || m_inclusiveFilter.testFlag(BookmarkFilter::NotFavorite))
        if (!((m_inclusiveFilter.testFlag(BookmarkFilter::Favorite)
             && item->data().isFavorite() == true) ||
                (m_inclusiveFilter.testFlag(BookmarkFilter::NotFavorite)
                 && item->data().isFavorite() == false)))
            return false;

    if (m_inclusiveFilter.testFlag(BookmarkFilter::ReadItLater)
            || m_inclusiveFilter.testFlag(BookmarkFilter::NotReadItLater))
        if (!((m_inclusiveFilter.testFlag(BookmarkFilter::ReadItLater)
             && item->data().isReadItLater() == true) ||
                (m_inclusiveFilter.testFlag(BookmarkFilter::NotReadItLater)
                 && item->data().isReadItLater() == false)))
            return false;

    if (m_inclusiveFilter.testFlag(BookmarkFilter::Trash)
            || m_inclusiveFilter.testFlag(BookmarkFilter::NotTrash))
        if (!((m_inclusiveFilter.testFlag(BookmarkFilter::Trash)
             && item->data().isTrash() == true) ||
                (m_inclusiveFilter.testFlag(BookmarkFilter::NotTrash)
                 && item->data().isTrash() == false)))
            return false;

    return true;
}

void BookmarkFilter::tagMgr_aboutToBeRemoved(TagItem *parent,
        int first, int last)
{
    for (int i = first; i <= last; ++i)
        m_tags.remove(parent->at(i));
}
