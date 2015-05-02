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
#include "ui_cmainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include "caboutdialog.h"
#include "cbookmarkview.h"
#include "bookmarkimportchromium.h"
#include "singleton.h"
#include "settings.h"


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("%1").arg(appName()));

    m_mainWidget = new CCompositWidget(this);
    setCentralWidget(m_mainWidget);

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

    // Menu: File
    ui->menu_file->addAction(GPrj()->actionCreate());
    ui->menu_file->addAction(GPrj()->actionOpen());
    ui->menu_file->addAction(GPrj()->actionSave());
    ui->menu_file->addAction(GPrj()->actionClose());
    ui->menu_file->addSeparator();
    ui->menu_file->addAction(ui->action_import);
    //ui->menu_file->addAction(ui->action_export);
    ui->menu_file->addSeparator();
    ui->menu_file->addAction(ui->action_quit);

    // Menu: Bookmark
    //ui->menu_bookmark->addAction(m_mainWidget->actionBookmarkAdd());
    //ui->menu_bookmark->addAction(m_mainWidget->actionBookmarkEdit());
    //ui->menu_bookmark->addAction(m_mainWidget->actionBookmarkRemove());

    project_closed();
    readSettings_window();
    readSettings_lastOpenedBookmarks();
}

CMainWindow::~CMainWindow()
{        
    writeSettings_window();
    writeSettings_lastOpenedBookmarks();

    delete ui;
}

void CMainWindow::project_opened()
{
    m_mainWidget->setEnabled(true);
    ui->action_import->setEnabled(true);
    ui->action_export->setEnabled(true);
    //ui->menu_bookmark->setEnabled(true);
    setWindowTitle(tr("%1 - %2").arg(appName(), GPrj()->path()));
}

void CMainWindow::project_closed()
{
    m_mainWidget->setEnabled(false);
    ui->action_import->setEnabled(false);
    ui->action_export->setEnabled(false);
    //ui->menu_bookmark->setEnabled(false);
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

void CMainWindow::on_action_quit_triggered()
{
    close();
}

void CMainWindow::on_action_import_triggered()
{
    QString reason, path = QDir::homePath() + "/.config/chromium/Default/Bookmarks";
    if (!bookmarkImportChromium(GPrj()->manager(), path, &reason))
        QMessageBox::warning(this, "Warning", reason);
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

void CMainWindow::on_action_about_triggered()
{
    CAboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void CMainWindow::on_action_aboutQt_triggered()
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
