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
#include "prjxml.h"
#include "bookmarkitem.h"
#include "bookmarkmgr.h"
#include "manager.h"
#include "tagitem.h"
#include "tagmgr.h"
#include <QIODevice>
#include <QStack>


const char *dtFormat = "yyyy-MM-dd,HH:mm:ss.zzz";

const char *nsDocType   = "QOOKMAKRS-1.0";
const char *nsPrj       = "PROJECT";
const char *nsTagItem       = "TAG";
const char *nsBookmarkMgr   = "BOOKMARKS";
const char *nsBookmarkItem      = "BOOKMARK";
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

bool PrjXml::saveEmptyXml(QIODevice *output, QString *reason)
{
    return saveXml(0, output, reason);
}

bool PrjXml::saveXml(Manager *manager, QIODevice *output, QString *reason)
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

bool PrjXml::loadXml(Manager *manager, QIODevice *input, QString *reason)
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

QDomElement PrjXml::createBookmarkMgrElem(QDomDocument doc, BookmarkMgr *mgr)
{
    QDomElement elem = doc.createElement(nsBookmarkMgr);
    for (int i = 0; i < mgr->count(); ++i)
        elem.appendChild(createBookmarkItemElem(doc, mgr->at(i)));

    return elem;
}

QDomElement PrjXml::createBookmarkItemElem(QDomDocument doc,
        BookmarkItem *item)
{
    QDomElement elem = doc.createElement(nsBookmarkItem);
    elem.setAttribute("title",  item->data().title());
    elem.setAttribute("url",    item->data().url().toString());
    elem.setAttribute("description",item->data().description());
    elem.setAttribute("keywords",   item->data().keywords());
    elem.setAttribute("notes",      item->data().notes());
    elem.setAttribute("readItLater",item->data().isReadItLater());
    elem.setAttribute("favorite",   item->data().isFavorite());
    elem.setAttribute("trash",      item->data().isTrash());
    elem.setAttribute("rating",     item->data().rating());
    elem.setAttribute("textColor",          colorToString(item->data().textColor()));
    elem.setAttribute("backgroundColor",    colorToString(item->data().backgroundColor()));
    elem.setAttribute("createdDateTime",    dateTimeToString(item->data().createdDateTime()));
    elem.setAttribute("editedDateTime",     dateTimeToString(item->data().editedDateTime()));
    elem.setAttribute("lastVisitedDateTime",dateTimeToString(item->data().lastVisitedDateTime()));
    elem.setAttribute("visitCount",         item->data().visitCount());
    elem.setAttribute("httpStatusCode",     item->data().httpStatusCode());
    elem.setAttribute("httpReasonPhrase",   item->data().httpReasonPhrase());
    elem.setAttribute("httpCheckDateTime",  dateTimeToString(item->data().httpCheckDateTime()));

    foreach (TagItem *tagItem, item->tags())
        elem.appendChild(createBookmarkTagElem(doc, tagItem));

    return elem;
}

QDomElement PrjXml::createBookmarkTagElem(QDomDocument doc, TagItem *item)
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

QDomElement PrjXml::createTagItemElem(QDomDocument doc, TagItem *item)
{
    QDomElement elem = doc.createElement(nsTagItem);
    elem.setAttribute("name", item->data().name());

    for (int i = 0; i < item->count(); ++i)
        elem.appendChild(createTagItemElem(doc, item->at(i)));

    return elem;
}

void PrjXml::parsePrjNode(Manager *manager, QDomNode node)
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

void PrjXml::parseTagNode(TagItem *parentTag, QDomNode node)
{
    while (!node.isNull())
    {
        if (node.nodeName() == nsTagItem)
        {
            TagItem *tag = parentTag->add(createTagData(node.toElement()));
            parseTagNode(tag, node.firstChild());
        }
        node = node.nextSibling();
    }
}

void PrjXml::parseBookmarkNode(Manager *manager, QDomNode node)
{
    QDomElement elem = node.toElement();
    Bookmark bookmarkData = createBookmarkData(elem);
    BookmarkItem *bookmarkItem = manager->bookmarkMgr()->add(bookmarkData);

    node = node.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == nsBookmarkTag)
        {
            QStringList tagPath = createBookmarkTagPath(node.firstChild());
            TagItem *tagItem = manager->tagMgr()->findByPath(tagPath);
            if (tagItem)
                tagItem->bookmarkAdd(bookmarkItem);
        }
        node = node.nextSibling();
    }
}

Tag PrjXml::createTagData(QDomElement elem)
{
    Tag tag;
    tag.setName(elem.attribute("name", "unknow"));
    return tag;
}

Bookmark PrjXml::createBookmarkData(QDomElement elem)
{
    Bookmark bookmark;
    bookmark.setTitle(elem.attribute("title", "untitled"));
    bookmark.setUrl(elem.attribute("url", ""));
    bookmark.setDescription(elem.attribute("description", ""));
    bookmark.setKeywords(elem.attribute("keywords", ""));
    bookmark.setNotes(elem.attribute("notes", ""));
    bookmark.setReadItLater(elem.attribute("readItLater", "0").toInt());
    bookmark.setFavorite(elem.attribute("favorite", "0").toInt());
    bookmark.setTrash(elem.attribute("trash", "0").toInt());
    bookmark.setRating(elem.attribute("rating", "0").toInt());
    bookmark.setTextColor(colorFromString(elem.attribute("textColor", "")));
    bookmark.setBackgroundColor(colorFromString(elem.attribute("backgroundColor", "")));
    bookmark.setCreatedDateTime(dateTimeFromString(elem.attribute("createdDateTime", "")));
    bookmark.setEditedDateTime(dateTimeFromString(elem.attribute("editedDateTime", "")));
    bookmark.setLastVisitedDateTime(dateTimeFromString(elem.attribute("lastVisitedDateTime", "")));
    bookmark.setVisitCount(elem.attribute("visitCount", "0").toInt());
    bookmark.setHttpStatusCode(elem.attribute("httpStatusCode", "0").toInt());
    bookmark.setHttpReasonPhrase(elem.attribute("httpReasonPhrase", "0"));
    bookmark.setHttpCheckDateTime(dateTimeFromString(elem.attribute("httpCheckDateTime", "")));
    return bookmark;
}

QStringList PrjXml::createBookmarkTagPath(QDomNode node)
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
