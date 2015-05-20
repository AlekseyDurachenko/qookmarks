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
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include "ccheckedtagitemmodel.h"
#include "ctagsortfilterproxymodel.h"
#include "cdownloadfaviconreply.h"
#include "ccheckurlreply.h"
#include "cdownloadwebpageinforeply.h"
#include "ciconmgr.h"
#include "settings.h"
#include "icontheme.h"
#include <QDebug>


CBookmarkEditDialog::CBookmarkEditDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CBookmarkEditDialog)
{
    ui->setupUi(this);

    // set icons
    ui->checkBox_favorite->setIcon(IconTheme::icon("anchor-bookmark-favorite"));
    ui->checkBox_readItLater->setIcon(IconTheme::icon("anchor-bookmark-readitlater"));
    ui->checkBox_trash->setIcon(IconTheme::icon("anchor-bookmark-trash"));
    ui->checkBox_addToDownloadQueue->setIcon(IconTheme::icon("action-download"));
    ui->toolButton_pageinfo->setIcon(IconTheme::icon("action-download-url-metadata"));
    ui->toolButton_httpCheck->setIcon(IconTheme::icon("action-check-url"));
    ui->toolButton_loadFromFile->setIcon(IconTheme::icon("action-note-open"));
    ui->toolButton_saveToFile->setIcon(IconTheme::icon("action-note-save-as"));
    ui->toolButton_clear->setIcon(IconTheme::icon("action-note-erase"));
    ui->toolButton_textWrap->setIcon(IconTheme::icon("action-note-text-wrap"));

    // configure the checked tag treeview
    m_checkedTagItemModel = new CCheckedTagItemModel(this);
    m_tagSortFilterProxyModel = new CTagSortFilterProxyModel(this);
    m_tagSortFilterProxyModel->setSourceModel(m_checkedTagItemModel);
    m_tagSortFilterProxyModel->setDynamicSortFilter(true);
    m_tagSortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->treeView_checkedTags->setModel(m_tagSortFilterProxyModel);
    ui->treeView_checkedTags->setSortingEnabled(true);
    ui->treeView_checkedTags->sortByColumn(0, Qt::AscendingOrder);
    ui->treeView_checkedTags->setHeaderHidden(true);

#if QT_VERSION >= 0x050200
    ui->lineEdit_tagFind->setClearButtonEnabled(true);
#endif
    ui->treeView_checkedTags->expandAll();

    m_faviconReply = 0;
    m_pageinfoReply = 0;
    m_httpCheckReply = 0;

    on_lineEdit_url_textChanged(ui->lineEdit_url->text());
    readSettings();
}

CBookmarkEditDialog::~CBookmarkEditDialog()
{
    writeSettings();

    if (m_faviconReply)
    {
        disconnect(m_faviconReply, 0, this, 0);
        m_faviconReply->abort();
        m_faviconReply->deleteLater();
    }

    if (m_pageinfoReply)
    {
        disconnect(m_pageinfoReply, 0, this, 0);
        m_pageinfoReply->abort();
        m_pageinfoReply->deleteLater();
    }

    if (m_httpCheckReply)
    {
        disconnect(m_httpCheckReply, 0, this, 0);
        m_httpCheckReply->abort();
        m_httpCheckReply->deleteLater();
    }

    delete ui;
}

CBookmark CBookmarkEditDialog::toData() const
{
    CBookmark data;
    data.setUrl(ui->lineEdit_url->text());
    data.setTitle(ui->lineEdit_title->text());
    data.setDescription(ui->lineEdit_description->text());
    data.setKeywords(ui->lineEdit_keywords->text());
    data.setRating(ui->spinBox_rating->value());
    data.setFavorite(ui->checkBox_favorite->isChecked());
    data.setReadItLater(ui->checkBox_readItLater->isChecked());
    data.setTrash(ui->checkBox_trash->isChecked());
    data.setHttpStatusCode(ui->spinBox_httpStatusCode->value());
    data.setHttpReasonPhrase(ui->lineEdit_httpReasonPhrase->text());
    data.setHttpCheckDateTime(ui->dateTimeEdit_httpCheck->dateTime());
    data.setNotes(ui->plainTextEdit_notes->toPlainText());
    data.setCreatedDateTime(ui->dateTimeEdit_created->dateTime());
    data.setEditedDateTime(ui->dateTimeEdit_edited->dateTime());
    data.setLastVisitedDateTime(ui->dateTimeEdit_lastVisited->dateTime());
    data.setVisitCount(ui->spinBox_visitCount->value());

    return data;
}

void CBookmarkEditDialog::setData(const CBookmark &data)
{
    ui->lineEdit_url->setText(data.url().toString());
    ui->lineEdit_title->setText(data.title());
    ui->lineEdit_description->setText(data.description());
    ui->lineEdit_keywords->setText(data.keywords());
    ui->spinBox_rating->setValue(data.rating());
    ui->checkBox_favorite->setChecked(data.isFavorite());
    ui->checkBox_readItLater->setChecked(data.isReadItLater());
    ui->checkBox_trash->setChecked(data.isTrash());
    ui->spinBox_httpStatusCode->setValue(data.httpStatusCode());
    ui->lineEdit_httpReasonPhrase->setText(data.httpReasonPhrase());
    ui->dateTimeEdit_httpCheck->setDateTime(data.httpCheckDateTime());
    ui->plainTextEdit_notes->setPlainText(data.notes());
    ui->dateTimeEdit_created->setDateTime(data.createdDateTime());
    ui->dateTimeEdit_edited->setDateTime(data.editedDateTime());
    ui->dateTimeEdit_lastVisited->setDateTime(data.lastVisitedDateTime());
    ui->spinBox_visitCount->setValue(data.visitCount());

    m_data = data;
}

void CBookmarkEditDialog::setCheckedTags(const QSet<CTagItem *> &checkedTags)
{
    m_checkedTagItemModel->setCheckedTags(checkedTags);
    ui->treeView_checkedTags->expandAll();
}

QIcon CBookmarkEditDialog::toFavicon() const
{
    return ui->toolButton_favicon->icon();
}

void CBookmarkEditDialog::setFavicon(const QIcon &icon) const
{
    ui->toolButton_favicon->setIcon(icon);
}

bool CBookmarkEditDialog::isAddToDownloadQueue() const
{
    return ui->checkBox_addToDownloadQueue->isChecked();
}

void CBookmarkEditDialog::setAddToDownloadQueue(bool state)
{
    ui->checkBox_addToDownloadQueue->setChecked(state);
}

const QSet<CTagItem *> CBookmarkEditDialog::toCheckedTags() const
{
    return m_checkedTagItemModel->checkedTags();
}

void CBookmarkEditDialog::on_toolButton_favicon_clicked()
{
    ui->toolButton_favicon->setEnabled(false);

    CDownloadFaviconRequest request(ui->lineEdit_url->text());
    m_faviconReply = GNetworkMgr()->favicon(request);
    connect(m_faviconReply, SIGNAL(finished()),
            this, SLOT(faviconReply_finished()));
}

void CBookmarkEditDialog::faviconReply_finished()
{
    ui->toolButton_favicon->setEnabled(true);

    ui->toolButton_favicon->setIcon(m_faviconReply->favicon());

    m_faviconReply->deleteLater();
    m_faviconReply = 0;
}

void CBookmarkEditDialog::on_toolButton_httpCheck_clicked()
{
    ui->toolButton_httpCheck->setEnabled(false);
    ui->spinBox_httpStatusCode->setEnabled(false);
    ui->lineEdit_httpReasonPhrase->setEnabled(false);
    ui->dateTimeEdit_httpCheck->setEnabled(false);

    CCheckUrlRequest request(ui->lineEdit_url->text());
    m_httpCheckReply = GNetworkMgr()->checkUrl(request);
    connect(m_httpCheckReply, SIGNAL(finished()),
            this, SLOT(httpCheckReply_finished()));
}

void CBookmarkEditDialog::httpCheckReply_finished()
{
    ui->toolButton_httpCheck->setEnabled(true);
    ui->spinBox_httpStatusCode->setEnabled(true);
    ui->lineEdit_httpReasonPhrase->setEnabled(true);
    ui->dateTimeEdit_httpCheck->setEnabled(true);

    if (m_httpCheckReply->error() == CAbstractDownloadReply::NoError)
    {
        ui->spinBox_httpStatusCode->setValue(m_httpCheckReply->httpStatusCode());
        ui->lineEdit_httpReasonPhrase->setText(m_httpCheckReply->httpReasonPhrase());
        ui->dateTimeEdit_httpCheck->setDateTime(QDateTime::currentDateTime());
    }
    else if (m_httpCheckReply->error() == CAbstractDownloadReply::NetworkError)
    {
        switch (m_httpCheckReply->networkError())
        {
        case QNetworkReply::ContentNotFoundError:
            ui->spinBox_httpStatusCode->setValue(404);
            ui->lineEdit_httpReasonPhrase->setText(tr("The remote content was not found at the server"));
            ui->dateTimeEdit_httpCheck->setDateTime(QDateTime::currentDateTime());
            break;
        default:
            ;
        }
    }

    m_httpCheckReply->deleteLater();
    m_httpCheckReply = 0;
}

void CBookmarkEditDialog::on_toolButton_pageinfo_clicked()
{
    ui->toolButton_pageinfo->setEnabled(false);
    ui->lineEdit_title->setEnabled(false);
    ui->lineEdit_description->setEnabled(false);
    ui->lineEdit_keywords->setEnabled(false);

    CDownloadWebPageInfoRequest request(ui->lineEdit_url->text());
    m_pageinfoReply = GNetworkMgr()->webPageInfo(request);
    connect(m_pageinfoReply, SIGNAL(finished()),
            this, SLOT(pageinfoReply_finished()));
}

void CBookmarkEditDialog::pageinfoReply_finished()
{
    ui->toolButton_pageinfo->setEnabled(true);
    ui->lineEdit_title->setEnabled(true);
    ui->lineEdit_description->setEnabled(true);
    ui->lineEdit_keywords->setEnabled(true);

    if (m_pageinfoReply->error() == CAbstractDownloadReply::NoError
            && m_pageinfoReply->mimeType() == "text/html")
    {
        ui->lineEdit_title->setText(m_pageinfoReply->title());
        ui->lineEdit_description->setText(m_pageinfoReply->description());
        ui->lineEdit_keywords->setText(m_pageinfoReply->keywords());
    }

    m_pageinfoReply->deleteLater();
    m_pageinfoReply = 0;
}

void CBookmarkEditDialog::on_lineEdit_url_textChanged(const QString &text)
{
    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (text.isEmpty() || !QUrl(text).isValid())
    {
        button->setEnabled(false);
    }
    else
    {
        button->setEnabled(true);
    }

    if (button->isEnabled())
    {
        ui->lineEdit_url->setPalette(QPalette());
    }
    else
    {
        QPalette palette = ui->lineEdit_url->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->lineEdit_url->setPalette(palette);
    }
}

void CBookmarkEditDialog::on_toolButton_showExtendedOptions_toggled(bool checked)
{
    if (checked)
    {
        ui->toolButton_showExtendedOptions->setArrowType(Qt::DownArrow);
        ui->widget_extendedOptions->setVisible(true);
    }
    else
    {
        ui->toolButton_showExtendedOptions->setArrowType(Qt::RightArrow);
        ui->widget_extendedOptions->setVisible(false);
    }
}

void CBookmarkEditDialog::on_lineEdit_tagFind_textChanged(const QString &text)
{
    m_tagSortFilterProxyModel->setFilterFixedString(text);
    ui->treeView_checkedTags->expandAll();
}

void CBookmarkEditDialog::on_toolButton_loadFromFile_clicked()
{
    G_SETTINGS_INIT();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open notes"),
                settings.value("lastNotesDirectory", "").toString(),
                tr("Text (*.txt)"));

    if (fileName.isEmpty())
        return;

    try
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            throw file.errorString();

        ui->plainTextEdit_notes->setPlainText(QString::fromUtf8(file.readAll()));
    }
    catch (const QString &error)
    {
        QMessageBox::critical(this, tr("Critical"), error);
    }

    settings.setValue("lastNotesDirectory", QFileInfo(fileName).absolutePath());
}

void CBookmarkEditDialog::on_toolButton_saveToFile_clicked()
{
    G_SETTINGS_INIT();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save notes"),
                settings.value("lastNotesDirectory", "").toString()
                    + QDir::separator() + "note.txt",
                tr("Text (*.txt)"));

    if (fileName.isEmpty())
        return;

    try
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            throw file.errorString();

        QByteArray content = ui->plainTextEdit_notes->toPlainText().toUtf8();
        if (file.write(content) != content.size())
            throw file.errorString();
    }
    catch (const QString &error)
    {
        QMessageBox::critical(this, tr("Critical"), error);
    }

    settings.setValue("lastNotesDirectory", QFileInfo(fileName).absolutePath());
}

void CBookmarkEditDialog::on_toolButton_clear_clicked()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to clear the text?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    ui->plainTextEdit_notes->clear();
}

void CBookmarkEditDialog::on_toolButton_textWrap_toggled(bool checked)
{
    if (checked)
        ui->plainTextEdit_notes->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    else
        ui->plainTextEdit_notes->setLineWrapMode(QPlainTextEdit::NoWrap);
}

void CBookmarkEditDialog::readSettings()
{
    G_SETTINGS_INIT();
    ui->toolButton_textWrap->setChecked(
                settings.value("CBookmarkEditDialog/toolButton_textWrap/checked",
                               false).toBool());
    ui->toolButton_showExtendedOptions->setChecked(
                settings.value("CBookmarkEditDialog/toolButton_showExtendedOptions/checked",
                               false).toBool());
}

void CBookmarkEditDialog::writeSettings()
{
    G_SETTINGS_INIT();
    settings.setValue("CBookmarkEditDialog/toolButton_textWrap/checked",
                      ui->toolButton_textWrap->isChecked());
    settings.setValue("CBookmarkEditDialog/toolButton_showExtendedOptions/checked",
                      ui->toolButton_showExtendedOptions->isChecked());
}
