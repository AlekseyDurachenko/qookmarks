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
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "ccompositwidget.h"



class CMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CMainWindow(QWidget *parent = 0);
    virtual ~CMainWindow();
private slots:
    void project_opened();
    void project_closed();

    void actionCreate_triggered();
    void actionOpen_triggered();
    void actionSave_triggered();
    void actionClose_triggered();

    void actionQuit_triggered();
    void actionBookmarkCollectionImport_triggered();
    void actionBookmarkCollectionExport_triggered();
    void actionAbout_triggered();
    void actionAboutQt_triggered();
protected:
    void closeEvent(QCloseEvent *event);
private:
    void readSettings_window();
    void writeSettings_window();

    void readSettings_lastOpenedBookmarks();
    void writeSettings_lastOpenedBookmarks();

    QString readSettings_lastBookmarkDirectory();
    void writeSettings_lastBookmarkDirectory(const QString &path);
private:
    CCompositWidget *m_mainWidget;
    QMenuBar *m_windowMenuBar;
    QMenu *m_fileMenu;
    QMenu *m_tagMenu;
    QMenu *m_bookmarkMenu;
    QMenu *m_viewMenu;
    QMenu *m_toolbarMenu;
    QMenu *m_aboutMenu;
    QStatusBar *m_windowStatusBar;

    QToolBar *m_fileToolBar;
    QToolBar *m_tagToolBar;
    QToolBar *m_bookmarkToolBar;
    QToolBar *m_aboutToolBar;

    QAction *m_actionQuit;
    QAction *m_actionBookmarkCollectionImport;
    QAction *m_actionBookmarkCollectionExport;
    QAction *m_actionAbout;
    QAction *m_actionAboutQt;
};


#endif // CMAINWINDOW_H
