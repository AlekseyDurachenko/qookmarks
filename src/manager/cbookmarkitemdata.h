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

    inline bool operator == (const CBookmarkItemData &other);
    inline bool operator != (const CBookmarkItemData &other);   
private:
    QIcon m_favicon;
    QByteArray m_faviconHash;   // used for quick comparison
    QString m_title;
    QUrl m_url;
    QString m_description;

    QString m_comment;
    QStringList m_keywordList;
    bool m_isReadLater;
    bool m_isFavorite;
    bool m_isDeleted;
    int m_rating;
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
            && m_rating == other.m_rating);
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
            || m_rating != other.m_rating);
}


#endif // CBOOKMARKITEMDATA_H
