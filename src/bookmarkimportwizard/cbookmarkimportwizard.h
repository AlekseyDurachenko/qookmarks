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
#ifndef CBOOKMARKIMPORTWIZARD_H
#define CBOOKMARKIMPORTWIZARD_H

#include <QWizard>
class QRadioButton;
class QListWidget;
class QToolButton;
class QLineEdit;


class CBookmarkImportWizard : public QWizard
{
    Q_OBJECT
    friend class CBookmarkImportSelectModeWizardPage;
    friend class CBookmarkImportSystemBrowserWizardPage;
    friend class CBookmarkImportFileWizardPage;
private:
    enum
    {
        Page_SelectMode,
        Page_SystemBrowser,
        Page_File
    };
public:
    explicit CBookmarkImportWizard(QWidget *parent = 0);
};



class CBookmarkImportSelectModeWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    CBookmarkImportSelectModeWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
private:
    QRadioButton *m_systemBrowserRadioButton;
    QRadioButton *m_fileRadioButton;
};



class CBookmarkImportSystemBrowserWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    CBookmarkImportSystemBrowserWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
    virtual bool isComplete() const;
    virtual bool validatePage();
private:
    QString chromiumBookmarkCollection() const;
private:
    QListWidget *m_browserList;
};



class CBookmarkImportFileWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    CBookmarkImportFileWizardPage(QWidget *parent = 0);
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


#endif // CBOOKMARKIMPORTWIZARD_H
