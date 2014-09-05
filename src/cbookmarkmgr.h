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
#ifndef CBOOKMARKMGR_H
#define CBOOKMARKMGR_H

#include "cbookmarkitem.h"
#include "ctagitem.h"
#include <QObject>

class CBookmarkMgr : public QObject
{
    Q_OBJECT
public:
    explicit CBookmarkMgr(QObject *parent = 0);
    virtual ~CBookmarkMgr();

    inline CBookmarkItem *bookmarkRoot() const;
    inline CTagItem *tagRoot() const;
signals:
    void tagInserted(CTagItem *parent, int first, int last);
    void tagRemoved(CTagItem *parent, int first, int last);
    void tagDataChanged(CTagItem *parent, int first, int last);

    void bookmarkInserted(CBookmarkItem *parent, int first, int last);
    void bookmarkRemoved(CBookmarkItem *parent, int first, int last);
    void bookmarkDataChanged(CBookmarkItem *parent, int first, int last);
public slots:
private:
    CBookmarkItem *m_bookmarkRoot;
    CTagItem *m_tagRoot;
};

CBookmarkItem *CBookmarkMgr::bookmarkRoot() const
{
    return m_bookmarkRoot;
}

CTagItem *CBookmarkMgr::tagRoot() const
{
    return m_tagRoot;
}

#endif // CBOOKMARKMGR_H
