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
#include <QDebug>
#include <QDir>
#include <QtGui>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "bookmarkimportchromium.h"
#include "cbookmarkfilter.h"
#include "cbookmarkfilterdatamodel.h"
#include "cbookmarkfilter.h"
#include "cbookmarkfilteritemmodel.h"
#include "cnavigationitemmodel.h"
#include <QMessageBox>
#include <QPushButton>


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    m_project = new CPrj(this);
    connect(m_project->actionCreate(), SIGNAL(triggered()),
            this, SLOT(actionCreate_triggered()));
    connect(m_project->actionOpen(), SIGNAL(triggered()),
            this, SLOT(actionOpen_triggered()));
    connect(m_project->actionSave(), SIGNAL(triggered()),
            this, SLOT(actionSave_triggered()));
    connect(m_project->actionClose(), SIGNAL(triggered()),
            this, SLOT(actionClose_triggered()));

    // Menu: File
    ui->menu_file->addAction(m_project->actionCreate());
    ui->menu_file->addAction(m_project->actionOpen());
    ui->menu_file->addAction(m_project->actionSave());
    ui->menu_file->addAction(m_project->actionClose());
    ui->menu_file->addSeparator();
    ui->menu_file->addAction(ui->action_quit);

    m_mainWidget = new CCompositWidget(m_project->manager(), this);
    setCentralWidget(m_mainWidget);

}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::actionCreate_triggered()
{
    if (m_project->isOpen())
    {
        int ret = QMessageBox::Ignore;
        if (m_project->hasChanges())
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
            if (!m_project->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }

        m_project->close();
    }

    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Create bookmarks"), "",
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    QString reason;
    if (!m_project->create(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);

    if (!m_project->open(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);
}

void CMainWindow::actionOpen_triggered()
{
    if (m_project->isOpen())
    {
        int ret = QMessageBox::Ignore;
        if (m_project->hasChanges())
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
            if (!m_project->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }

        m_project->close();
    }

    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Open bookmarks"), "",
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    QString reason;
    if (!m_project->open(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);
}

void CMainWindow::actionSave_triggered()
{
    QString reason;
    if (!m_project->save(&reason))
        QMessageBox::critical(this, tr("Critical"), reason);
}

void CMainWindow::actionClose_triggered()
{
    if (m_project->hasChanges())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Opened bookmarks was changed. Save the changes?"),
                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            QString reason;
            if (!m_project->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }
    }

    m_project->close();
}

void CMainWindow::on_action_quit_triggered()
{
    close();
}
