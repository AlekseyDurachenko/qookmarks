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
#include "bookmark.h"


Bookmark::Bookmark() : data(new BookmarkData)
{
    data->title = QObject::tr("untitled");
    data->isFavorite = false;
    data->isReadItLater = false;
    data->isTrash = false;
    data->rating = 0;
    data->visitCount = 0;
    data->httpStatusCode = 0;
}

Bookmark::Bookmark(const Bookmark &rhs) : data(rhs.data)
{
}

Bookmark &Bookmark::operator=(const Bookmark &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

Bookmark::~Bookmark()
{
}

void Bookmark::setTitle(const QString &title)
{
    data->title = title;
}

void Bookmark::setUrl(const QUrl &url)
{
    data->url = url;
}

void Bookmark::setDescription(const QString &description)
{
    data->description = description;
}

void Bookmark::setNotes(const QString &notes)
{
    data->notes = notes;
}

void Bookmark::setKeywords(const QString &keywords)
{
    data->keywords = keywords;
}

void Bookmark::setReadItLater(bool state)
{
    data->isReadItLater = state;
}

void Bookmark::setFavorite(bool state)
{
    data->isFavorite = state;
}

void Bookmark::setTrash(bool state)
{
    data->isTrash = state;
}

void Bookmark::setRating(int rating)
{
    data->rating = rating;
}

void Bookmark::setTextColor(const QColor &textColor)
{
    data->textColor = textColor;
}

void Bookmark::setBackgroundColor(const QColor &backgroundColor)
{
    data->backgroundColor = backgroundColor;
}

void Bookmark::setCreatedDateTime(const QDateTime &createdDateTime)
{
    data->createdDateTime = createdDateTime;
}

void Bookmark::setEditedDateTime(const QDateTime &lastEditedDateTime)
{
    data->editedDateTime = lastEditedDateTime;
}

void Bookmark::setLastVisitedDateTime(const QDateTime &lastVisitedDateTime)
{
    data->lastVisitedDateTime = lastVisitedDateTime;
}

void Bookmark::setVisitCount(int visitCount)
{
    data->visitCount = visitCount;
}

void Bookmark::setHttpStatusCode(int httpResponseCode)
{
    data->httpStatusCode = httpResponseCode;
}

void Bookmark::setHttpReasonPhrase(const QString &httpReasonPhrase)
{
    data->httpReasonPhrase = httpReasonPhrase;
}

void Bookmark::setHttpCheckDateTime(const QDateTime &httpCheckDateTime)
{
    data->checkDateTime = httpCheckDateTime;
}
