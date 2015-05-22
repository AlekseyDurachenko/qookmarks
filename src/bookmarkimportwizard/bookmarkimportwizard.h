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
#ifndef BOOKMARKIMPORTWIZARD_H
#define BOOKMARKIMPORTWIZARD_H

#include <QWizard>
class QLineEdit;
class QListWidget;
class QRadioButton;
class QToolButton;


class BookmarkImportWizard : public QWizard
{
    Q_OBJECT
    friend class BookmarkImportSelectModeWizardPage;
    friend class BookmarkImportSystemBrowserWizardPage;
    friend class BookmarkImportFileWizardPage;
private:
    enum
    {
        Page_SelectMode,
        Page_SystemBrowser,
        Page_File
    };
public:
    explicit BookmarkImportWizard(QWidget *parent = 0);
};



class BookmarkImportSelectModeWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    BookmarkImportSelectModeWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
private:
    QRadioButton *m_systemBrowserRadioButton;
    QRadioButton *m_fileRadioButton;
};



class BookmarkImportSystemBrowserWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    BookmarkImportSystemBrowserWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
    virtual bool isComplete() const;
    virtual bool validatePage();
private:
    QString chromiumBookmarkCollection() const;
private:
    QListWidget *m_browserList;
};



class BookmarkImportFileWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    BookmarkImportFileWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
    virtual bool isComplete() const;
    virtual bool validatePage();
private slots:
    void selectFileName_toggled();
private:
    QListWidget *m_browserList;
    QToolButton *m_selectFileNameButton;
    QLineEdit *m_fileNameLineEdit;
};


#endif // BOOKMARKIMPORTWIZARD_H
