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
#include "caboutdialog.h"
#include "ui_caboutdialog.h"
#include "global.h"
#include <QFile>

CAboutDialog::CAboutDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::CAboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About %1").arg(g_programName));

    ui->label_programmTitle->setText(G_FULL_PROGRAM_NAME());

    ui->plainTextEdit_about->setPlainText
            (fromResource(":/about/ABOUT"));
    ui->plainTextEdit_authors->setPlainText
            (fromResource(":/about/AUTHORS"));
    ui->plainTextEdit_authors->setPlainText
            (fromResource(":/about/AUTHORS"));
    ui->plainTextEdit_changelog->setPlainText
            (fromResource(":/about/CHANGELOG"));
    ui->plainTextEdit_license->setPlainText
            (fromResource(":/about/LICENSE"));
    ui->plainTextEdit_libraries->setPlainText
            (fromResource(":/about/LIBRARIES"));
}

CAboutDialog::~CAboutDialog()
{
    delete ui;
}

QString CAboutDialog::fromResource(const QString &resourceName)
{
    QFile text(resourceName);
    text.open(QIODevice::ReadOnly);
    return text.readAll();
}
