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
#ifndef CBOOKMARKEDITDIALOG_H
#define CBOOKMARKEDITDIALOG_H

#include "cbookmarkitem.h"
//#include "ccheckabletagitemmodel.h"
#include <QDialog>
#include "singleton.h"


namespace Ui {
class CBookmarkEditDialog;
}

class CBookmarkEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CBookmarkEditDialog(QWidget *parent = 0);
    virtual ~CBookmarkEditDialog();

    CBookmark toData() const;
    void setData(const CBookmark &toData);

    const QSet<CTagItem *> toCheckedTags() const;
    void setCheckedTags(const QSet<CTagItem *> &toCheckedTags);

    inline const QIcon &favicon() const;

    bool isAddToDownloadQueue() const;
    void setAddToDownloadQueue(bool state);
private slots:
    void on_toolButton_favicon_clicked();
    void faviconReply_finished();
    void on_toolButton_httpCheck_clicked();
    void checkUrlReply_finished();    
    void on_toolButton_pageinfo_clicked();
    void webPageInfoReply_finished();

    void on_toolButton_loadFromFile_clicked();
    void on_toolButton_saveToFile_clicked();
    void on_toolButton_clear_clicked();
    void on_toolButton_textWrap_toggled(bool checked);

    void on_checkBox_autoPageinfo_toggled(bool checked);
    void on_lineEdit_url_textChanged(const QString &test);
    void autoPageinfoTimer_timeout();
private:
    void readSettings();
    void writeSettings();
private:
    Ui::CBookmarkEditDialog *ui;
    QTimer *m_autoPageinfoTimer;

    CBookmark m_data;
    QIcon m_favicon;

    CDownloadWebPageInfoReply *m_pageinfoReply;
    CDownloadFaviconReply *m_faviconReply;
    CCheckUrlReply *m_httpCheckReply;
};

const QIcon &CBookmarkEditDialog::favicon() const
{
    return m_favicon;
}


#endif // CBOOKMARKEDITDIALOG_H
