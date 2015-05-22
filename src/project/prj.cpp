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
#include "prj.h"
#include "bookmarkimportchromium.h"
#include "bookmarkitem.h"
#include "bookmarkmgr.h"
#include "iconmgr.h"
#include "manager.h"
#include "prjxml.h"
#include "tagitem.h"
#include "tagmgr.h"
#include "icontheme.h"
#include <QDir>
#include <QFileInfo>


Prj::Prj(QObject *parent) : QObject(parent)
{
    m_createAction = new QAction(tr("New Bookmark Collection..."), this);
    m_createAction->setIcon(IconTheme::icon("action-collection-create"));
    m_openAction = new QAction(tr("Open Bookmark Collection..."), this);
    m_openAction->setIcon(IconTheme::icon("action-collection-open"));
    m_saveAction = new QAction(tr("Save Bookmark Collection"), this);
    m_saveAction->setIcon(IconTheme::icon("action-collection-save"));
    m_closeAction = new QAction(tr("Close Bookmark Collection..."), this);
    m_closeAction->setIcon(IconTheme::icon("action-collection-close"));

    m_manager = new Manager(this);
    m_iconMgr = new IconMgr();
    connect(m_manager->tagMgr(), SIGNAL(inserted(TagItem*,int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->tagMgr(), SIGNAL(removed(TagItem*,int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->tagMgr(), SIGNAL(dataChanged(TagItem*)),
            this, SLOT(somethingChanged()));
    connect(m_manager->tagMgr(), SIGNAL(moved(TagItem*,int,int,TagItem*,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(dataChanged(BookmarkItem*)),
            this, SLOT(somethingChanged()));
    connect(m_manager->bookmarkMgr(), SIGNAL(tagsChanged(BookmarkItem*)),
            this, SLOT(somethingChanged()));

    m_path = QString();
    m_hasChanges = false;
    updateActions();
}

Prj::~Prj()
{
    delete m_iconMgr;
}

bool Prj::create(const QString &path, QString *reason)
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
        return PrjXml::saveEmptyXml(&file, reason);
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

bool Prj::open(const QString &path, QString *reason)
{
    try
    {
        if (isOpen())
            throw tr("bookmarks collection is already opened");

        QFile file(xmlPath(path));
        if (!file.open(QIODevice::ReadOnly))
            throw file.errorString();

        blockSignals(true);
        bool state = PrjXml::loadXml(m_manager, &file, reason);
        blockSignals(false);
        if (!state)
            return false;

        m_path = path;
        m_hasChanges = false;

        QList<QUrl> urls;
        foreach (BookmarkItem *item, m_manager->bookmarkMgr()->bookmarks())
            urls.push_back(item->data().url());
        m_iconMgr->setIconsPath(iconPath());
        m_iconMgr->loadIcons(urls);

        updateActions();
        emit opened();

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

bool Prj::save(QString *reason)
{
    try
    {
        if (!isOpen())
            throw tr("bookmarks collection is not opened");

        QFile file(xmlPath());
        if (!file.open(QIODevice::WriteOnly))
            throw file.errorString();

        bool state = PrjXml::saveXml(m_manager, &file, reason);
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

void Prj::close()
{
    blockSignals(true);
    m_manager->bookmarkMgr()->removeAll();
    m_manager->tagMgr()->rootItem()->removeAll();
    blockSignals(false);

    m_iconMgr->unloadIcons();
    m_iconMgr->setIconsPath(QString());

    m_path = QString();
    m_hasChanges = false;
    updateActions();
    emit closed();
}

QString Prj::xmlPath(const QString &path)
{
    return path + QDir::separator() + "qookmarks.xml";
}

QString Prj::iconPath(const QString &path)
{
    return path + QDir::separator() + "icons";
}

QString Prj::screenshotPath(const QString &path)
{
    return path + QDir::separator() + "screenshots";
}

QString Prj::downloadsPath(const QString &path)
{
    return path + QDir::separator() + "downloads";
}

void Prj::somethingChanged()
{
    m_hasChanges = true;
    updateActions();
}

void Prj::updateActions()
{
    // TODO: realize the correct updates
    m_saveAction->setEnabled(hasChanges());
    m_closeAction->setEnabled(isOpen());
}
