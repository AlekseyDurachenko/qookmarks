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
#ifndef CWEBCHECKERQUEUEITEM_H
#define CWEBCHECKERQUEUEITEM_H

#include "cbookmarkitem.h"


class CWebCheckerQueueItem
{
public:
    enum Status {
        Queue       = 0,
        Processing  = 1,
        Done        = 2
    };
public:
    explicit CWebCheckerQueueItem(CBookmarkItem *bookmark = 0,
                                  Status status = Queue);

    inline CBookmarkItem *bookmark() const;
    void setBookmark(CBookmarkItem *bookmark);

    inline Status status() const;
    void setStatus(Status status);
private:
    CBookmarkItem *m_bookmark;
    Status m_status;
};

CBookmarkItem *CWebCheckerQueueItem::bookmark() const
{
    return m_bookmark;
}

CWebCheckerQueueItem::Status CWebCheckerQueueItem::status() const
{
    return m_status;
}


#endif // CWEBCHECKERQUEUEITEM_H
