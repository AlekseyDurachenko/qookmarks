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
#include "cbookmarkeditdialog.h"
#include "ui_cbookmarkeditdialog.h"


CBookmarkEditDialog::CBookmarkEditDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CBookmarkEditDialog)
{
    ui->setupUi(this);
}

CBookmarkEditDialog::~CBookmarkEditDialog()
{
    delete ui;
}

CBookmarkItemData CBookmarkEditDialog::toData() const
{
    CBookmarkItemData data;
    data.setTitle(ui->lineEdit_title->text());
    data.setUrl(ui->lineEdit_url->text());
    return data;
}

void CBookmarkEditDialog::setData(const CBookmarkItemData &data)
{
    ui->lineEdit_title->setText(data.title());
    ui->lineEdit_url->setText(data.url().toString());
}
