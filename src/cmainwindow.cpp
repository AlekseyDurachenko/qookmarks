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
#include "cbookmarkitemmodel.h"
#include "ctagitemmodel.h"
#include "cbookmarkmgr.h"
#include "caboutdialog.h"
#include "global.h"
#include "cbookmarktagproxymodel.h"
#include <QDebug>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(G_SHORT_PROGRAM_NAME());

    mgr = new CBookmarkMgr(this);
    for (int i = 0; i < mgr->bookmarkCount(); ++i)
        qDebug() << mgr->bookmarkAt(i)->data().title();

    //mgr->tagAdd(mgr->tagTagRootItem(), CTagItemData());

    ui->treeView_tags->setModel(new CTagItemModel(mgr->tagRootItem(), this));

    CBookmarkItemModel *model = new CBookmarkItemModel(mgr, this);
    CBookmarkTagProxyModel *proxy = new CBookmarkTagProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setTags(QSet<CTagItem *>() << mgr->tagUntaggedItem());
    //mgr->bookmarkAddTag(mgr->bookmarkAt(0), mgr->tagReadLaterItem());
    ui->treeView_bookmarks->setModel(proxy);

    //ui->treeView_bookmarks->setModel(new CBookmarkItemModel(mgr, this));

//    CBookmarkItemData data;
//    data.setTitle("one");
//    data.setUrl(QUrl("http://one.url/"));
//    mgr->bookmarkAdd(data);

//    data.setTitle("two");
//    data.setUrl(QUrl("http://two.url/"));
//    mgr->bookmarkAdd(data);

//    data.setTitle("three");
//    data.setUrl(QUrl("http://three.url/"));
//    mgr->bookmarkAdd(data);
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

void CMainWindow::on_pushButton_clicked()
{
    static int n = 10;
    CBookmarkItemData data;
    data.setTitle(QString::number(++n));
    mgr->bookmarkAdd(data);
}

void CMainWindow::on_pushButton_2_clicked()
{
    mgr->bookmarkRemove(mgr->bookmarkAt(1));
    //mgr->tagMove(mgr->tagReadLaterItem(), mgr->tagTagRootItem()->childAt(0));
    //mgr->tagRemove(mgr->tagTagRootItem()->childAt(0));
}
