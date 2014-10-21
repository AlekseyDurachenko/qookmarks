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
#ifndef CBOOKMARKITEMDATA_H
#define CBOOKMARKITEMDATA_H

#include <QStringList>
#include <QUrl>
#include <QIcon>
#include <QFont>
#include <QDateTime>
#include <QVariant>


class CBookmarkItemData
{
public:
    CBookmarkItemData();

    inline const QIcon &favicon() const;
    void setFavicon(const QIcon &favicon);

    inline const QString &title() const;
    void setTitle(const QString &title);

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline const QString &description() const;
    void setDescription(const QString &description);

    inline const QString &comment() const;
    void setComment(const QString &comment);

    inline const QStringList &keywordList() const;
    void setKeywordList(const QStringList &keywordList);

    inline bool isReadLater() const;
    void setReadLater(bool state);

    inline bool isFavorite() const;
    void setFavorite(bool state);

    inline bool isDeleted() const;
    void setDeleted(bool state);

    inline int rating() const;
    void setRating(int rating);

    inline const QFont &textFont() const;
    void setTextFont(const QFont &textFont);

    inline const QColor &textColor() const;
    void setTextColor(const QColor &textColor);

    inline const QColor &textBackgroundColor() const;
    void setTextBackgroundColor(const QColor &textBackgroundColor);

    inline const QDateTime &createdDateTime() const;
    void setCreatedDateTime(const QDateTime &createdDateTime);

    inline const QDateTime &editedDateTime() const;
    void setEditedDateTime(const QDateTime &editedDateTime);

    inline const QDateTime &lastVisitedDateTime() const;
    void setLastVisitedDateTime(const QDateTime &lastVisitedDateTime);

    inline int visitCount() const;
    void setVisitCount(int visitCount);

    inline int httpResponseCode() const;
    void setHttpResponseCode(int httpResponseCode);

    inline const QString &httpResponseText() const;
    void setHttpResponseText(const QString &httpResponseText);

    inline const QDateTime &lastCheckDateTime() const;
    void setLastCheckDateTime(const QDateTime &lastCheckDateTime);

    inline bool operator == (const CBookmarkItemData &other);
    inline bool operator != (const CBookmarkItemData &other);   
private:
    // base
    QIcon m_favicon;
    QByteArray m_faviconHash;   // used for quick comparison
    QString m_title;
    QUrl m_url;
    QString m_description;
    // ext
    QString m_comment;
    QStringList m_keywordList;
    bool m_isReadLater;
    bool m_isFavorite;
    bool m_isDeleted;
    int m_rating;
    // style
    QFont m_textFont;
    QColor m_textColor;
    QColor m_textBackgroundColor;
    // meta
    QDateTime m_createdDateTime;
    QDateTime m_editedDateTime;
    QDateTime m_lastVisitedDateTime;
    int m_visitCount;
    // status
    int m_httpResponseCode;
    QString m_httpResponseText;
    QDateTime m_lastCheckDateTime;
};

inline const QIcon &CBookmarkItemData::favicon() const
{
    return m_favicon;
}

const QString &CBookmarkItemData::title() const
{
    return m_title;
}

const QUrl &CBookmarkItemData::url() const
{
    return m_url;
}

const QString &CBookmarkItemData::description() const
{
    return m_description;
}

const QString &CBookmarkItemData::comment() const
{
    return m_comment;
}

const QStringList &CBookmarkItemData::keywordList() const
{
    return m_keywordList;
}

bool CBookmarkItemData::isReadLater() const
{
    return m_isReadLater;
}

bool CBookmarkItemData::isFavorite() const
{
    return m_isFavorite;
}

bool CBookmarkItemData::isDeleted() const
{
    return m_isDeleted;
}

int CBookmarkItemData::rating() const
{
    return m_rating;
}

const QFont &CBookmarkItemData::textFont() const
{
    return m_textFont;
}

const QColor &CBookmarkItemData::textColor() const
{
    return m_textColor;
}

const QColor &CBookmarkItemData::textBackgroundColor() const
{
    return m_textBackgroundColor;
}

const QDateTime &CBookmarkItemData::createdDateTime() const
{
    return m_createdDateTime;
}

const QDateTime &CBookmarkItemData::editedDateTime() const
{
    return m_editedDateTime;
}

const QDateTime &CBookmarkItemData::lastVisitedDateTime() const
{
    return m_lastVisitedDateTime;
}

int CBookmarkItemData::visitCount() const
{
    return m_visitCount;
}

int CBookmarkItemData::httpResponseCode() const
{
    return m_httpResponseCode;
}

const QString &CBookmarkItemData::httpResponseText() const
{
    return m_httpResponseText;
}

const QDateTime &CBookmarkItemData::lastCheckDateTime() const
{
    return m_lastCheckDateTime;
}

bool CBookmarkItemData::operator == (const CBookmarkItemData &other)
{
    return (m_faviconHash == other.m_faviconHash
            && m_title == other.m_title
            && m_url == other.m_url
            && m_description == other.m_description
            && m_comment == other.m_comment
            && m_keywordList == other.m_keywordList
            && m_isReadLater == other.m_isReadLater
            && m_isFavorite == other.m_isFavorite
            && m_isDeleted == other.m_isDeleted
            && m_rating == other.m_rating
            && m_textFont == other.m_textFont
            && m_textColor == other.m_textColor
            && m_textBackgroundColor == other.m_textBackgroundColor
            && m_createdDateTime == other.m_createdDateTime
            && m_editedDateTime == other.m_editedDateTime
            && m_lastVisitedDateTime == other.m_lastVisitedDateTime
            && m_visitCount == other.m_visitCount
            && m_httpResponseCode == other.m_httpResponseCode
            && m_httpResponseText == other.m_httpResponseText
            && m_lastCheckDateTime == other.m_lastCheckDateTime);
}

bool CBookmarkItemData::operator != (const CBookmarkItemData &other)
{
    return (m_faviconHash != other.m_faviconHash
            || m_title != other.m_title
            || m_url != other.m_url
            || m_description != other.m_description
            || m_comment != other.m_comment
            || m_keywordList != other.m_keywordList
            || m_isReadLater != other.m_isReadLater
            || m_isFavorite != other.m_isFavorite
            || m_isDeleted != other.m_isDeleted
            || m_rating != other.m_rating
            || m_textFont != other.m_textFont
            || m_textColor != other.m_textColor
            || m_textBackgroundColor != other.m_textBackgroundColor
            || m_createdDateTime != other.m_createdDateTime
            || m_editedDateTime != other.m_editedDateTime
            || m_lastVisitedDateTime != other.m_lastVisitedDateTime
            || m_visitCount != other.m_visitCount
            || m_httpResponseCode != other.m_httpResponseCode
            || m_httpResponseText != other.m_httpResponseText
            || m_lastCheckDateTime != other.m_lastCheckDateTime);
}


#endif // CBOOKMARKITEMDATA_H
