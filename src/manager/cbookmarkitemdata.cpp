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
#include "cbookmarkitemdata.h"
#include <QObject>
#include <QCryptographicHash>


CBookmarkItemData::CBookmarkItemData()
{
    m_title = QObject::tr("untitled");
    m_isFavorite = false;
    m_isReadLater = false;
    m_isDeleted = false;
    m_rating = 0;
    m_visitCount = 0;
    m_httpResponseCode = 0;
}

void CBookmarkItemData::setFavicon(const QIcon &favicon)
{
    m_favicon = favicon;
    if (m_favicon.isNull())
    {
        m_faviconHash = QByteArray();
    }
    else
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(QVariant(m_favicon).toByteArray());
        m_faviconHash = hash.result();
    }
}

void CBookmarkItemData::setTitle(const QString &title)
{
    m_title = title;
}

void CBookmarkItemData::setUrl(const QUrl &url)
{
    m_url = url;
}

void CBookmarkItemData::setDescription(const QString &description)
{
    m_description = description;
}

void CBookmarkItemData::setComment(const QString &comment)
{
    m_comment = comment;
}

void CBookmarkItemData::setKeywordList(const QStringList &keywordList)
{
    m_keywordList = keywordList;
}

void CBookmarkItemData::setReadLater(bool state)
{
    m_isReadLater = state;
}

void CBookmarkItemData::setFavorite(bool state)
{
    m_isFavorite = state;
}

void CBookmarkItemData::setDeleted(bool state)
{
    m_isDeleted = state;
}

void CBookmarkItemData::setRating(int rating)
{
    m_rating = rating;
}

void CBookmarkItemData::setTextFont(const QFont &textFont)
{
    m_textFont = textFont;
}

void CBookmarkItemData::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
}

void CBookmarkItemData::setTextBackgroundColor(
        const QColor &textBackgroundColor)
{
    m_textBackgroundColor = textBackgroundColor;
}

void CBookmarkItemData::setCreatedDateTime(const QDateTime &createdDateTime)
{
    m_createdDateTime = createdDateTime;
}

void CBookmarkItemData::setEditedDateTime(
        const QDateTime &lastEditedDateTime)
{
    m_editedDateTime = lastEditedDateTime;
}

void CBookmarkItemData::setLastVisitedDateTime(
        const QDateTime &lastVisitedDateTime)
{
    m_lastVisitedDateTime = lastVisitedDateTime;
}

void CBookmarkItemData::setVisitCount(int visitCount)
{
    m_visitCount = visitCount;
}

void CBookmarkItemData::setHttpResponseCode(int httpResponseCode)
{
    m_httpResponseCode = httpResponseCode;
}

void CBookmarkItemData::setHttpResponseText(const QString &httpResponseText)
{
    m_httpResponseText = httpResponseText;
}

void CBookmarkItemData::setLastCheckDateTime(const QDateTime &lastCheckDateTime)
{
    m_lastVisitedDateTime = lastCheckDateTime;
}
