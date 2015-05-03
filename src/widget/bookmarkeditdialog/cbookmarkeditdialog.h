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

    void setTags(const QSet<CTagItem *> &tags);
    const QSet<CTagItem *> checkedTags() const;
private slots:
    void on_toolButton_downloadFavIcon_clicked();
    void faviconReply_finished();
    void on_toolButton_checkHttpStatus_clicked();
    void checkUrlReply_finished();    
    void on_toolButton_downloadPageInformation_clicked();
    void webPageInfoReply_finished();
private:
    Ui::CBookmarkEditDialog *ui;
    CBookmark m_data;
    CDownloadFaviconReply *m_faviconReply;
    CCheckUrlReply *m_checkUrlReply;
    CDownloadWebPageInfoReply *m_webPageInfoReply;
};


#endif // CBOOKMARKEDITDIALOG_H
