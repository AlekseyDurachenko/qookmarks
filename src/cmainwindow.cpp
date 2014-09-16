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
#include <QtWebKit/QWebView>

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

    m_networkMgr = new QNetworkAccessManager(this);
    m_reply = 0;

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

void CMainWindow::on_action_TestAllUrls_triggered()
{
    m_urlList.clear();
    for (int i = 0; i < m_bookmarkMgr->bookmarkCount(); ++i)
    {
        m_urlList.push_back(m_bookmarkMgr->bookmarkAt(i)->data().url());
        m_tmpHash[m_bookmarkMgr->bookmarkAt(i)->data().url()] = i;
    }

    QMetaObject::invokeMethod(this, "state_getNextUrl");
}

void CMainWindow::state_getNextUrl()
{
    m_reply = m_networkMgr->get(QNetworkRequest(m_urlList.takeFirst()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(state_downloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(state_replyFinished()));
    connect(m_reply, SIGNAL(finished()), m_reply, SLOT(deleteLater()));
    m_reply->setReadBufferSize(64);
}

void CMainWindow::state_replyFinished()
{
    int code = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl url = m_reply->url();
    m_reply = 0;

    CBookmarkItem *item = m_bookmarkMgr->bookmarkAt(m_tmpHash[url]);
    CBookmarkItemData data = item->data();
    data.setHttpCode(code);
    item->setData(data);

    if (!m_urlList.isEmpty())
        QMetaObject::invokeMethod(this, "state_getNextUrl");
}

void CMainWindow::state_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_reply->abort(); // double call are happened
}

void CMainWindow::on_action_TestSavePageImage_triggered()
{
//    QWebView *view = new QWebView;
//    view->setAttribute(Qt::WA_DeleteOnClose);
//    view->load(QUrl("http://stackoverflow.com/questions/9086046/create-whole-page-screen-shot-using-qwebpage"));
//    view->show();
}
