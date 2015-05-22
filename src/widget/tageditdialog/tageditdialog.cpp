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
#include "tageditdialog.h"
#include "ui_tageditdialog.h"
#include "ctagitem.h"
#include "ctagmgr.h"
#include <QPushButton>


TagEditDialog::TagEditDialog(Mode mode, CTagItem *tagParent,
        QWidget *parent) : QDialog(parent), ui(new Ui::TagEditDialog)
{
    ui->setupUi(this);
    connect(ui->name_lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(slot_name_lineEdit_textChanged(QString)));

    m_mode = mode;
    switch (m_mode)
    {
    case New:
        setWindowTitle(tr("Create new tag"));
        break;
    case Edit:
        setWindowTitle(tr("Edit the tag"));
        break;
    }

    m_tagParent = tagParent;

    slot_name_lineEdit_textChanged(ui->name_lineEdit->text());
}

TagEditDialog::~TagEditDialog()
{
    delete ui;
}

CTag TagEditDialog::toData() const
{
    CTag data;
    data.setName(ui->name_lineEdit->text());
    return data;
}

void TagEditDialog::setData(const CTag &data)
{
    m_data = data;
    ui->name_lineEdit->setText(m_data.name());
}

void TagEditDialog::slot_name_lineEdit_textChanged(const QString &text)
{
    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (!m_tagParent || text.isEmpty())
    {
        button->setEnabled(false);
    }
    else if (m_mode == New)
    {
        button->setEnabled(!m_tagParent->find(text));
    }
    else if (m_mode == Edit)
    {
        button->setEnabled(m_data.name() == text
                           || !m_tagParent->find(text));
    }

    if (button->isEnabled())
    {
        ui->name_lineEdit->setPalette(QPalette());
    }
    else
    {
        QPalette palette = ui->name_lineEdit->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->name_lineEdit->setPalette(palette);
    }
}
