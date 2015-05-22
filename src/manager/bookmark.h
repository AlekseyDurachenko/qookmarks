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
#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QColor>
#include <QDateTime>
#include <QSharedDataPointer>
#include <QUrl>


class BookmarkData : public QSharedData
{
public:
    // base
    QString title;
    QUrl url;
    QString description;
    // ext
    QString notes;
    QString keywords;
    bool isReadItLater;
    bool isFavorite;
    bool isTrash;
    int rating;
    // style
    QColor textColor;
    QColor backgroundColor;
    // meta
    QDateTime createdDateTime;
    QDateTime editedDateTime;
    QDateTime lastVisitedDateTime;
    int visitCount;
    // status
    int httpStatusCode;
    QString httpReasonPhrase;
    QDateTime checkDateTime;
};


class Bookmark
{
public:
    Bookmark();
    Bookmark(const Bookmark &);
    Bookmark &operator=(const Bookmark &);
    ~Bookmark();

    inline const QString &title() const;
    void setTitle(const QString &title);

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline const QString &description() const;
    void setDescription(const QString &description);

    inline const QString &notes() const;
    void setNotes(const QString &notes);

    inline const QString &keywords() const;
    void setKeywords(const QString &keywords);

    inline bool isReadItLater() const;
    void setReadItLater(bool state);

    inline bool isFavorite() const;
    void setFavorite(bool state);

    inline bool isTrash() const;
    void setTrash(bool state);

    inline int rating() const;
    void setRating(int rating);

    inline const QColor &textColor() const;
    void setTextColor(const QColor &textColor);

    inline const QColor &backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    inline const QDateTime &createdDateTime() const;
    void setCreatedDateTime(const QDateTime &createdDateTime);

    inline const QDateTime &editedDateTime() const;
    void setEditedDateTime(const QDateTime &editedDateTime);

    inline const QDateTime &lastVisitedDateTime() const;
    void setLastVisitedDateTime(const QDateTime &lastVisitedDateTime);

    inline int visitCount() const;
    void setVisitCount(int visitCount);

    inline int httpStatusCode() const;
    void setHttpStatusCode(int httpStatusCode);

    inline const QString &httpReasonPhrase() const;
    void setHttpReasonPhrase(const QString &httpReasonPhrase);

    inline const QDateTime &httpCheckDateTime() const;
    void setHttpCheckDateTime(const QDateTime &httpCheckDateTime);

    inline bool operator ==(const Bookmark &other);
    inline bool operator !=(const Bookmark &other);
private:
    QSharedDataPointer<BookmarkData> data;
};

const QUrl &Bookmark::url() const
{
    return data->url;
}

const QString &Bookmark::title() const
{
    return data->title;
}

const QString &Bookmark::description() const
{
    return data->description;
}

const QString &Bookmark::notes() const
{
    return data->notes;
}

const QString &Bookmark::keywords() const
{
    return data->keywords;
}

bool Bookmark::isReadItLater() const
{
    return data->isReadItLater;
}

bool Bookmark::isFavorite() const
{
    return data->isFavorite;
}

bool Bookmark::isTrash() const
{
    return data->isTrash;
}

int Bookmark::rating() const
{
    return data->rating;
}

const QColor &Bookmark::textColor() const
{
    return data->textColor;
}

const QColor &Bookmark::backgroundColor() const
{
    return data->backgroundColor;
}

const QDateTime &Bookmark::createdDateTime() const
{
    return data->createdDateTime;
}

const QDateTime &Bookmark::editedDateTime() const
{
    return data->editedDateTime;
}

const QDateTime &Bookmark::lastVisitedDateTime() const
{
    return data->lastVisitedDateTime;
}

int Bookmark::visitCount() const
{
    return data->visitCount;
}

int Bookmark::httpStatusCode() const
{
    return data->httpStatusCode;
}

const QString &Bookmark::httpReasonPhrase() const
{
    return data->httpReasonPhrase;
}

const QDateTime &Bookmark::httpCheckDateTime() const
{
    return data->checkDateTime;
}

bool Bookmark::operator ==(const Bookmark &other)
{
    return (data->title == other.data->title
            && data->url == other.data->url
            && data->description == other.data->description
            && data->notes == other.data->notes
            && data->keywords == other.data->keywords
            && data->isReadItLater == other.data->isReadItLater
            && data->isFavorite == other.data->isFavorite
            && data->isTrash == other.data->isTrash
            && data->rating == other.data->rating
            && data->textColor == other.data->textColor
            && data->backgroundColor == other.data->backgroundColor
            && data->createdDateTime == other.data->createdDateTime
            && data->editedDateTime == other.data->editedDateTime
            && data->lastVisitedDateTime == other.data->lastVisitedDateTime
            && data->visitCount == other.data->visitCount
            && data->httpStatusCode == other.data->httpStatusCode
            && data->httpReasonPhrase == other.data->httpReasonPhrase
            && data->checkDateTime == other.data->checkDateTime);
}

bool Bookmark::operator !=(const Bookmark &other)
{
    return (data->title != other.data->title
            || data->url != other.data->url
            || data->description != other.data->description
            || data->notes != other.data->notes
            || data->keywords != other.data->keywords
            || data->isReadItLater != other.data->isReadItLater
            || data->isFavorite != other.data->isFavorite
            || data->isTrash != other.data->isTrash
            || data->rating != other.data->rating
            || data->textColor != other.data->textColor
            || data->backgroundColor != other.data->backgroundColor
            || data->createdDateTime != other.data->createdDateTime
            || data->editedDateTime != other.data->editedDateTime
            || data->lastVisitedDateTime != other.data->lastVisitedDateTime
            || data->visitCount != other.data->visitCount
            || data->httpStatusCode != other.data->httpStatusCode
            || data->httpReasonPhrase != other.data->httpReasonPhrase
            || data->checkDateTime != other.data->checkDateTime);
}


#endif // BOOKMARK_H
