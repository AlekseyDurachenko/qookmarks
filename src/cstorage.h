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

#include "cbookmarkitemdata.h"
#include "ctagitemdata.h"
#include "ctagitem.h"
#include <QtSql>

class CStorage
{
public:
    static bool open(const QString &fileName);
    static void flush();
    static void close();

    static QSqlQuery createQuery();

    static int bookmarkInsert(const CBookmarkItemData &data);
    static bool bookmarkUpdate(int id, const CBookmarkItemData &data);
    static bool bookmarkDelete(int id);
    static bool bookmarkDeleteAll();

    static int tagInsert(CTagItem::Type type, int parentId,
            const CTagItemData &data);
    static bool tagUpdate(int id, int parentId, CTagItem::Type type,
            const CTagItemData &data);
private:
    static QString m_dbFileName;
    static QSqlDatabase m_db;
};

#endif // CSTORAGE_H
