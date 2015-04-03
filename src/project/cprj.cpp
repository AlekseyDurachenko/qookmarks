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
    m_actionCreate = new QAction(tr("Create..."), this);
    m_actionOpen = new QAction(tr("Open..."), this);
    m_actionSave = new QAction(tr("Save"), this);
    m_actionClose = new QAction(tr("Close..."), this);

    m_manager = new CManager(this);
    connect(m_manager->tagMgr(), SIGNAL(inserted(CTagItem*,int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->tagMgr(), SIGNAL(removed(CTagItem*,int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->tagMgr(), SIGNAL(dataChanged(CTagItem*,CTag,CTag)),
            this, SLOT(somethingChanged()));
    connect(m_manager->tagMgr(), SIGNAL(moved(CTagItem*,int,int,CTagItem*,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*,CBookmark,CBookmark)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(tagsChanged(CBookmarkItem*)),
            this, SLOT(somethingChanged()));

    m_path = QString();
    m_hasChanges = false;
    updateActions();
}

bool CPrj::create(const QString &path, QString *reason)
{
    try
    {
        QString absPath = QFileInfo(path).absoluteFilePath();       

        // create path for icons
        if (!QDir(iconPath(absPath)).exists())
            if (!QDir().mkpath(iconPath(absPath)))
                throw tr("can't create path \"%1\"").arg(iconPath(absPath));
        // create path for screenshots
        if (!QDir(screenshotPath(absPath)).exists())
            if (!QDir().mkpath(screenshotPath(absPath)))
                throw tr("can't create path \"%1\"").arg(screenshotPath(absPath));
        // create path for downloads (webpages, e.t.c.)
        if (!QDir(downloadsPath(absPath)).exists())
            if (!QDir().mkpath(downloadsPath(absPath)))
                throw tr("can't create path \"%1\"").arg(downloadsPath(absPath));

        QFile file(xmlPath(path));
        if (!file.open(QIODevice::WriteOnly))
            throw file.errorString();

        // write empty xml document
        return CPrjXml::saveEmptyXml(&file, reason);
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

bool CPrj::open(const QString &path, QString *reason)
{
    try
    {
        if (isOpen())
            throw tr("bookmarks is already opened");

        QFile file(xmlPath(path));
        if (!file.open(QIODevice::ReadOnly))
            throw file.errorString();

        blockSignals(true);
        if (!CPrjXml::loadXml(m_manager, &file, reason))
            return false;
        blockSignals(false);

        m_path = path;
        m_hasChanges = false;
        updateActions();

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
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

        bool state = CPrjXml::saveXml(m_manager, &file, reason);
        if (state)
            m_hasChanges = false;
        updateActions();

        return state;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

void CPrj::close()
{
    blockSignals(true);
    m_manager->bookmarkMgr()->removeAll();
    m_manager->tagMgr()->rootItem()->removeAll();
    blockSignals(false);

    m_path = QString();
    m_hasChanges = false;
    updateActions();
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

void CPrj::somethingChanged()
{
    m_hasChanges = true;
    updateActions();
}

void CPrj::updateActions()
{
    // TODO: realize the correct updates
    m_actionSave->setEnabled(hasChanges());
    m_actionClose->setEnabled(isOpen());
}
