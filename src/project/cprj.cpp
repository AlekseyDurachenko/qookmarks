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
#include "cprj.h"
#include <QFileInfo>
#include <QDir>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cprjxml.h"
#include "bookmarkimportchromium.h"


CPrj::CPrj(QObject *parent) : QObject(parent)
{
    m_manager = new CManager(this);
    bookmarkImportChromium(m_manager, QDir::homePath() + "/.config/chromium/Default/Bookmarks");
    m_path.clear();
}

CPrj::~CPrj()
{
}

bool CPrj::create(const QString &path, QString *reason)
{
    try
    {
        QString absPath = QFileInfo(path).absoluteFilePath();
        // icons
        if (!QDir(iconPath(absPath)).exists())
            if (!QDir().mkpath(iconPath(absPath)))
                throw tr("can't create path \"%1\"").arg(iconPath(absPath));
        // screenshots
        if (!QDir(screenshotPath(absPath)).exists())
            if (!QDir().mkpath(screenshotPath(absPath)))
                throw tr("can't create path \"%1\"").arg(screenshotPath(absPath));
        // downloads (webpages, e.t.c.)
        if (!QDir(downloadsPath(absPath)).exists())
            if (!QDir().mkpath(downloadsPath(absPath)))
                throw tr("can't create path \"%1\"").arg(downloadsPath(absPath));

        QFile file(xmlPath(path));
        if (!file.open(QIODevice::WriteOnly))
            throw file.errorString();

        // default xml data
        QByteArray xmlData =
                "<!DOCTYPE QOOKMARKS-1.0>"
                "<PROJECT>"
                    "<TAG/>"
                    "<BOOKMARKS/>"
                "</PROJECT>";

        //if (file.write(xmlData) != xmlData.size())
        //    throw file.errorString();
        CPrjXml::saveXml(m_manager, &file, reason);

        //close();
        m_path = absPath;

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

bool CPrj::open(const QString &path, QString *reason)
{

}

bool CPrj::save(QString *reason)
{
    try
    {
        if (!isOpen())
            throw tr("bookmarks is not opened");

        QFile file(xmlPath());
        if (!file.open(QIODevice::WriteOnly))
            throw file.errorString();

        return CPrjXml::saveXml(m_manager, &file, reason);
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

void CPrj::close()
{
    m_manager->bookmarkMgr()->removeAll();
    m_manager->tagMgr()->rootItem()->removeAll();
    m_path.clear();
}

QString CPrj::xmlPath(const QString &path)
{
    return path + QDir::separator() + "qookmarks.xml";
}

QString CPrj::iconPath(const QString &path)
{
    return path + QDir::separator() + "icons";
}

QString CPrj::screenshotPath(const QString &path)
{
    return path + QDir::separator() + "screenshots";
}

QString CPrj::downloadsPath(const QString &path)
{
    return path + QDir::separator() + "downloads";
}
