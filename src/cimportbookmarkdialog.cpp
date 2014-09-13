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
#include "cimportbookmarkdialog.h"
#include "ui_cimportbookmarkdialog.h"
#include <QPushButton>
#include <QFileInfo>
#include <QDir>


CImportBookmarkDialog::CImportBookmarkDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CImportBookmarkDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    if (!pathChromium().isEmpty() && QFileInfo(pathChromium()).exists())
        ui->listWidget_systemBrowsers->addItem
                (new QListWidgetItem(QIcon(":/icons/browser-chromium.png"),
                    tr("Chromium")));
}

CImportBookmarkDialog::~CImportBookmarkDialog()
{
    delete ui;
}

void CImportBookmarkDialog::accept()
{
    QDialog::accept();
}

void CImportBookmarkDialog::on_listWidget_systemBrowsers_currentRowChanged(
        int currentRow)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(currentRow != -1);
}

QString CImportBookmarkDialog::pathChromium() const
{
#ifdef Q_OS_UNIX
    return QDir::homePath() + "/.config/chromium/Default/Bookmarks";
#endif
    return QString();
}
