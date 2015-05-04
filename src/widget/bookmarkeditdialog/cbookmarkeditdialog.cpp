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
    data.setHttpResponseCode(ui->spinBox_httpResponseCode->value());
    data.setHttpResponseText(ui->lineEdit_httpResponseText->text());
    data.setLastCheckDateTime(ui->dateTimeEdit_lastCheck->dateTime());
    data.setNote(ui->plainTextEdit_comments->toPlainText());
    data.setCreatedDateTime(ui->dateTimeEdit_metadata_created->dateTime());
    data.setEditedDateTime(ui->dateTimeEdit_metadata_edited->dateTime());
    data.setLastVisitedDateTime(ui->dateTimeEdit_metadata_lastVisited->dateTime());
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
    ui->spinBox_httpResponseCode->setValue(data.httpResponseCode());
    ui->lineEdit_httpResponseText->setText(data.httpResponseText());
    ui->dateTimeEdit_lastCheck->setDateTime(data.lastCheckDateTime());
    // TODO: show screenshot if exists
    ui->plainTextEdit_comments->setPlainText(data.note());
    ui->dateTimeEdit_metadata_created->setDateTime(data.createdDateTime());
    ui->dateTimeEdit_metadata_edited->setDateTime(data.editedDateTime());
    ui->dateTimeEdit_metadata_lastVisited->setDateTime(data.lastVisitedDateTime());
}

void CBookmarkEditDialog::setTags(const QSet<CTagItem *> &tags)
{
    ui->treeView_tags->checkedTagItemModel()->setCheckedTags(tags);
}

const QSet<CTagItem *> CBookmarkEditDialog::checkedTags() const
{
    return ui->treeView_tags->checkedTagItemModel()->checkedTags();
}

void CBookmarkEditDialog::on_toolButton_downloadFavIcon_clicked()
{
    CDownloadFaviconRequest request(ui->lineEdit_url->text());
    m_faviconReply = GNetworkMgr()->favicon(request);
    connect(m_faviconReply, SIGNAL(finished()),
            this, SLOT(faviconReply_finished()));
}

void CBookmarkEditDialog::faviconReply_finished()
{
    qDebug() << m_faviconReply->error() << m_faviconReply->errorString();
    ui->toolButton_downloadFavIcon->setIcon(m_faviconReply->favicon());
    m_faviconReply->deleteLater();
    m_faviconReply = 0;
}

void CBookmarkEditDialog::on_toolButton_checkHttpStatus_clicked()
{
    CCheckUrlRequest request(ui->lineEdit_url->text());
    m_checkUrlReply = GNetworkMgr()->checkUrl(request);
    connect(m_checkUrlReply, SIGNAL(finished()),
            this, SLOT(checkUrlReply_finished()));
}

void CBookmarkEditDialog::checkUrlReply_finished()
{
    qDebug() << m_checkUrlReply->error() << m_checkUrlReply->errorString();
    ui->spinBox_httpResponseCode->setValue(m_checkUrlReply->httpStatusCode());
    ui->lineEdit_httpResponseText->setText(m_checkUrlReply->httpReasonPhrase());
    ui->dateTimeEdit_lastCheck->setDateTime(QDateTime::currentDateTime());
    m_checkUrlReply->deleteLater();
    m_checkUrlReply = 0;
}

void CBookmarkEditDialog::on_toolButton_downloadPageInformation_clicked()
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
