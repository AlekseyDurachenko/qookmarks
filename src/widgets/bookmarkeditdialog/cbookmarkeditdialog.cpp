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


CBookmarkEditDialog::CBookmarkEditDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CBookmarkEditDialog)
{
    ui->setupUi(this);
}

CBookmarkEditDialog::~CBookmarkEditDialog()
{
    delete ui;
}

CBookmarkItemData CBookmarkEditDialog::toData() const
{
    CBookmarkItemData data;
    data.setUrl(ui->lineEdit_url->text());
    data.setFavicon(m_data.favicon()); // HACK
    data.setTitle(ui->lineEdit_title->text());
    data.setDescription(ui->lineEdit_description->text());
    data.setKeywordList(ui->lineEdit_keywords->text().split(","));
    data.setFavorite(ui->checkBox_favorite->isChecked());
    data.setReadLater(ui->checkBox_readItLater->isChecked());
    data.setRating(ui->spinBox_rating->value());
    data.setHttpResponseCode(ui->spinBox_httpResponseCode->value());
    data.setHttpResponseText(ui->lineEdit_httpResponseText->text());
    data.setLastCheckDateTime(ui->dateTimeEdit_lastCheck->dateTime());
    data.setComment(ui->plainTextEdit_comments->toPlainText());
    data.setCreatedDateTime(ui->dateTimeEdit_metadata_created->dateTime());
    data.setEditedDateTime(ui->dateTimeEdit_metadata_edited->dateTime());
    data.setLastVisitedDateTime(ui->dateTimeEdit_metadata_lastVisited->dateTime());
    return data;
}

void CBookmarkEditDialog::setData(const CBookmarkItemData &data)
{
    m_data = data;

    ui->lineEdit_url->setText(data.url().toString());
    if (data.favicon().isNull())
        ui->faviconPlace->setPixmap(QIcon(":/icons/bookmark-item.png").pixmap(24, 24));
    else
        ui->faviconPlace->setPixmap(data.favicon().pixmap(24, 24));
    // TODO: set font && color for example text
    ui->lineEdit_title->setText(data.title());
    ui->lineEdit_description->setText(data.description());
    ui->lineEdit_keywords->setText(data.keywordList().join(","));
    ui->checkBox_favorite->setChecked(data.isFavorite());
    ui->checkBox_readItLater->setChecked(data.isReadLater());
    ui->spinBox_rating->setValue(data.rating());
    // TODO: set tags
    ui->spinBox_httpResponseCode->setValue(data.httpResponseCode());
    ui->lineEdit_httpResponseText->setText(data.httpResponseText());
    ui->dateTimeEdit_lastCheck->setDateTime(data.lastCheckDateTime());
    // TODO: show screenshot if exists
    ui->plainTextEdit_comments->setPlainText(data.comment());
    ui->dateTimeEdit_metadata_created->setDateTime(data.createdDateTime());
    ui->dateTimeEdit_metadata_edited->setDateTime(data.editedDateTime());
    ui->dateTimeEdit_metadata_lastVisited->setDateTime(data.lastVisitedDateTime());
}
