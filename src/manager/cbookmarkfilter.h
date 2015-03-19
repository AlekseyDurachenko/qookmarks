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
#ifndef CBOOKMARKFILTER_H
#define CBOOKMARKFILTER_H

#include <QSharedDataPointer>
#include <QSet>
#include "cabstractbookmarkfilter.h"
#include "consts.h"
class CManager;
class CTagItem;


class CBookmarkFilter : public CAbstractBookmarkFilter
{
    Q_OBJECT
public:
    CBookmarkFilter(CManager *manager, QObject *parent = 0);
    virtual ~CBookmarkFilter();

    inline CManager *manager() const;
    void setManager(CManager *manager);

    inline const QSet<CTagItem *> tags() const;
    void setTags(const QSet<CTagItem *> &tags);

    inline const Bookmark::FilterOptions &inclusiveOption() const;
    void setInclusiveOption(const Bookmark::FilterOptions &options);

    inline int minRating() const;
    inline int maxRating() const;
    void setRatingRange(int min, int max);

    virtual bool validate(const CBookmarkItem *item) const;
private slots:
    void tagMgr_aboutToBeRemoved(CTagItem *parent, int first, int last);
    void tagMgr_destroyed();
private:
    CManager *m_manager;
    QSet<CTagItem *> m_tags;
    Bookmark::FilterOptions m_inclusiveFilter;
    int m_minRating;
    int m_maxRating;
};

CManager *CBookmarkFilter::manager() const
{
    return m_manager;
}

const QSet<CTagItem *> CBookmarkFilter::tags() const
{
    return m_tags;
}

const Bookmark::FilterOptions &CBookmarkFilter::inclusiveOption() const
{
    return m_inclusiveFilter;
}

int CBookmarkFilter::minRating() const
{
    return m_minRating;
}

int CBookmarkFilter::maxRating() const
{
    return m_maxRating;
}


#endif // CBOOKMARKFILTER_H
