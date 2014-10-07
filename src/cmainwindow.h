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
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QtNetwork>
#include <QMainWindow>
#include <QHash>
#include <QUrl>
class CBookmarkMgr;


namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CMainWindow(QWidget *parent = 0);
    virtual ~CMainWindow();
private slots:
    void on_action_Quit_triggered();
    void on_action_AboutQt_triggered();
    void on_action_About_triggered();
    void on_action_ImportBookmarks_triggered();
    void on_action_TestAllUrls_triggered();

    void state_getNextUrl();
    void state_replyFinished();
    void state_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void on_action_TestSavePageImage_triggered();
    void on_treeView_tags_currentTagChanged();
private:
    void saveSettings();
    void loadSettings();
private:
    Ui::CMainWindow *ui;
    CBookmarkMgr *m_bookmarkMgr;
    QList<QUrl> m_urlList;
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_networkMgr;
    QHash<QUrl, int> m_tmpHash;
};


#endif // CMAINWINDOW_H
