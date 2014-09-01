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
#ifndef CSTORAGE_H
#define CSTORAGE_H

#include "ctagitem.h"
#include <QtSql>

class CStorage
{
public:
    static bool open(const QString &fileName);
    // returns the last_insert_id, or -1 if failed
    static int insertTag(int parentId, const QString &tagName, CTagItem::Type type);
    static bool updateTagName(int id, const QString &tagName);
    static QSqlQuery selectTags(CTagItem::Type type, int parentId = -1);
    static int insertBookmark(const QString &title, const QString &link);
    static int insertBookmarkTag(int bookmarkId, int tagId);
private:
    static QString m_dbFileName;
    static QSqlDatabase m_db;
};

#endif // CSTORAGE_H
