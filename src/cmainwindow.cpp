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
#include "cmainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QMenuBar>
#include "caboutdialog.h"
#include "cbookmarkview.h"
#include "bookmarkimportchromium.h"
#include "singleton.h"
#include "settings.h"
#include "icontheme.h"
#include "cbookmarkimportwizard.h"


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("%1").arg(appName()));

    m_windowStatusBar = new QStatusBar(this);
    setStatusBar(m_windowStatusBar);

    m_fileMenu = new QMenu(tr("&File"), this);
    m_tagMenu = new QMenu(tr("&Tag"), this);
    m_bookmarkMenu = new QMenu(tr("&Bookmark"), this);
    m_viewMenu = new QMenu(tr("&View"), this);
    m_toolbarMenu = new QMenu(tr("&Toolbars"), this);
    m_aboutMenu = new QMenu(tr("&About"), this);

    m_windowMenuBar = new QMenuBar(this);
    m_windowMenuBar->addMenu(m_fileMenu);
    m_windowMenuBar->addMenu(m_tagMenu);
    m_windowMenuBar->addMenu(m_bookmarkMenu);
    m_windowMenuBar->addMenu(m_viewMenu);
    m_viewMenu->addMenu(m_toolbarMenu);
    m_windowMenuBar->addMenu(m_aboutMenu);
    setMenuBar(m_windowMenuBar);


    m_actionBookmarkCollectionImport = new QAction(this);
    m_actionBookmarkCollectionImport->setText("Bookmark collection import...");
    m_actionBookmarkCollectionImport->setIcon(IconTheme::icon("action-collection-import"));
    connect(m_actionBookmarkCollectionImport, SIGNAL(triggered()),
            this, SLOT(actionBookmarkCollectionImport_triggered()));

    m_actionBookmarkCollectionExport = new QAction(this);
    m_actionBookmarkCollectionExport->setText("Bookmark collection export...");
    m_actionBookmarkCollectionExport->setIcon(IconTheme::icon("action-collection-export"));
    connect(m_actionBookmarkCollectionExport, SIGNAL(triggered()),
            this, SLOT(actionBookmarkCollectionExport_triggered()));

    m_actionQuit = new QAction(this);
    m_actionQuit->setText("Quit...");
    m_actionQuit->setIcon(IconTheme::icon("action-quit"));
    connect(m_actionQuit, SIGNAL(triggered()),
            this, SLOT(actionQuit_triggered()));

    m_actionAbout = new QAction(this);
    m_actionAbout->setText("About...");
    m_actionAbout->setIcon(IconTheme::icon("action-about"));
    connect(m_actionAbout, SIGNAL(triggered()),
            this, SLOT(actionAbout_triggered()));

    m_actionAboutQt = new QAction(this);
    m_actionAboutQt->setText("About Qt...");
    m_actionAboutQt->setIcon(IconTheme::icon("action-about-qt"));
    connect(m_actionAboutQt, SIGNAL(triggered()),
            this, SLOT(actionAboutQt_triggered()));


    m_fileToolBar = new QToolBar(tr("File"), this);
    m_fileToolBar->setObjectName("m_fileToolBar");
    addToolBar(m_fileToolBar);

    m_tagToolBar = new QToolBar(tr("Tag"), this);
    m_tagToolBar->setObjectName("m_tagToolBar");
    addToolBar(m_tagToolBar);

    m_bookmarkToolBar = new QToolBar(tr("Bookmark"), this);
    m_bookmarkToolBar->setObjectName("m_bookmarkToolBar");
    addToolBar(m_bookmarkToolBar);

    m_aboutToolBar = new QToolBar(tr("About"), this);
    m_aboutToolBar->setObjectName("m_aboutToolBar");
    addToolBar(m_aboutToolBar);

    m_toolbarMenu->addAction(m_fileToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_tagToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_bookmarkToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_aboutToolBar->toggleViewAction());

    m_mainWidget = new CCompositWidget(this);
    setCentralWidget(m_mainWidget);

    GNetworkMgr()->setNetwork(new QNetworkAccessManager(this));

    connect(GPrj()->actionCreate(), SIGNAL(triggered()),
            this, SLOT(actionCreate_triggered()));
    connect(GPrj()->actionOpen(), SIGNAL(triggered()),
            this, SLOT(actionOpen_triggered()));
    connect(GPrj()->actionSave(), SIGNAL(triggered()),
            this, SLOT(actionSave_triggered()));
    connect(GPrj()->actionClose(), SIGNAL(triggered()),
            this, SLOT(actionClose_triggered()));
    connect(GPrj(), SIGNAL(opened()), this, SLOT(project_opened()));
    connect(GPrj(), SIGNAL(closed()), this, SLOT(project_closed()));

    // Menu && Toolbar: File
    m_fileMenu->addAction(GPrj()->actionCreate());
    m_fileMenu->addAction(GPrj()->actionOpen());
    m_fileMenu->addAction(GPrj()->actionSave());
    m_fileMenu->addAction(GPrj()->actionClose());
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionBookmarkCollectionImport);
    m_fileMenu->addAction(m_actionBookmarkCollectionExport);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionQuit);


    m_fileToolBar->addAction(GPrj()->actionCreate());
    m_fileToolBar->addAction(GPrj()->actionOpen());
    m_fileToolBar->addAction(GPrj()->actionSave());
    m_fileToolBar->addAction(GPrj()->actionClose());
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_actionBookmarkCollectionImport);
    m_fileToolBar->addAction(m_actionBookmarkCollectionExport);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_actionQuit);

    // Menu && Toolbar: Tags
    m_tagMenu->addAction(m_mainWidget->actionTagAdd());
    m_tagMenu->addAction(m_mainWidget->actionTagEdit());
    m_tagMenu->addAction(m_mainWidget->actionTagRemove());

    m_tagToolBar->addAction(m_mainWidget->actionTagAdd());
    m_tagToolBar->addAction(m_mainWidget->actionTagEdit());
    m_tagToolBar->addAction(m_mainWidget->actionTagRemove());


    // Menu && Toolbar: Bookmark
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkOpenUrl());
    m_bookmarkMenu->addAction(m_mainWidget->actionMenuBookmarkOpenUrl());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkSelectAll());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_mainWidget->actionMenuFavorite());
    m_bookmarkMenu->addAction(m_mainWidget->actionMenuReadItLater());
    m_bookmarkMenu->addAction(m_mainWidget->actionMenuRating());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkAdd());
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkEdit());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkSendToTrash());
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkRestore());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_mainWidget->actionBookmarkRemove());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_mainWidget->actionEmptyTrash());

    QToolButton *openUrlButton = new QToolButton(this);
    openUrlButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    openUrlButton->setDefaultAction(m_mainWidget->actionBookmarkOpenUrl());
    m_bookmarkToolBar->addWidget(openUrlButton);
    QToolButton *openUrlExtButton = new QToolButton(this);
    openUrlExtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    openUrlExtButton->setPopupMode(QToolButton::MenuButtonPopup);
    openUrlExtButton->setMenu(m_mainWidget->menuBookmarkOpenUrl());
    openUrlExtButton->setText(m_mainWidget->actionMenuBookmarkOpenUrl()->text());
    openUrlExtButton->setIcon(m_mainWidget->actionBookmarkOpenUrl()->icon());
    m_bookmarkToolBar->addWidget(openUrlExtButton);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_mainWidget->actionBookmarkSelectAll());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_mainWidget->actionMenuFavorite());
    m_bookmarkToolBar->addAction(m_mainWidget->actionMenuReadItLater());
    m_bookmarkToolBar->addAction(m_mainWidget->actionMenuRating());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_mainWidget->actionBookmarkAdd());
    m_bookmarkToolBar->addAction(m_mainWidget->actionBookmarkEdit());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_mainWidget->actionBookmarkSendToTrash());
    m_bookmarkToolBar->addAction(m_mainWidget->actionBookmarkRestore());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_mainWidget->actionBookmarkRemove());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_mainWidget->actionEmptyTrash());

    // Menu && Toolbar: About
    m_aboutMenu->addAction(m_actionAbout);
    m_aboutMenu->addAction(m_actionAboutQt);

    m_aboutToolBar->addAction(m_actionAbout);
    m_aboutToolBar->addAction(m_actionAboutQt);

    project_closed();
    readSettings_window();
    readSettings_lastOpenedBookmarks();
}

CMainWindow::~CMainWindow()
{
    writeSettings_window();
    writeSettings_lastOpenedBookmarks();
}

void CMainWindow::project_opened()
{
    m_actionBookmarkCollectionImport->setEnabled(true);
    m_actionBookmarkCollectionExport->setEnabled(true);
    m_tagMenu->setEnabled(true);
    m_bookmarkMenu->setEnabled(true);
    setWindowTitle(tr("%1 - %2").arg(appName(), GPrj()->path()));
}

void CMainWindow::project_closed()
{
    m_actionBookmarkCollectionImport->setEnabled(false);
    m_actionBookmarkCollectionExport->setEnabled(false);
    m_tagMenu->setEnabled(false);
    m_bookmarkMenu->setEnabled(false);
    setWindowTitle(tr("%1").arg(appName()));
}

void CMainWindow::actionCreate_triggered()
{
    if (GPrj()->isOpen())
    {
        int ret = QMessageBox::Ignore;
        if (GPrj()->hasChanges())
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Opened bookmarks was changed. What do you want?"),
                    QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
        }
        else
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Bookmarks is open. Close current bookmarks?"),
                    QMessageBox::Yes|QMessageBox::Cancel);
        }

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
        else if (ret == QMessageBox::Save)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }

        GPrj()->close();
    }

    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Create bookmarks"), readSettings_lastBookmarkDirectory(),
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    if (QFile(CPrj::xmlPath(dirName)).exists())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Project is already created. Replace the project?"),
                QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    QString reason;
    if (!GPrj()->create(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);
    else
        if (!GPrj()->open(dirName, &reason))
            QMessageBox::warning(this, tr("Warning"), reason);

    writeSettings_lastBookmarkDirectory(dirName);
}

void CMainWindow::actionOpen_triggered()
{
    if (GPrj()->isOpen())
    {
        int ret = QMessageBox::Ignore;
        if (GPrj()->hasChanges())
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Opened bookmarks was changed. What do you want?"),
                    QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
        }
        else
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Bookmarks is open. Close current bookmarks?"),
                    QMessageBox::Yes|QMessageBox::Cancel);
        }

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
        else if (ret == QMessageBox::Save)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }

        GPrj()->close();
    }

    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Open bookmarks"), readSettings_lastBookmarkDirectory(),
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    QString reason;
    if (!GPrj()->open(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);

    writeSettings_lastBookmarkDirectory(dirName);
}

void CMainWindow::actionSave_triggered()
{
    QString reason;
    if (!GPrj()->save(&reason))
        QMessageBox::critical(this, tr("Critical"), reason);
}

void CMainWindow::actionClose_triggered()
{
    if (GPrj()->hasChanges())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Opened bookmarks was changed. Save the changes?"),
                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }
    }

    // HACK: removing so slow if selection is exists
    m_mainWidget->bookmarkView()->selectionModel()->clear();

    GPrj()->close();
}

void CMainWindow::actionQuit_triggered()
{
    close();
}

void CMainWindow::actionBookmarkCollectionImport_triggered()
{
    CBookmarkImportWizard importWizard;
    importWizard.exec();
}

void CMainWindow::actionBookmarkCollectionExport_triggered()
{
    ;
}

void CMainWindow::readSettings_window()
{
    G_SETTINGS_INIT();
    restoreState(settings.value(
                     "CMainWindow/state", saveState()).toByteArray());
    restoreGeometry(settings.value(
                        "CMainWindow/geometry",saveGeometry()).toByteArray());
}

void CMainWindow::writeSettings_window()
{
    G_SETTINGS_INIT();
    settings.setValue("CMainWindow/state", saveState());
    settings.setValue("CMainWindow/geometry", saveGeometry());
}

void CMainWindow::readSettings_lastOpenedBookmarks()
{
    G_SETTINGS_INIT();
    QString reason, path = settings.value("lastBookmarks", "").toString();
    if (!path.isEmpty())
        if (!GPrj()->open(path, &reason))
            QMessageBox::warning(this, tr("Warning"), reason);
}

void CMainWindow::writeSettings_lastOpenedBookmarks()
{
    G_SETTINGS_INIT();
    settings.setValue("lastBookmarks", GPrj()->path());
}

QString CMainWindow::readSettings_lastBookmarkDirectory()
{
    G_SETTINGS_INIT();
    return settings.value("lastBookmarksDirectory", "").toString();
}

void CMainWindow::writeSettings_lastBookmarkDirectory(const QString &path)
{
    G_SETTINGS_INIT();
    settings.setValue("lastBookmarksDirectory", path);
}

void CMainWindow::actionAbout_triggered()
{
    CAboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void CMainWindow::actionAboutQt_triggered()
{
    qApp->aboutQt();
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    if (GPrj()->hasChanges())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Opened bookmarks was changed. Save the changes?"),
                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                event->ignore();
            }
        }
    }

    QMainWindow::closeEvent(event);
}
