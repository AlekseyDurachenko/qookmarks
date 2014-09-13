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
#ifndef CTAGEDITDIALOG_H
#define CTAGEDITDIALOG_H

#include "ctagitemdata.h"
#include <QDialog>


namespace Ui {
class CTagEditDialog;
}

class CTagEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CTagEditDialog(QWidget *parent = 0);
    virtual ~CTagEditDialog();

    QString name() const;
    CTagItemData toData() const;
private:
    Ui::CTagEditDialog *ui;
};


#endif // CTAGEDITDIALOG_H
