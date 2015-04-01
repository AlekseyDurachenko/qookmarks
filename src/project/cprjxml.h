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
#ifndef CPRJXML_H
#define CPRJXML_H

#include <QDomDocument>
#include <QDomElement>
#include "ctag.h"
#include "cbookmark.h"
class QString;
class QIODevice;
class CManager;
class CBookmarkMgr;
class CBookmarkItem;
class CTagItem;


class CPrjXml
{
public:
    static bool saveEmptyXml(QIODevice *output, QString *reason = 0);
    static bool saveXml(CManager *manager, QIODevice *output, QString *reason = 0);
    static bool loadXml(CManager *manager, QIODevice *input, QString *reason = 0);
private:
    static QDomElement createBookmarkMgrElem(QDomDocument doc, CBookmarkMgr *mgr);
    static QDomElement createBookmarkItemElem(QDomDocument doc, CBookmarkItem *item);
    static QDomElement createBookmarkTagElem(QDomDocument doc, CTagItem *item);
    static QDomElement createTagItemElem(QDomDocument doc, CTagItem *mgr);
    static void parsePrjNode(CManager *manager, QDomNode node);
    static void parseTagNode(CTagItem *parentTag, QDomNode node);
    static void parseBookmarkNode(CManager *manager, QDomNode node);
    static CTag createTagData(QDomElement elem);
    static CBookmark createBookmarkData(QDomElement elem);
    static QSet<QString> createBookmarkKeyworkds(QDomNode node);
    static QStringList createBookmarkTagPath(QDomNode node);
};


#endif // CPRJXML_H
