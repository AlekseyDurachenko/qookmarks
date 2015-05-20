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
#include <QListWidget>
#include <QDir>
#include <QMessageBox>
#include <QLineEdit>
#include <QToolButton>
#include <QSettings>
#include <QFileDialog>
#include "icontheme.h"
#include "singleton.h"
#include "settings.h"
#include "bookmarkimportchromium.h"
#include <QDebug>


CBookmarkImportWizard::CBookmarkImportWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_SelectMode, new CBookmarkImportSelectModeWizardPage);
    setPage(Page_SystemBrowser, new CBookmarkImportSystemBrowserWizardPage);
    setPage(Page_File, new CBookmarkImportFileWizardPage);

    setStartId(Page_SelectMode);
    setWindowTitle(tr("Import bookmark collection"));
}



CBookmarkImportSelectModeWizardPage::CBookmarkImportSelectModeWizardPage(
        QWidget *parent) : QWizardPage(parent)
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

int CBookmarkImportSelectModeWizardPage::nextId() const
{
    if (m_systemBrowserRadioButton->isChecked())
        return CBookmarkImportWizard::Page_SystemBrowser;
    else
        return CBookmarkImportWizard::Page_File;
}



CBookmarkImportSystemBrowserWizardPage::CBookmarkImportSystemBrowserWizardPage(
        QWidget *parent) : QWizardPage(parent)
{
    setTitle("Import bookmark collection from the system browser");

    m_browserList = new QListWidget(this);
    connect(m_browserList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SIGNAL(completeChanged()));

    // try to detect chromium bookmarks
    if (!chromiumBookmarkCollection().isEmpty())
    {
        QListWidgetItem *item = new QListWidgetItem(m_browserList);
        item->setIcon(IconTheme::icon("browser-chromium"));
        item->setText(tr("Chromium"));
        item->setData(Qt::UserRole, "chromium");
        m_browserList->addItem(item);
    }


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_browserList);
    setLayout(layout);
}

int CBookmarkImportSystemBrowserWizardPage::nextId() const
{
    return -1;
}

bool CBookmarkImportSystemBrowserWizardPage::isComplete() const
{
    if (m_browserList->currentItem())
        return true;
    else
        return false;
}

bool CBookmarkImportSystemBrowserWizardPage::validatePage()
{
    if (m_browserList->currentItem()->data(Qt::UserRole) == "chromium")
    {
        QString reason, path = chromiumBookmarkCollection();
        if (!bookmarkImportChromium(GPrj()->manager(), path, &reason))
        {
            QMessageBox::warning(this, "Warning", reason);
            return false;
        }
    }

    return true;
}

QString CBookmarkImportSystemBrowserWizardPage::chromiumBookmarkCollection() const
{
#ifdef Q_OS_UNIX
    return QDir::homePath() + "/.config/chromium/Default/Bookmarks";
#endif

    return QString();
}



CBookmarkImportFileWizardPage::CBookmarkImportFileWizardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Import bookmark collection from the file");

    m_browserList = new QListWidget(this);
    connect(m_browserList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SIGNAL(completeChanged()));

    m_fileNameLineEdit = new QLineEdit(this);
    connect(m_fileNameLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));

    m_selectFileNameButton = new QToolButton(this);
    m_selectFileNameButton->setText("...");
    connect(m_selectFileNameButton, SIGNAL(clicked(bool)),
            this, SLOT(selectFileName_toggled()));

    // chromium
    QListWidgetItem *chromiumItem = new QListWidgetItem(m_browserList);
    chromiumItem->setIcon(IconTheme::icon("browser-chromium"));
    chromiumItem->setText(tr("Chromium"));
    chromiumItem->setData(Qt::UserRole, "chromium");
    m_browserList->addItem(chromiumItem);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_browserList);
    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(m_fileNameLineEdit);
    fileLayout->addWidget(m_selectFileNameButton);
    layout->addLayout(fileLayout);
    setLayout(layout);
}

int CBookmarkImportFileWizardPage::nextId() const
{
    return -1;
}

bool CBookmarkImportFileWizardPage::isComplete() const
{
    if (m_browserList->currentItem()
            && !m_fileNameLineEdit->text().isEmpty())
        return true;
    else
        return false;
}

bool CBookmarkImportFileWizardPage::validatePage()
{
    if (m_browserList->currentItem()->data(Qt::UserRole) == "chromium")
    {
        QString reason, path = m_fileNameLineEdit->text();
        if (!bookmarkImportChromium(GPrj()->manager(), path, &reason))
        {
            QMessageBox::warning(this, "Warning", reason);
            return false;
        }
    }

    return true;
}

void CBookmarkImportFileWizardPage::selectFileName_toggled()
{
    G_SETTINGS_INIT();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open notes"),
                settings.value("lastBookmarkCollectionDirectory", "").toString(),
                tr("All (* *.*)"));

    if (fileName.isEmpty())
        return;

    m_fileNameLineEdit->setText(fileName);
    settings.setValue("lastBookmarkCollectionDirectory", QFileInfo(fileName).absolutePath());
}
