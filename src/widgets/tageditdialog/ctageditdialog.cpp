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
#include "ctageditdialog.h"
#include "ui_ctageditdialog.h"
#include <QPushButton>


CTagEditDialog::CTagEditDialog(Mode mode, CTagItem *tagParent,
        QWidget *parent) : QDialog(parent), ui(new Ui::CTagEditDialog)
{
    ui->setupUi(this);
    connect(ui->lineEdit_name, SIGNAL(textChanged(QString)),
            this, SLOT(slot_lineEdit_name_textChanged(QString)));

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
    connect(m_tagParent->mgr(), SIGNAL(destroyed()),
            this, SLOT(slot_mgr_destroyed()));

    slot_lineEdit_name_textChanged(ui->lineEdit_name->text());
}

CTagEditDialog::~CTagEditDialog()
{
    delete ui;
}

CTagItemData CTagEditDialog::toData() const
{
    CTagItemData data;
    data.setName(ui->lineEdit_name->text());
    return data;
}

void CTagEditDialog::setData(const CTagItemData &data)
{
    m_data = data;
    ui->lineEdit_name->setText(m_data.name());
}

void CTagEditDialog::slot_mgr_destroyed()
{
    m_tagParent = 0;
}

void CTagEditDialog::slot_lineEdit_name_textChanged(const QString &text)
{
    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (!m_tagParent || text.isEmpty())
    {
        button->setEnabled(false);
    }
    else if (m_mode == New)
    {
        button->setEnabled(!m_tagParent->findChild(text));
    }
    else if (m_mode == Edit)
    {
        button->setEnabled(m_data.name() == text
                           || !m_tagParent->findChild(text));
    }

    if (button->isEnabled())
    {
        ui->lineEdit_name->setPalette(QPalette());
    }
    else
    {
        QPalette palette = ui->lineEdit_name->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_name->setPalette(palette);
    }
}
