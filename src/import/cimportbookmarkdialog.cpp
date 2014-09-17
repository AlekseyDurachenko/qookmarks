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
#include "importbookmarkchromium.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>


CImportBookmarkDialog::CImportBookmarkDialog(CBookmarkMgr *mgr,
        QWidget *parent) :
    QDialog(parent), ui(new Ui::CImportBookmarkDialog)
{
    m_mgr = mgr;
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    if (!pathChromium().isEmpty() && QFileInfo(pathChromium()).exists())
    {
        QListWidgetItem *item = new QListWidgetItem(tr("Chromium"));
        item->setIcon(QIcon(":/icons/browser-chromium.png"));
        item->setData(Qt::UserRole, Chromium);
        ui->listWidget_systemBrowsers->addItem(item);
    }
}

CImportBookmarkDialog::~CImportBookmarkDialog()
{
    delete ui;
}

void CImportBookmarkDialog::accept()
{
    bool ok = false;
    QString errorString;

    if (ui->tabWidget->currentIndex() == 0)
    {
        QListWidgetItem *item = ui->listWidget_systemBrowsers->currentItem();
        if (item->data(Qt::UserRole).toInt() == Chromium)
            ok = importBookmarkChromium(m_mgr, pathChromium(), &errorString);
    }

    if (!ok)
    {
        QMessageBox::critical(this, tr("Critical"), errorString);
        QDialog::reject();
    }
    else
    {
        QDialog::accept();
    }
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
