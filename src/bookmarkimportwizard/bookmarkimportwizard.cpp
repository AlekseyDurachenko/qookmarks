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
#include "bookmarkimportwizard.h"
#include "bookmarkimportchromium.h"
#include "icontheme.h"
#include "settings.h"
#include "singleton.h"
#include <QActionGroup>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QRadioButton>
#include <QSettings>
#include <QToolButton>
#include <QVBoxLayout>
#include <QDebug>


BookmarkImportWizard::BookmarkImportWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_SelectMode, new BookmarkImportSelectModeWizardPage);
    setPage(Page_SystemBrowser, new BookmarkImportSystemBrowserWizardPage);
    setPage(Page_File, new BookmarkImportFileWizardPage);

    setStartId(Page_SelectMode);
    setWindowTitle(tr("Import bookmark collection"));
}



BookmarkImportSelectModeWizardPage::BookmarkImportSelectModeWizardPage(
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

int BookmarkImportSelectModeWizardPage::nextId() const
{
    if (m_systemBrowserRadioButton->isChecked())
        return BookmarkImportWizard::Page_SystemBrowser;
    else
        return BookmarkImportWizard::Page_File;
}



BookmarkImportSystemBrowserWizardPage::BookmarkImportSystemBrowserWizardPage(
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

int BookmarkImportSystemBrowserWizardPage::nextId() const
{
    return -1;
}

bool BookmarkImportSystemBrowserWizardPage::isComplete() const
{
    if (m_browserList->currentItem())
        return true;
    else
        return false;
}

bool BookmarkImportSystemBrowserWizardPage::validatePage()
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

QString BookmarkImportSystemBrowserWizardPage::chromiumBookmarkCollection() const
{
#ifdef Q_OS_UNIX
    return QDir::homePath() + "/.config/chromium/Default/Bookmarks";
#endif

    return QString();
}



BookmarkImportFileWizardPage::BookmarkImportFileWizardPage(QWidget *parent) :
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

int BookmarkImportFileWizardPage::nextId() const
{
    return -1;
}

bool BookmarkImportFileWizardPage::isComplete() const
{
    if (m_browserList->currentItem()
            && !m_fileNameLineEdit->text().isEmpty())
        return true;
    else
        return false;
}

bool BookmarkImportFileWizardPage::validatePage()
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

void BookmarkImportFileWizardPage::selectFileName_toggled()
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
