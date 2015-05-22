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
#include "cbookmarkfilter.h"
#include "cmanager.h"
#include "ctagmgr.h"
#include "cbookmarkmgr.h"
#include "ctagitem.h"
#include "cbookmarkitem.h"
#include "prj.h"
#include "singleton.h"
#include <QDebug>


CBookmarkFilter::CBookmarkFilter(QObject *parent) :
    CAbstractBookmarkFilter(parent)
{
    m_inclusiveFilter = ~CBookmarkFilter::FilterOptions(CBookmarkFilter::Trash);
    m_minRating = Bookmark::MinRating;
    m_maxRating = Bookmark::MaxRating;

    connect(GTagMgr(), SIGNAL(aboutToBeRemoved(CTagItem*,int,int)),
            this, SLOT(tagMgr_aboutToBeRemoved(CTagItem*,int,int)));
}

CBookmarkFilter::~CBookmarkFilter()
{
}

void CBookmarkFilter::setTags(const QSet<CTagItem *> &tags)
{
    m_tags = tags;
}

void CBookmarkFilter::setTag(CTagItem *tag)
{
    m_tags.clear();
    m_tags.insert(tag);
}

void CBookmarkFilter::clearTags()
{
    m_tags.clear();
}

void CBookmarkFilter::setInclusiveOption(const CBookmarkFilter::FilterOptions &options)
{
    m_inclusiveFilter = options;
}

void CBookmarkFilter::setRatingRange(int min, int max)
{
    m_minRating = qMin(min, max);
    m_maxRating = qMax(min, max);
}

bool CBookmarkFilter::validate(const CBookmarkItem *item) const
{
    if (!m_tags.isEmpty())
    {
        if (m_tags.contains(GTagMgr()->rootItem()))
        {
            if (!item->tags().isEmpty())
                return false;
        }
        else if (!CTagItem::checkIntersection(m_tags, item->tags()))
        {
            return false;
        }
    }


    if (item->data().rating() < m_minRating
            || item->data().rating() > m_maxRating)
        return false;

    if (m_inclusiveFilter.testFlag(CBookmarkFilter::Favorite)
            || m_inclusiveFilter.testFlag(CBookmarkFilter::NotFavorite))
        if (!((m_inclusiveFilter.testFlag(CBookmarkFilter::Favorite)
             && item->data().isFavorite() == true) ||
                (m_inclusiveFilter.testFlag(CBookmarkFilter::NotFavorite)
                 && item->data().isFavorite() == false)))
            return false;

    if (m_inclusiveFilter.testFlag(CBookmarkFilter::ReadItLater)
            || m_inclusiveFilter.testFlag(CBookmarkFilter::NotReadItLater))
        if (!((m_inclusiveFilter.testFlag(CBookmarkFilter::ReadItLater)
             && item->data().isReadItLater() == true) ||
                (m_inclusiveFilter.testFlag(CBookmarkFilter::NotReadItLater)
                 && item->data().isReadItLater() == false)))
            return false;

    if (m_inclusiveFilter.testFlag(CBookmarkFilter::Trash)
            || m_inclusiveFilter.testFlag(CBookmarkFilter::NotTrash))
        if (!((m_inclusiveFilter.testFlag(CBookmarkFilter::Trash)
             && item->data().isTrash() == true) ||
                (m_inclusiveFilter.testFlag(CBookmarkFilter::NotTrash)
                 && item->data().isTrash() == false)))
            return false;

    return true;
}

void CBookmarkFilter::tagMgr_aboutToBeRemoved(CTagItem *parent,
        int first, int last)
{
    for (int i = first; i <= last; ++i)
        m_tags.remove(parent->at(i));
}
