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
#include "cstorage.h"
#include <QDebug>

QString CStorage::m_dbFileName;
QSqlDatabase CStorage::m_db;

bool CStorage::open(const QString &fileName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(fileName);
    m_dbFileName = fileName;
    if (m_db.open())
    {
        QSqlQuery query(m_db);
        QFile file(":/sql/createtables.sql");
        file.open(QIODevice::ReadOnly);
        return query.exec(file.readAll());
    }

    return false;
}

int CStorage::insertTag(int parentId, const QString &tagName,
        CTagItem::Type type)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO TTag(parentId, tagName, type) "
                  "VALUES(:parentid, :tagname, :type)");
    query.bindValue("parentid", parentId);
    query.bindValue("tagname", tagName);
    query.bindValue("type", static_cast<int>(type));
    if (query.exec())
    {
        return query.lastInsertId().toInt();
    }

    return -1;
}

bool CStorage::updateTagName(int id, const QString &tagName)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE TTag SET tagName = :tagName WHERE id = :id");
    query.bindValue("tagname", tagName);
    query.bindValue("id", id);
    return query.exec();
}
