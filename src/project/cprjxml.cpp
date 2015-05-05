// Copyright 2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "cprjxml.h"
#include <QIODevice>
#include <QStack>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"


const char *dtFormat = "yyyy-MM-dd,HH:mm:ss.zzz";

const char *nsDocType   = "QOOKMAKRS-1.0";
const char *nsPrj       = "PROJECT";
const char *nsTagItem       = "TAG";
const char *nsBookmarkMgr   = "BOOKMARKS";
const char *nsBookmarkItem      = "BOOKMARK";
const char *nsBookmarkKeyword   = "KEYWORD";
const char *nsBookmarkTag       = "TAG";
const char *nsBookmarkTagPath       = "PATH";


static QDateTime dateTimeFromString(const QString &str)
{
    QDateTime dateTime = QDateTime::fromString(str, dtFormat);
    dateTime.setTimeSpec(Qt::UTC);
    return dateTime;
}

static QString dateTimeToString(const QDateTime &dt)
{
    return dt.toString(dtFormat);
}

static QColor colorFromString(const QString &str)
{
    if (str.isEmpty())
        return QColor();

    return QColor(str);
}

static QString colorToString(const QColor &color)
{
    if (color.isValid())
        return color.name();
    else
        return "";
}

bool CPrjXml::saveEmptyXml(QIODevice *output, QString *reason)
{
    return saveXml(0, output, reason);
}

bool CPrjXml::saveXml(CManager *manager, QIODevice *output, QString *reason)
{
    try
    {
        QDomDocument doc(nsDocType);
        QDomElement prjElem = doc.createElement(nsPrj);

        if (manager)
        {
            prjElem.appendChild(createTagItemElem(doc, manager->tagMgr()->rootItem()));
            prjElem.appendChild(createBookmarkMgrElem(doc, manager->bookmarkMgr()));
        }
        doc.appendChild(prjElem);

        QByteArray xmlData = doc.toByteArray();
        if (output->write(xmlData) != xmlData.size())
            throw output->errorString();

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

bool CPrjXml::loadXml(CManager *manager, QIODevice *input, QString *reason)
{
    try
    {
        QDomDocument doc(nsDocType);
        if (!doc.setContent(input, reason))
           return false;

        if (doc.documentElement().nodeName() != nsPrj)
            return false;

        parsePrjNode(manager, doc.documentElement().firstChild());

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

QDomElement CPrjXml::createBookmarkMgrElem(QDomDocument doc, CBookmarkMgr *mgr)
{
    QDomElement elem = doc.createElement(nsBookmarkMgr);
    for (int i = 0; i < mgr->count(); ++i)
        elem.appendChild(createBookmarkItemElem(doc, mgr->at(i)));

    return elem;
}

QDomElement CPrjXml::createBookmarkItemElem(QDomDocument doc,
        CBookmarkItem *item)
{
    QDomElement elem = doc.createElement(nsBookmarkItem);
    elem.setAttribute("title",  item->data().title());
    elem.setAttribute("url",    item->data().url().toString());
    elem.setAttribute("desc",   item->data().description());
    elem.setAttribute("note",   item->data().notes());
    elem.setAttribute("readLater",  item->data().isReadItLater());
    elem.setAttribute("favorite",   item->data().isFavorite());
    elem.setAttribute("trash",      item->data().isTrash());
    elem.setAttribute("rating",     item->data().rating());
    elem.setAttribute("textColor",
                      colorToString(item->data().textColor()));
    elem.setAttribute("backgroundColor",
                      colorToString(item->data().backgroundColor()));
    elem.setAttribute("createdDateTime",
                      dateTimeToString(item->data().createdDateTime()));
    elem.setAttribute("editedDateTime",
                      dateTimeToString(item->data().editedDateTime()));
    elem.setAttribute("lastVisitedDateTime",
                      dateTimeToString(item->data().lastVisitedDateTime()));
    elem.setAttribute("visitCount", item->data().visitCount());
    elem.setAttribute("httpResponseCode",   item->data().httpStatusCode());
    elem.setAttribute("httpResponseText",   item->data().httpReasonPhrase());
    elem.setAttribute("lastCheckDateTime",
                      dateTimeToString(item->data().httpCheckDateTime()));

    foreach (const QString &keyword, item->data().keywords())
    {
        QDomElement keywordElem = doc.createElement(nsBookmarkKeyword);
        keywordElem.setAttribute("value", keyword);
        elem.appendChild(keywordElem);
    }

    foreach (CTagItem *tagItem, item->tags())
        elem.appendChild(createBookmarkTagElem(doc, tagItem));

    return elem;
}

QDomElement CPrjXml::createBookmarkTagElem(QDomDocument doc, CTagItem *item)
{
    QDomElement elem = doc.createElement(nsBookmarkTag);
    foreach (const QString &path, item->path())
    {
        QDomElement pathElem = doc.createElement(nsBookmarkTagPath);
        pathElem.setAttribute("value", path);
        elem.appendChild(pathElem);
    }

    return elem;
}

QDomElement CPrjXml::createTagItemElem(QDomDocument doc, CTagItem *item)
{
    QDomElement elem = doc.createElement(nsTagItem);
    elem.setAttribute("name", item->data().name());

    for (int i = 0; i < item->count(); ++i)
        elem.appendChild(createTagItemElem(doc, item->at(i)));

    return elem;
}

void CPrjXml::parsePrjNode(CManager *manager, QDomNode node)
{
    while (!node.isNull())
    {
        if (node.nodeName() == nsTagItem)
        {
            parseTagNode(manager->tagMgr()->rootItem(), node.firstChild());
        }
        else if (node.nodeName() == nsBookmarkMgr)
        {
            QDomNode bookmarkNode = node.firstChild();
            while(!bookmarkNode.isNull())
            {
                if (bookmarkNode.nodeName() == nsBookmarkItem)
                    parseBookmarkNode(manager, bookmarkNode);

                bookmarkNode = bookmarkNode.nextSibling();
            }
        }

        node = node.nextSibling();
    }
}

void CPrjXml::parseTagNode(CTagItem *parentTag, QDomNode node)
{
    while (!node.isNull())
    {
        if (node.nodeName() == nsTagItem)
        {
            CTagItem *tag = parentTag->add(createTagData(node.toElement()));
            parseTagNode(tag, node.firstChild());
        }
        node = node.nextSibling();
    }
}

void CPrjXml::parseBookmarkNode(CManager *manager, QDomNode node)
{
    QDomElement elem = node.toElement();
    CBookmark bookmarkData = createBookmarkData(elem);
    bookmarkData.setKeywords(createBookmarkKeyworkds(node.firstChild()));
    CBookmarkItem *bookmarkItem = manager->bookmarkMgr()->add(bookmarkData);

    node = node.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == nsBookmarkTag)
        {
            QStringList tagPath = createBookmarkTagPath(node.firstChild());
            CTagItem *tagItem = manager->tagMgr()->findByPath(tagPath);
            if (tagItem)
                tagItem->bookmarkAdd(bookmarkItem);
        }
        node = node.nextSibling();
    }
}

CTag CPrjXml::createTagData(QDomElement elem)
{
    CTag tag;
    tag.setName(elem.attribute("name", "unknow"));
    return tag;
}

CBookmark CPrjXml::createBookmarkData(QDomElement elem)
{
    CBookmark bookmark;
    bookmark.setTitle(elem.attribute("title", "untitled"));
    bookmark.setUrl(elem.attribute("url", ""));
    bookmark.setDescription(elem.attribute("desc", ""));
    bookmark.setNotes(elem.attribute("note", ""));
    bookmark.setReadItLater(elem.attribute("readLater", "0").toInt());
    bookmark.setFavorite(elem.attribute("favorite", "0").toInt());
    bookmark.setTrash(elem.attribute("trash", "0").toInt());
    bookmark.setRating(elem.attribute("rating", "0").toInt());
    bookmark.setTextColor(
                colorFromString(elem.attribute("textColor", "")));
    bookmark.setBackgroundColor(
                colorFromString(elem.attribute("backgroundColor", "")));
    bookmark.setCreatedDateTime(
                dateTimeFromString(elem.attribute("createdDateTime", "")));
    bookmark.setEditedDateTime(
                dateTimeFromString(elem.attribute("editedDateTime", "")));
    bookmark.setLastVisitedDateTime(
                dateTimeFromString(elem.attribute("lastVisitedDateTime", "")));
    bookmark.setVisitCount(elem.attribute("visitCount", "0").toInt());
    bookmark.setHttpStatusCode(
                elem.attribute("httpResponseCode", "0").toInt());
    bookmark.setHttpReasonPhrase(elem.attribute("httpResponseText", "0"));
    bookmark.setHttpCheckDateTime(
                dateTimeFromString(elem.attribute("lastCheckDateTime", "")));
    return bookmark;
}

QSet<QString> CPrjXml::createBookmarkKeyworkds(QDomNode node)
{
    QSet<QString> keyworkds;
    while (!node.isNull())
    {
        if (node.nodeName() == nsBookmarkKeyword)
            keyworkds.insert(node.toElement().attribute("value"));

        node = node.nextSibling();
    }

    return keyworkds;
}

QStringList CPrjXml::createBookmarkTagPath(QDomNode node)
{
    QStringList path;
    while (!node.isNull())
    {
        if (node.nodeName() == nsBookmarkTagPath)
            path.push_back(node.toElement().attribute("value"));

        node = node.nextSibling();
    }

    return path;
}
