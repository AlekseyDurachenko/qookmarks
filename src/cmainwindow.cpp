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
#include "cbookmarkmgr.h"
#include "ctagitemmodel.h"
#include "caboutdialog.h"
#include "global.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(G_SHORT_PROGRAM_NAME());
    ui->treeView_tags->setModel(new CTagItemModel(this));

    CBookmarkMgr *mgr = new CBookmarkMgr(this);
}

CMainWindow::~CMainWindow()
{
    delete ui;
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
