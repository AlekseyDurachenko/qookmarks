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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "settings.h"
#include <QFile>


AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About %1 - %2").arg(appName(), appVersion()));

    ui->programmTitle_label->setText(
                tr("About %1 - %2").arg(appName(), appVersion()));

    ui->about_plainTextEdit->setPlainText(
                fromResource(":/about/ABOUT"));
    ui->authors_plainTextEdit->setPlainText(
                fromResource(":/about/AUTHORS"));
    ui->changelog_plainTextEdit->setPlainText(
                fromResource(":/about/CHANGELOG"));
    ui->license_plainTextEdit->setPlainText(
                fromResource(":/about/LICENSE"));
    ui->libraries_plainTextEdit->setPlainText(
                fromResource(":/about/LIBRARIES"));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

QString AboutDialog::fromResource(const QString &resourceName)
{
    QFile text(resourceName);
    text.open(QIODevice::ReadOnly);
    return QString::fromUtf8(text.readAll());
}
