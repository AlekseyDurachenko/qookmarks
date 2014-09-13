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
#ifndef CIMPORTBOOKMARKDIALOG_H
#define CIMPORTBOOKMARKDIALOG_H

#include <QDialog>


namespace Ui {
class CImportBookmarkDialog;
}

class CImportBookmarkDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CImportBookmarkDialog(QWidget *parent = 0);
    virtual ~CImportBookmarkDialog();
public slots:
    virtual void accept();
private slots:
    void on_listWidget_systemBrowsers_currentRowChanged(int currentRow);
private:
    QString pathChromium() const;
private:
    Ui::CImportBookmarkDialog *ui;
};


#endif // CIMPORTBOOKMARKDIALOG_H
