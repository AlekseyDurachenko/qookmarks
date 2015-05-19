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
#include "cbookmarkimportwizard.h"
#include <QActionGroup>
#include <QCheckBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QDebug>


CBookmarkImportWizard::CBookmarkImportWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(WizardPageSelect, new CBookmarkImportSelectWizardPage);
    setPage(WizardPageSelectSystemBrowser, new CBookmarkImportSelectSystemBrowserWizardPage);
    setPage(WizardPageSelectFile, new CBookmarkImportSelectFileWizardPage);

    setStartId(WizardPageSelect);
    setWindowTitle(tr("Import bookmark collection"));

    restart();
}

CBookmarkImportWizard::~CBookmarkImportWizard()
{
}



CBookmarkImportSelectWizardPage::CBookmarkImportSelectWizardPage(QWidget *parent)
{
    setTitle(tr("Select the bookmark collection location"));

    m_systemBrowserRadioButton = new QRadioButton(this);
    m_systemBrowserRadioButton->setText(tr("Load from the system browser"));
    m_systemBrowserRadioButton->setChecked(true);

    m_fileRadioButton = new QRadioButton(this);
    m_fileRadioButton->setText(tr("Load from the from local file"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_systemBrowserRadioButton);
    layout->addWidget(m_fileRadioButton);
    layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(layout);
}

int CBookmarkImportSelectWizardPage::nextId() const
{
    if (m_systemBrowserRadioButton->isChecked())
        return CBookmarkImportWizard::WizardPageSelectSystemBrowser;
    else
        return CBookmarkImportWizard::WizardPageSelectFile;
}



CBookmarkImportSelectSystemBrowserWizardPage::CBookmarkImportSelectSystemBrowserWizardPage(QWidget *parent)
{
}

int CBookmarkImportSelectSystemBrowserWizardPage::nextId() const
{
    return -1;
}



CBookmarkImportSelectFileWizardPage::CBookmarkImportSelectFileWizardPage(QWidget *parent)
{
}

int CBookmarkImportSelectFileWizardPage::nextId() const
{
    return -1;
}
