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


class CBookmarkImportWizard : public QWizard
{
    Q_OBJECT
private:
    enum CollectionLocationType
    {
        CollectionLocationNone = 0,
        CollectionLocationSystemBrowser,
        CollectionLocationFile
    };
public:
    enum WizardPageType
    {
        WizardPageSelect,
        WizardPageSelectSystemBrowser,
        WizardPageSelectFile
    };
public:
    explicit CBookmarkImportWizard(QWidget *parent = 0);
    virtual ~CBookmarkImportWizard();
};



class CBookmarkImportSelectWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    CBookmarkImportSelectWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
private:
    QRadioButton *m_systemBrowserRadioButton;
    QRadioButton *m_fileRadioButton;
};



class CBookmarkImportSelectSystemBrowserWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    CBookmarkImportSelectSystemBrowserWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
private:
};



class CBookmarkImportSelectFileWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    CBookmarkImportSelectFileWizardPage(QWidget *parent = 0);
    virtual int nextId() const;
private:
};


#endif // CBOOKMARKIMPORTWIZARD_H
