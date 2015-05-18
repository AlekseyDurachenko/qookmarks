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
#include "icontheme.h"


QHash<QByteArray, QIcon> IconTheme::m_icons;


void IconTheme::init()
{
    m_icons.insert("anchor-bookmark-all",                   QIcon(":/icons/anchor-bookmark-all.png"));
    m_icons.insert("anchor-bookmark-untagged",              QIcon(":/icons/anchor-bookmark-untagged.png"));
    m_icons.insert("anchor-bookmark-favorite",              QIcon(":/icons/anchor-bookmark-favorite.png"));
    m_icons.insert("anchor-bookmark-favorite-disabled",     QIcon(":/icons/anchor-bookmark-favorite-disabled.png"));
    m_icons.insert("anchor-bookmark-readitlater",           QIcon(":/icons/anchor-bookmark-readitlater.png"));
    m_icons.insert("anchor-bookmark-readitlater-disabled",  QIcon(":/icons/anchor-bookmark-readitlater-disabled.png"));
    m_icons.insert("anchor-bookmark-rated",                 QIcon(":/icons/anchor-bookmark-rated.png"));
    m_icons.insert("anchor-bookmark-trash",                 QIcon(":/icons/anchor-bookmark-trash.png"));
    m_icons.insert("anchor-bookmark-trash-disabled",        QIcon(":/icons/anchor-bookmark-trash-disabled.png"));
    m_icons.insert("anchor-bookmark-trash-full",            QIcon(":/icons/anchor-bookmark-trash-full.png"));
    m_icons.insert("rating-bookmark-star",                  QIcon(":/icons/rating-bookmark-star.png"));
    m_icons.insert("rating-bookmark-star-half-disabled",    QIcon(":/icons/rating-bookmark-star-half-disabled.png"));
    m_icons.insert("rating-bookmark-star-disabled",         QIcon(":/icons/rating-bookmark-star-disabled.png"));
    m_icons.insert("favicon-default",   QIcon(":/icons/favicon-default.png"));
    m_icons.insert("tag-icon-default",              QIcon(":/icons/tag-icon-default.png"));
    m_icons.insert("action-note-erase",     QIcon(":/icons/action-note-erase.png"));
    m_icons.insert("action-note-open",      QIcon(":/icons/action-note-open.png"));
    m_icons.insert("action-note-save-as",   QIcon(":/icons/action-note-save-as.png"));
    m_icons.insert("action-note-text-wrap", QIcon(":/icons/action-note-text-wrap.png"));
    m_icons.insert("action-about",                  QIcon(":/icons/action-about.png"));
    m_icons.insert("action-about-qt",               QIcon(":/icons/action-about-qt.png"));
    m_icons.insert("action-check-url",              QIcon(":/icons/action-check-url.png"));
    m_icons.insert("action-download",               QIcon(":/icons/action-download.png"));
    m_icons.insert("action-download-url-metadata",  QIcon(":/icons/action-download-url-metadata.png"));
    m_icons.insert("action-quit",                   QIcon(":/icons/action-quit.png"));
    m_icons.insert("action-empty-trash",            QIcon(":/icons/action-empty-trash.png"));
    m_icons.insert("action-bookmark-select-all",    QIcon(":/icons/action-bookmark-select-all.png"));
    m_icons.insert("action-bookmark-add",           QIcon(":/icons/action-bookmark-add.png"));
    m_icons.insert("action-bookmark-edit",          QIcon(":/icons/action-bookmark-edit.png"));
    m_icons.insert("action-bookmark-send-to-trash", QIcon(":/icons/action-bookmark-send-to-trash.png"));
    m_icons.insert("action-bookmark-restore",       QIcon(":/icons/action-bookmark-restore.png"));
    m_icons.insert("action-bookmark-remove",        QIcon(":/icons/action-bookmark-remove.png"));
    m_icons.insert("action-tag-add",                QIcon(":/icons/action-tag-add.png"));
    m_icons.insert("action-tag-edit",               QIcon(":/icons/action-tag-edit.png"));
    m_icons.insert("action-tag-remove",             QIcon(":/icons/action-tag-remove.png"));
    m_icons.insert("browser-default",               QIcon(":/icons/browser-default.png"));
    m_icons.insert("browser-chromium",              QIcon(":/icons/browser-chromium.png"));
    m_icons.insert("browser-firefox",               QIcon(":/icons/browser-firefox.png"));
}

const QIcon IconTheme::icon(const QByteArray &name, const QIcon &defaultIcon)
{
    return m_icons.value(name, defaultIcon);
}
