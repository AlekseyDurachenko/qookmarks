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
        foreach(QString statement,
                QString::fromUtf8(file.readAll()).split("---NEXT---"))
        {
            if (!query.exec(statement))
            {
                return false;
            }
        }

        m_db.transaction();
        return true;
    }

    return false;
}

void CStorage::flush()
{
    m_db.commit();
    m_db.transaction();
}

void CStorage::close()
{
    m_db.commit();
    m_db.close();
}

QSqlQuery CStorage::createQuery()
{
    return QSqlQuery(m_db);
}

int CStorage::bookmarkInsert(const CBookmarkItemData &data)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO TBookmark(title, url) VALUES(:title, :url)");
    query.bindValue("title", data.title());
    query.bindValue("url", data.url());
    if (query.exec())
        return query.lastInsertId().toInt();

    return -1;
}

bool CStorage::bookmarkUpdate(int id, const CBookmarkItemData &data)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE TBookmark"
            " SET title = :title, url = :url"
            " WHERE id = :id");
    query.bindValue("title", data.title());
    query.bindValue("url", data.url());
    query.bindValue("id", id);
    return query.exec();
}

bool CStorage::bookmarkDelete(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM TBookmarkTag WHERE TBookmarkId = :id");
    query.bindValue(":id", id);
    if (!query.exec())
        return false;

    query.prepare("DELETE FROM TBookmark WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool CStorage::bookmarkDeleteAll()
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM TBookmarkTag");
    if (!query.exec())
        return false;

    query.prepare("DELETE FROM TBookmark");
    return query.exec();
}

int CStorage::tagInsert(CTagItem::Type type, int parentId,
        const CTagItemData &data)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO TTag(title, type, parentId)"
            " VALUES(:title, :type, :parentid)");
    query.bindValue("title", data.title());
    query.bindValue("type", type);
    query.bindValue("parentid", parentId);
    if (query.exec())
        return query.lastInsertId().toInt();

    return -1;
}

bool CStorage::tagUpdate(int id, int parentId, CTagItem::Type type,
        const CTagItemData &data)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE TTag"
            " SET title = :title, parentId = :parentId, type = :type"
            " WHERE id = :id");
    query.bindValue("title", data.title());
    query.bindValue("parentId", parentId);
    query.bindValue("type", type);
    query.bindValue("id", id);
    return query.exec();
}

QSqlQuery CStorage::bookmarkSelectAll()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, title, url FROM TBookmark");
    query.exec();
    return query;
}

//int CStorage::insertTag(int parentId, const QString &tagName,
//        CTagItem::Type type)
//{
//    QSqlQuery query(m_db);
//    query.prepare("INSERT INTO TTag(parentId, tagName, type) "
//                  "VALUES(:parentid, :tagname, :type)");
//    query.bindValue("parentid", parentId);
//    query.bindValue("tagname", tagName);
//    query.bindValue("type", static_cast<int>(type));
//    if (query.exec())
//    {
//        return query.lastInsertId().toInt();
//    }

//    return -1;
//}

bool CStorage::updateTagName(int id, const QString &tagName)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE TTag SET tagName = :tagName WHERE id = :id");
    query.bindValue("tagname", tagName);
    query.bindValue("id", id);
    return query.exec();
}

//QSqlQuery CStorage::selectTags(CTagItem::Type type, int parentId)
//{
//    QSqlQuery query(m_db);
//    query.prepare("SELECT id, tagName FROM TTag "
//                  "WHERE type = :type and parentId = :parentid");
//    query.bindValue(":type", static_cast<int>(type));
//    query.bindValue(":parentid", parentId);
//    query.exec();
//    return query;
//}

int CStorage::insertBookmark(const QString &title, const QString &link)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO TBookmark(title, link) "
                  "VALUES(:title, :link)");
    query.bindValue("title", title);
    query.bindValue("link", link);
    if (query.exec())
    {
        return query.lastInsertId().toInt();
    }

    return -1;
}

int CStorage::insertBookmarkTag(int bookmarkId, int tagId)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO TBookmarkTag(TBookmarkId, TTagId) "
                  "VALUES(:TBookmarkId, :TTagId)");
    query.bindValue("TBookmarkId", bookmarkId);
    query.bindValue("TTagId", tagId);
    if (query.exec())
    {
        return query.lastInsertId().toInt();
    }

    return -1;
}
