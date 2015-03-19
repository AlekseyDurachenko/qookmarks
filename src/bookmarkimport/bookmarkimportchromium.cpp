// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "bookmarkimportchromium.h"
#include <json.h>
#include <QFile>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"


static void addUrl(CManager *mgr, CTagItem *parent, const QVariantMap &map)
{
    CBookmark data;
    data.setTitle(map.value("name").toString());
    data.setUrl(map.value("url").toString());

    CBookmarkItem *bookmark = mgr->bookmarkMgr()->find(data.url());
    if (!bookmark)
        bookmark = mgr->bookmarkMgr()->add(data);

    if (parent != mgr->tagMgr()->rootItem())
        parent->bookmarkAdd(bookmark);
}


static CTagItem *findOrAddTag(CTagItem *parent, const QVariantMap &map)
{
    CTagItem *item = parent->find(map.value("name").toString());
    if (item)
        return item;

    CTag data;
    data.setName(map.value("name").toString());
    return parent->add(data);
}


static void parseFolder(CManager *mgr, CTagItem *parent, const QVariantMap &map)
{
    foreach (QVariant val, map.value("children").toList())
    {
        QVariantMap map = val.toMap();

        QString type = map.value("type").toString();
        if (type == "url")
            addUrl(mgr, parent, map);
        else if (type == "folder")
            parseFolder(mgr, findOrAddTag(parent, map), map);
    }
}


bool bookmarkImportChromium(CManager *mgr, const QString &fileName,
        QString *reason)
{
    try
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            throw file.errorString();

        bool ok;
        QVariant res = QtJson::parse(QString::fromUtf8(file.readAll()), ok);
        if (!ok)
            throw QObject::tr("can't parse the chromium bookmark file");

        QVariant root = res.toMap().value("roots").toMap().value("other");
        if (!root.isValid())
            throw QObject::tr("can't parse the chromium bookmark file");

        parseFolder(mgr, mgr->tagMgr()->rootItem(), root.toMap());
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
        return false;
    }

    return true;
}
