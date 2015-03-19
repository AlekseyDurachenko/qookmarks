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


CBookmarkFilter::CBookmarkFilter(CManager *manager, QObject *parent) :
    CAbstractBookmarkFilter(parent)
{
    m_manager = 0;
    m_inclusiveFilter = ~Bookmark::FilterOptions(Bookmark::Trash);
    m_minRating = Bookmark::MinimumRating;
    m_maxRating = Bookmark::MaximumRating;

    setManager(manager);
}

CBookmarkFilter::~CBookmarkFilter()
{
}

void CBookmarkFilter::setManager(CManager *manager)
{
    if (m_manager == manager)
        return;

    if (m_manager != 0)
    {
        disconnect(m_manager->tagMgr(), 0, this, 0);
        disconnect(m_manager->bookmarkMgr(), 0, this, 0);
    }

    m_manager = 0;
    if (m_manager)
    {
        connect(m_manager->tagMgr(), SIGNAL(destroyed()),
                this, SLOT(tagMgr_destroyed()));
        connect(m_manager->tagMgr(), SIGNAL(aboutToBeRemoved(CTagItem*,int,int)),
                this, SLOT(tagMgr_aboutToBeRemoved(CTagItem*,int,int)));
    }

    m_tags.clear();
    update();
}

void CBookmarkFilter::setTags(const QSet<CTagItem *> &tags)
{
    m_tags = tags;
}

void CBookmarkFilter::setInclusiveOption(const Bookmark::FilterOptions &options)
{
    m_inclusiveFilter = options;
}

void CBookmarkFilter::setRatingRange(int min, int max)
{
    m_maxRating = qMin(min, max);
    m_minRating = qMax(min, max);
}

bool CBookmarkFilter::validate(const CBookmarkItem *item) const
{
    if (m_manager == 0)
        return true;


    if (m_tags.isEmpty() == false
            && CTagItem::checkIntersection(m_tags, item->tags()) == false)
        return false;


    if (item->data().rating() < m_minRating
            || item->data().rating() > m_maxRating)
        return false;


    if (m_inclusiveFilter.testFlag(Bookmark::Favorite)
            && item->data().isFavorite() == true)
        return true;

    if (m_inclusiveFilter.testFlag(Bookmark::NotFavorite)
            && item->data().isFavorite() == false)
        return true;

    if (m_inclusiveFilter.testFlag(Bookmark::ReadLater)
            && item->data().isReadLater() == true)
        return true;

    if (m_inclusiveFilter.testFlag(Bookmark::NotReadLater)
            && item->data().isReadLater() == false)
        return true;

    if (m_inclusiveFilter.testFlag(Bookmark::Trash)
            && item->data().isTrash() == true)
        return true;

    if (m_inclusiveFilter.testFlag(Bookmark::NotTrash)
            && item->data().isTrash() == false)
        return true;


    return false;
}

void CBookmarkFilter::tagMgr_aboutToBeRemoved(CTagItem *parent,
        int first, int last)
{
    for (int i = first; i <= last; ++i)
        m_tags.remove(parent->at(i));
}

void CBookmarkFilter::tagMgr_destroyed()
{
    m_manager = 0;
    update();
}
