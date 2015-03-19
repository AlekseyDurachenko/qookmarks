// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "cbookmark.h"


CBookmark::CBookmark() : data(new CBookmarkData)
{
    data->title = QObject::tr("untitled");
    data->isFavorite = false;
    data->isReadLater = false;
    data->isTrash = false;
    data->rating = 0;
    data->visitCount = 0;
    data->httpResponseCode = 0;
}

CBookmark::CBookmark(const CBookmark &rhs) : data(rhs.data)
{
}

CBookmark &CBookmark::operator=(const CBookmark &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

CBookmark::~CBookmark()
{
}

void CBookmark::setTitle(const QString &title)
{
    data->title = title;
}

void CBookmark::setUrl(const QUrl &url)
{
    data->url = url;
}

void CBookmark::setDescription(const QString &description)
{
    data->description = description;
}

void CBookmark::setNote(const QString &note)
{
    data->note = note;
}

void CBookmark::setKeywords(const QSet<QString> &keywords)
{
    data->keywords = keywords;
}

void CBookmark::setReadLater(bool state)
{
    data->isReadLater = state;
}

void CBookmark::setFavorite(bool state)
{
    data->isFavorite = state;
}

void CBookmark::setTrash(bool state)
{
    data->isTrash = state;
}

void CBookmark::setRating(int rating)
{
    data->rating = rating;
}

void CBookmark::setTextColor(const QColor &textColor)
{
    data->textColor = textColor;
}

void CBookmark::setBackgroundColor(const QColor &backgroundColor)
{
    data->backgroundColor = backgroundColor;
}

void CBookmark::setCreatedDateTime(const QDateTime &createdDateTime)
{
    data->createdDateTime = createdDateTime;
}

void CBookmark::setEditedDateTime(const QDateTime &lastEditedDateTime)
{
    data->editedDateTime = lastEditedDateTime;
}

void CBookmark::setLastVisitedDateTime(const QDateTime &lastVisitedDateTime)
{
    data->lastVisitedDateTime = lastVisitedDateTime;
}

void CBookmark::setVisitCount(int visitCount)
{
    data->visitCount = visitCount;
}

void CBookmark::setHttpResponseCode(int httpResponseCode)
{
    data->httpResponseCode = httpResponseCode;
}

void CBookmark::setHttpResponseText(const QString &httpResponseText)
{
    data->httpResponseText = httpResponseText;
}

void CBookmark::setLastCheckDateTime(const QDateTime &lastCheckDateTime)
{
    data->lastCheckDateTime = lastCheckDateTime;
}
