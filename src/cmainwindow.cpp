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
#include "cmainwindow.h"
#include "ui_cmainwindow.h"
#include "caboutdialog.h"
#include "cimportbookmarkdialog.h"
#include "cbookmarkitemmodel.h"
#include "ctagitemmodel.h"
#include "cbookmarkmgr.h"
#include "global.h"
#include "cbookmarktagproxymodel.h"
#include <QSettings>
#include <QDebug>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(G_SHORT_PROGRAM_NAME());

    m_bookmarkMgr = new CBookmarkMgr(this);
    ui->treeView_tags->setBookmarkMgr(m_bookmarkMgr);
    ui->treeView_bookmarks->setBookmarkMgr(m_bookmarkMgr);
    connect(ui->treeView_tags, SIGNAL(currentTagChanged(QSet<CTagItem*>)),
            ui->treeView_bookmarks, SLOT(setTagFilter(QSet<CTagItem*>)));

    loadSettings();
}

CMainWindow::~CMainWindow()
{
    saveSettings();

    delete ui;
}

void CMainWindow::saveSettings()
{
    G_SETTINGS_INIT();
    settings.setValue("CMainWindow/geometry", saveGeometry());
    settings.setValue("CMainWindow/state", saveState());

    settings.setValue("CMainWindow/splitter_tagBookmark/geometry",
            ui->splitter_tagBookmark->saveGeometry());
    settings.setValue("CMainWindow/splitter_tagBookmark/state",
            ui->splitter_tagBookmark->saveState());
}

void CMainWindow::loadSettings()
{
    G_SETTINGS_INIT();
    restoreGeometry
            (settings.value
                ("CMainWindow/geometry", saveGeometry()).toByteArray());
    restoreState
            (settings.value
                ("CMainWindow/state", saveState()).toByteArray());

    ui->splitter_tagBookmark->restoreGeometry
            (settings.value("CMainWindow/splitter_tagBookmark/geometry",
                    ui->splitter_tagBookmark->saveGeometry()).toByteArray());
    ui->splitter_tagBookmark->restoreState
            (settings.value("CMainWindow/splitter_tagBookmark/state",
                    ui->splitter_tagBookmark->saveState()).toByteArray());
}

void CMainWindow::on_action_Quit_triggered()
{
    close();
}

void CMainWindow::on_action_AboutQt_triggered()
{
    qApp->aboutQt();
}

void CMainWindow::on_action_About_triggered()
{
    CAboutDialog dlg(this);
    dlg.exec();
}

void CMainWindow::on_action_ImportBookmarks_triggered()
{
    CImportBookmarkDialog dlg(m_bookmarkMgr, this);
    dlg.exec();
}
