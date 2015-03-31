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
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include <QDebug>

const char *nsDocType   = "QOOKMAKRS-1.0";
const char *nsPrj       = "PROJECT";
const char *nsTagItem       = "TAG";
const char *nsBookmarkMgr   = "BOOKMARKS";
const char *nsBookmarkItem      = "BOOKMARK";
const char *nsBookmarkTag       = "TAG";
const char *nsBookmarkTagPath       = "PATH";


bool CPrjXml::saveXml(CManager *manager, QIODevice *output, QString *reason)
{
    try
    {
        QDomDocument doc(nsDocType);
        QDomElement prjElem = doc.createElement(nsPrj);

        prjElem.appendChild(createTagItemElem(doc, manager->tagMgr()->rootItem()));
        prjElem.appendChild(createBookmarkMgrElem(doc, manager->bookmarkMgr()));
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
    elem.setAttribute("title", item->data().title());

    foreach (CTagItem *tagItem, item->tags())
        elem.appendChild(createBookmarkTagElem(doc, tagItem));

    return elem;
}

QDomElement CPrjXml::createBookmarkTagElem(QDomDocument doc, CTagItem *item)
{
    QDomElement elem = doc.createElement(nsBookmarkTag);
    foreach (const QString &p, item->path())
    {
        QDomElement tmp = doc.createElement(nsBookmarkTagPath);
        tmp.setAttribute("value", p);
        elem.appendChild(tmp);
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
