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

#include <QString>
#include <QUrl>

class CBookmarkItemData
{
public:
    CBookmarkItemData();

    inline const QString &title() const;
    void setTitle(const QString &title);

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline int httpCode() const;
    void setHttpCode(int code);

    inline bool operator == (const CBookmarkItemData &other);
    inline bool operator != (const CBookmarkItemData &other);
private:
    QString m_title;
    QUrl m_url;
    int m_httpCode;
};

const QString &CBookmarkItemData::title() const
{
    return m_title;
}

const QUrl &CBookmarkItemData::url() const
{
    return m_url;
}

int CBookmarkItemData::httpCode() const
{
    return m_httpCode;
}

bool CBookmarkItemData::operator == (const CBookmarkItemData &other)
{
    return (m_title == other.m_title
            && m_url == other.m_url
            && m_httpCode == other.m_httpCode);
}

bool CBookmarkItemData::operator != (const CBookmarkItemData &other)
{
    return (m_title != other.m_title
            || m_url != other.m_url
            || m_httpCode != other.m_httpCode);
}


#endif // CBOOKMARKITEMDATA_H
