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
#ifndef BOOKMARKEDITDIALOG_H
#define BOOKMARKEDITDIALOG_H

#include "bookmarkitem.h"
#include "singleton.h"
#include <QDialog>
class TagSortFilterProxyModel;
class CheckedTagItemModel;


namespace Ui {
class BookmarkEditDialog;
}

class BookmarkEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BookmarkEditDialog(QWidget *parent = 0);
    virtual ~BookmarkEditDialog();

    Bookmark toData() const;
    void setData(const Bookmark &toData);

    const QSet<TagItem *> toCheckedTags() const;
    void setCheckedTags(const QSet<TagItem *> &toCheckedTags);

    QIcon toFavicon() const;
    void setFavicon(const QIcon &icon) const;

    bool isAddToDownloadQueue() const;
    void setAddToDownloadQueue(bool state);
private slots:
    void on_favicon_toolButton_clicked();
    void faviconReply_finished();

    void on_httpCheck_toolButton_clicked();
    void httpCheckReply_finished();

    void on_pageinfo_toolButton_clicked();
    void pageinfoReply_finished();

    void on_url_lineEdit_textChanged(const QString &text);
    void on_showExtendedOptions_toolButton_toggled(bool checked);
    void on_tagFind_lineEdit_textChanged(const QString &text);

    void on_loadFromFile_toolButton_clicked();
    void on_saveToFile_toolButton_clicked();
    void on_clear_toolButton_clicked();
    void on_textWrap_toolButton_toggled(bool checked);
private:
    void readSettings();
    void writeSettings();
private:
    Ui::BookmarkEditDialog *ui;
    CheckedTagItemModel *m_checkedTagItemModel;
    TagSortFilterProxyModel *m_tagSortFilterProxyModel;

    Bookmark m_data;

    CDownloadFaviconReply *m_faviconReply;
    CDownloadWebPageInfoReply *m_pageinfoReply;
    CCheckUrlReply *m_httpCheckReply;
};


#endif // BOOKMARKEDITDIALOG_H
