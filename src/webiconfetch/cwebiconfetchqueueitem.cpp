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
#include "cwebiconfetchqueueitem.h"

CWebIconFetchQueueItem::CWebIconFetchQueueItem(CBookmarkItem *bookmark,
        Status status)
{
    m_bookmark = bookmark;
    m_status = status;
}


void CWebIconFetchQueueItem::setBookmark(CBookmarkItem *bookmark)
{
    m_bookmark = bookmark;
}

void CWebIconFetchQueueItem::setStatus(CWebIconFetchQueueItem::Status status)
{
    m_status = status;
}
