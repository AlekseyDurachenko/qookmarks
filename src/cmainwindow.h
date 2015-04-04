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
#include <QDragEnterEvent>
#include "ctagitem.h"
#include "cbookmarkitem.h"
#include "inavigationactions.h"
#include "ccompositwidget.h"
#include "project/cprj.h"
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CManager;

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow //, public INavigationActions
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
    void on_action_quit_triggered();
    void on_action_import_triggered();
private:
    void readSettings_window();
    void readSettings_lastOpenedBookmarks();
    QString readSettings_lastBookmarkDirectory();
    void writeSettings_window();
    void writeSettings_lastOpenedBookmarks();
    void writeSettings_lastBookmarkDirectory(const QString &path);
private:
    Ui::CMainWindow *ui;
    CPrj *m_project;
    CCompositWidget *m_mainWidget;
};

#endif // CMAINWINDOW_H
