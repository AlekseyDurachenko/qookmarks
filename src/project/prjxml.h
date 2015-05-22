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
#ifndef PRJXML_H
#define PRJXML_H

#include "bookmark.h"
#include "tag.h"
#include <QDomDocument>
#include <QDomElement>
class BookmarkItem;
class BookmarkMgr;
class Manager;
class TagItem;
class QIODevice;
class QString;


class PrjXml
{
public:
    static bool saveEmptyXml(QIODevice *output, QString *reason = 0);
    static bool saveXml(Manager *manager, QIODevice *output, QString *reason = 0);
    static bool loadXml(Manager *manager, QIODevice *input, QString *reason = 0);
private:
    static QDomElement createBookmarkMgrElem(QDomDocument doc, BookmarkMgr *mgr);
    static QDomElement createBookmarkItemElem(QDomDocument doc, BookmarkItem *item);
    static QDomElement createBookmarkTagElem(QDomDocument doc, TagItem *item);
    static QDomElement createTagItemElem(QDomDocument doc, TagItem *mgr);
    static void parsePrjNode(Manager *manager, QDomNode node);
    static void parseTagNode(TagItem *parentTag, QDomNode node);
    static void parseBookmarkNode(Manager *manager, QDomNode node);
    static Tag createTagData(QDomElement elem);
    static Bookmark createBookmarkData(QDomElement elem);
    static QStringList createBookmarkTagPath(QDomNode node);
};


#endif // PRJXML_H
