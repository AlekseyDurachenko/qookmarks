// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef TAGEDITDIALOG_H
#define TAGEDITDIALOG_H

#include "ctag.h"
#include <QDialog>
class CTagItem;


namespace Ui {
class TagEditDialog;
}

class TagEditDialog : public QDialog
{
    Q_OBJECT
public:
    enum Mode
    {
        New     = 0,
        Edit    = 1
    };
public:
    explicit TagEditDialog(Mode mode, CTagItem *tagParent, QWidget *parent = 0);
    virtual ~TagEditDialog();

    inline Mode mode() const;
    inline CTagItem *tagParent() const;

    CTag toData() const;
    void setData(const CTag &data);
private slots:
    void slot_name_lineEdit_textChanged(const QString &text);
private:
    Ui::TagEditDialog *ui;
    CTag m_data;
    CTagItem *m_tagParent;
    Mode m_mode;
};

TagEditDialog::Mode TagEditDialog::mode() const
{
    return m_mode;
}

CTagItem *TagEditDialog::tagParent() const
{
    return m_tagParent;
}


#endif // TAGEDITDIALOG_H
