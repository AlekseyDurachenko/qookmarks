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
#include "cbookmarkeditdialog.h"
#include "ui_cbookmarkeditdialog.h"
#include "cdownloadfaviconreply.h"
#include "ccheckurlreply.h"
#include "cdownloadwebpageinforeply.h"
#include <QDebug>


CBookmarkEditDialog::CBookmarkEditDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CBookmarkEditDialog)
{
    ui->setupUi(this);
//    m_model = new CCheckableTagItemModel(this);
//    ui->treeView_tags->setModel(m_model);
}

CBookmarkEditDialog::~CBookmarkEditDialog()
{
    delete ui;
}

CBookmark CBookmarkEditDialog::toData() const
{
    CBookmark data;
    data.setUrl(ui->lineEdit_url->text());
    //data.setFavicon(m_data.favicon()); // HACK
    data.setTitle(ui->lineEdit_title->text());
    data.setDescription(ui->lineEdit_description->text());
    data.setKeywords(ui->lineEdit_keywords->text().split(",").toSet());
    data.setFavorite(ui->checkBox_favorite->isChecked());
    data.setReadLater(ui->checkBox_readItLater->isChecked());
    data.setRating(ui->spinBox_rating->value());
    data.setHttpResponseCode(ui->spinBox_httpStatusCode->value());
    data.setHttpResponseText(ui->lineEdit_httpReasonPhrase->text());
    data.setLastCheckDateTime(ui->dateTimeEdit_httpCheck->dateTime());
    data.setNote(ui->plainTextEdit_notes->toPlainText());
    data.setCreatedDateTime(ui->dateTimeEdit_created->dateTime());
    data.setEditedDateTime(ui->dateTimeEdit_edited->dateTime());
    data.setLastVisitedDateTime(ui->dateTimeEdit_lastVisited->dateTime());
    return data;
}

void CBookmarkEditDialog::setData(const CBookmark &data)
{
    m_data = data;

    ui->lineEdit_url->setText(data.url().toString());
    //if (data.favicon().isNull())
    //    ui->faviconPlace->setPixmap(QIcon(":/icons/bookmark-item.png").pixmap(24, 24));
    //else
    //    ui->faviconPlace->setPixmap(data.favicon().pixmap(24, 24));
    // TODO: set font && color for example text
    ui->lineEdit_title->setText(data.title());
    ui->lineEdit_description->setText(data.description());
    ui->lineEdit_keywords->setText(QStringList(data.keywords().toList()).join(","));
    ui->checkBox_favorite->setChecked(data.isFavorite());
    ui->checkBox_readItLater->setChecked(data.isReadLater());
    ui->spinBox_rating->setValue(data.rating());
    ui->spinBox_httpStatusCode->setValue(data.httpResponseCode());
    ui->lineEdit_httpReasonPhrase->setText(data.httpResponseText());
    ui->dateTimeEdit_httpCheck->setDateTime(data.lastCheckDateTime());
    // TODO: show screenshot if exists
    ui->plainTextEdit_notes->setPlainText(data.note());
    ui->dateTimeEdit_created->setDateTime(data.createdDateTime());
    ui->dateTimeEdit_edited->setDateTime(data.editedDateTime());
    ui->dateTimeEdit_lastVisited->setDateTime(data.lastVisitedDateTime());
}

void CBookmarkEditDialog::setCheckedTags(const QSet<CTagItem *> &checkedTags)
{
    ui->treeView_checkedTags->checkedTagItemModel()->setCheckedTags(checkedTags);
}

const QSet<CTagItem *> CBookmarkEditDialog::checkedTags() const
{
    return ui->treeView_checkedTags->checkedTagItemModel()->checkedTags();
}

void CBookmarkEditDialog::on_toolButton_favicon_clicked()
{
    CDownloadFaviconRequest request(ui->lineEdit_url->text());
    m_faviconReply = GNetworkMgr()->favicon(request);
    connect(m_faviconReply, SIGNAL(finished()),
            this, SLOT(faviconReply_finished()));
}

void CBookmarkEditDialog::faviconReply_finished()
{
    qDebug() << m_faviconReply->error() << m_faviconReply->errorString();
    ui->toolButton_favicon->setIcon(m_faviconReply->favicon());
    m_faviconReply->deleteLater();
    m_faviconReply = 0;
}

void CBookmarkEditDialog::on_toolButton_httpCheck_clicked()
{
    CCheckUrlRequest request(ui->lineEdit_url->text());
    m_checkUrlReply = GNetworkMgr()->checkUrl(request);
    connect(m_checkUrlReply, SIGNAL(finished()),
            this, SLOT(checkUrlReply_finished()));
}

void CBookmarkEditDialog::checkUrlReply_finished()
{
    qDebug() << m_checkUrlReply->error() << m_checkUrlReply->errorString();
    ui->spinBox_httpStatusCode->setValue(m_checkUrlReply->httpStatusCode());
    ui->lineEdit_httpReasonPhrase->setText(m_checkUrlReply->httpReasonPhrase());
    ui->dateTimeEdit_httpCheck->setDateTime(QDateTime::currentDateTime());
    m_checkUrlReply->deleteLater();
    m_checkUrlReply = 0;
}

void CBookmarkEditDialog::on_toolButton_pageinfo_clicked()
{
    CDownloadWebPageInfoRequest request(ui->lineEdit_url->text());
    m_webPageInfoReply = GNetworkMgr()->webPageInfo(request);
    connect(m_webPageInfoReply, SIGNAL(finished()),
            this, SLOT(webPageInfoReply_finished()));
}

void CBookmarkEditDialog::webPageInfoReply_finished()
{
    qDebug() << m_webPageInfoReply->error() << m_webPageInfoReply->errorString();
    ui->lineEdit_title->setText(m_webPageInfoReply->title());
    ui->lineEdit_description->setText(m_webPageInfoReply->description());
    ui->lineEdit_keywords->setText(m_webPageInfoReply->keywords());
    m_webPageInfoReply->deleteLater();
    m_webPageInfoReply = 0;
}
