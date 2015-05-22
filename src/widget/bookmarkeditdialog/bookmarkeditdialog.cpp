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
#include "bookmarkeditdialog.h"
#include "ui_cbookmarkeditdialog.h"
#include "ccheckedtagitemmodel.h"
#include "ccheckurlreply.h"
#include "cdownloadfaviconreply.h"
#include "cdownloadwebpageinforeply.h"
#include "ciconmgr.h"
#include "ctagsortfilterproxymodel.h"
#include "icontheme.h"
#include "settings.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QDebug>


BookmarkEditDialog::BookmarkEditDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::BookmarkEditDialog)
{
    ui->setupUi(this);

    // set icons
    ui->favorite_checkBox->setIcon(IconTheme::icon("anchor-bookmark-favorite"));
    ui->readItLater_checkBox->setIcon(IconTheme::icon("anchor-bookmark-readitlater"));
    ui->trash_checkBox->setIcon(IconTheme::icon("anchor-bookmark-trash"));
    ui->addToDownloadQueue_checkBox->setIcon(IconTheme::icon("action-download"));
    ui->pageinfo_toolButton->setIcon(IconTheme::icon("action-download-url-metadata"));
    ui->httpCheck_toolButton->setIcon(IconTheme::icon("action-check-url"));
    ui->loadFromFile_toolButton->setIcon(IconTheme::icon("action-note-open"));
    ui->saveToFile_toolButton->setIcon(IconTheme::icon("action-note-save-as"));
    ui->clear_toolButton->setIcon(IconTheme::icon("action-note-erase"));
    ui->textWrap_toolButton->setIcon(IconTheme::icon("action-note-text-wrap"));

    // configure the checked tag treeview
    m_checkedTagItemModel = new CCheckedTagItemModel(this);
    m_tagSortFilterProxyModel = new CTagSortFilterProxyModel(this);
    m_tagSortFilterProxyModel->setSourceModel(m_checkedTagItemModel);
    m_tagSortFilterProxyModel->setDynamicSortFilter(true);
    m_tagSortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->checkedTags_treeView->setModel(m_tagSortFilterProxyModel);
    ui->checkedTags_treeView->setSortingEnabled(true);
    ui->checkedTags_treeView->sortByColumn(0, Qt::AscendingOrder);
    ui->checkedTags_treeView->setHeaderHidden(true);

#if QT_VERSION >= 0x050200
    ui->tagFind_lineEdit->setClearButtonEnabled(true);
#endif
    ui->checkedTags_treeView->expandAll();

    m_faviconReply = 0;
    m_pageinfoReply = 0;
    m_httpCheckReply = 0;

    on_url_lineEdit_textChanged(ui->url_lineEdit->text());
    readSettings();
}

BookmarkEditDialog::~BookmarkEditDialog()
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

CBookmark BookmarkEditDialog::toData() const
{
    CBookmark data;
    data.setUrl(ui->url_lineEdit->text());
    data.setTitle(ui->title_lineEdit->text());
    data.setDescription(ui->description_lineEdit->text());
    data.setKeywords(ui->keywords_lineEdit->text());
    data.setRating(ui->rating_spinBox->value());
    data.setFavorite(ui->favorite_checkBox->isChecked());
    data.setReadItLater(ui->readItLater_checkBox->isChecked());
    data.setTrash(ui->trash_checkBox->isChecked());
    data.setHttpStatusCode(ui->httpStatusCode_spinBox->value());
    data.setHttpReasonPhrase(ui->httpReasonPhrase_lineEdit->text());
    data.setHttpCheckDateTime(ui->httpCheck_dateTimeEdit->dateTime());
    data.setNotes(ui->notes_plainTextEdit->toPlainText());
    data.setCreatedDateTime(ui->created_dateTimeEdit->dateTime());
    data.setEditedDateTime(ui->edited_dateTimeEdit->dateTime());
    data.setLastVisitedDateTime(ui->lastVisited_dateTimeEdit->dateTime());
    data.setVisitCount(ui->visitCount_spinBox->value());

    return data;
}

void BookmarkEditDialog::setData(const CBookmark &data)
{
    ui->url_lineEdit->setText(data.url().toString());
    ui->title_lineEdit->setText(data.title());
    ui->description_lineEdit->setText(data.description());
    ui->keywords_lineEdit->setText(data.keywords());
    ui->rating_spinBox->setValue(data.rating());
    ui->favorite_checkBox->setChecked(data.isFavorite());
    ui->readItLater_checkBox->setChecked(data.isReadItLater());
    ui->trash_checkBox->setChecked(data.isTrash());
    ui->httpStatusCode_spinBox->setValue(data.httpStatusCode());
    ui->httpReasonPhrase_lineEdit->setText(data.httpReasonPhrase());
    ui->httpCheck_dateTimeEdit->setDateTime(data.httpCheckDateTime());
    ui->notes_plainTextEdit->setPlainText(data.notes());
    ui->created_dateTimeEdit->setDateTime(data.createdDateTime());
    ui->edited_dateTimeEdit->setDateTime(data.editedDateTime());
    ui->lastVisited_dateTimeEdit->setDateTime(data.lastVisitedDateTime());
    ui->visitCount_spinBox->setValue(data.visitCount());

    m_data = data;
}

void BookmarkEditDialog::setCheckedTags(const QSet<CTagItem *> &checkedTags)
{
    m_checkedTagItemModel->setCheckedTags(checkedTags);
    ui->checkedTags_treeView->expandAll();
}

QIcon BookmarkEditDialog::toFavicon() const
{
    return ui->favicon_toolButton->icon();
}

void BookmarkEditDialog::setFavicon(const QIcon &icon) const
{
    ui->favicon_toolButton->setIcon(icon);
}

bool BookmarkEditDialog::isAddToDownloadQueue() const
{
    return ui->addToDownloadQueue_checkBox->isChecked();
}

void BookmarkEditDialog::setAddToDownloadQueue(bool state)
{
    ui->addToDownloadQueue_checkBox->setChecked(state);
}

const QSet<CTagItem *> BookmarkEditDialog::toCheckedTags() const
{
    return m_checkedTagItemModel->checkedTags();
}

void BookmarkEditDialog::on_favicon_toolButton_clicked()
{
    ui->favicon_toolButton->setEnabled(false);

    CDownloadFaviconRequest request(ui->url_lineEdit->text());
    m_faviconReply = GNetworkMgr()->favicon(request);
    connect(m_faviconReply, SIGNAL(finished()),
            this, SLOT(faviconReply_finished()));
}

void BookmarkEditDialog::faviconReply_finished()
{
    ui->favicon_toolButton->setEnabled(true);

    ui->favicon_toolButton->setIcon(m_faviconReply->favicon());

    m_faviconReply->deleteLater();
    m_faviconReply = 0;
}

void BookmarkEditDialog::on_httpCheck_toolButton_clicked()
{
    ui->httpCheck_toolButton->setEnabled(false);
    ui->httpStatusCode_spinBox->setEnabled(false);
    ui->httpReasonPhrase_lineEdit->setEnabled(false);
    ui->httpCheck_dateTimeEdit->setEnabled(false);

    CCheckUrlRequest request(ui->url_lineEdit->text());
    m_httpCheckReply = GNetworkMgr()->checkUrl(request);
    connect(m_httpCheckReply, SIGNAL(finished()),
            this, SLOT(httpCheckReply_finished()));
}

void BookmarkEditDialog::httpCheckReply_finished()
{
    ui->httpCheck_toolButton->setEnabled(true);
    ui->httpStatusCode_spinBox->setEnabled(true);
    ui->httpReasonPhrase_lineEdit->setEnabled(true);
    ui->httpCheck_dateTimeEdit->setEnabled(true);

    if (m_httpCheckReply->error() == CAbstractDownloadReply::NoError)
    {
        ui->httpStatusCode_spinBox->setValue(m_httpCheckReply->httpStatusCode());
        ui->httpReasonPhrase_lineEdit->setText(m_httpCheckReply->httpReasonPhrase());
        ui->httpCheck_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    }
    else if (m_httpCheckReply->error() == CAbstractDownloadReply::NetworkError)
    {
        switch (m_httpCheckReply->networkError())
        {
        case QNetworkReply::ContentNotFoundError:
            ui->httpStatusCode_spinBox->setValue(404);
            ui->httpReasonPhrase_lineEdit->setText(tr("The remote content was not found at the server"));
            ui->httpCheck_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
            break;
        default:
            ;
        }
    }

    m_httpCheckReply->deleteLater();
    m_httpCheckReply = 0;
}

void BookmarkEditDialog::on_pageinfo_toolButton_clicked()
{
    ui->pageinfo_toolButton->setEnabled(false);
    ui->title_lineEdit->setEnabled(false);
    ui->description_lineEdit->setEnabled(false);
    ui->keywords_lineEdit->setEnabled(false);

    CDownloadWebPageInfoRequest request(ui->url_lineEdit->text());
    m_pageinfoReply = GNetworkMgr()->webPageInfo(request);
    connect(m_pageinfoReply, SIGNAL(finished()),
            this, SLOT(pageinfoReply_finished()));
}

void BookmarkEditDialog::pageinfoReply_finished()
{
    ui->pageinfo_toolButton->setEnabled(true);
    ui->title_lineEdit->setEnabled(true);
    ui->description_lineEdit->setEnabled(true);
    ui->keywords_lineEdit->setEnabled(true);

    if (m_pageinfoReply->error() == CAbstractDownloadReply::NoError
            && m_pageinfoReply->mimeType() == "text/html")
    {
        ui->title_lineEdit->setText(m_pageinfoReply->title());
        ui->description_lineEdit->setText(m_pageinfoReply->description());
        ui->keywords_lineEdit->setText(m_pageinfoReply->keywords());
    }

    m_pageinfoReply->deleteLater();
    m_pageinfoReply = 0;
}

void BookmarkEditDialog::on_url_lineEdit_textChanged(const QString &text)
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
        ui->url_lineEdit->setPalette(QPalette());
    }
    else
    {
        QPalette palette = ui->url_lineEdit->palette();
        palette.setColor(QPalette::Text, Qt::red);
        ui->url_lineEdit->setPalette(palette);
    }
}

void BookmarkEditDialog::on_showExtendedOptions_toolButton_toggled(bool checked)
{
    if (checked)
    {
        ui->showExtendedOptions_toolButton->setArrowType(Qt::DownArrow);
        ui->extendedOptions_widget->setVisible(true);
    }
    else
    {
        ui->showExtendedOptions_toolButton->setArrowType(Qt::RightArrow);
        ui->extendedOptions_widget->setVisible(false);
    }
}

void BookmarkEditDialog::on_tagFind_lineEdit_textChanged(const QString &text)
{
    m_tagSortFilterProxyModel->setFilterFixedString(text);
    ui->checkedTags_treeView->expandAll();
}

void BookmarkEditDialog::on_loadFromFile_toolButton_clicked()
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

        ui->notes_plainTextEdit->setPlainText(QString::fromUtf8(file.readAll()));
    }
    catch (const QString &error)
    {
        QMessageBox::critical(this, tr("Critical"), error);
    }

    settings.setValue("lastNotesDirectory", QFileInfo(fileName).absolutePath());
}

void BookmarkEditDialog::on_saveToFile_toolButton_clicked()
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

        QByteArray content = ui->notes_plainTextEdit->toPlainText().toUtf8();
        if (file.write(content) != content.size())
            throw file.errorString();
    }
    catch (const QString &error)
    {
        QMessageBox::critical(this, tr("Critical"), error);
    }

    settings.setValue("lastNotesDirectory", QFileInfo(fileName).absolutePath());
}

void BookmarkEditDialog::on_clear_toolButton_clicked()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to clear the text?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    ui->notes_plainTextEdit->clear();
}

void BookmarkEditDialog::on_textWrap_toolButton_toggled(bool checked)
{
    if (checked)
        ui->notes_plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    else
        ui->notes_plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
}

void BookmarkEditDialog::readSettings()
{
    G_SETTINGS_INIT();
    ui->textWrap_toolButton->setChecked(
                settings.value("CBookmarkEditDialog/toolButton_textWrap/checked",
                               false).toBool());
    ui->showExtendedOptions_toolButton->setChecked(
                settings.value("CBookmarkEditDialog/toolButton_showExtendedOptions/checked",
                               false).toBool());
}

void BookmarkEditDialog::writeSettings()
{
    G_SETTINGS_INIT();
    settings.setValue("CBookmarkEditDialog/toolButton_textWrap/checked",
                      ui->textWrap_toolButton->isChecked());
    settings.setValue("CBookmarkEditDialog/toolButton_showExtendedOptions/checked",
                      ui->showExtendedOptions_toolButton->isChecked());
}
