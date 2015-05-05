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
#include "cbookmarkbooleanicondelegate.h"
#include "cbookmarkitem.h"
#include <QPainter>
#include <QDebug>

CBookmarkBooleanIconDelegate::CBookmarkBooleanIconDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void CBookmarkBooleanIconDelegate::setIconOn(const QIcon &icon)
{
    m_iconOn = icon;
}

void CBookmarkBooleanIconDelegate::setIconOff(const QIcon &icon)
{
    m_iconOff = icon;
}

void CBookmarkBooleanIconDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    drawFocus(painter, option, option.rect);

    int size = qMin(option.rect.width(), option.rect.height());
    if (index.data(Qt::DisplayRole).toBool())
    {
        painter->drawPixmap(option.rect.left(),
                            option.rect.top(),
                            m_iconOn.pixmap(QSize(size, size)));
    }
    else
    {
        painter->drawPixmap(option.rect.left(),
                            option.rect.top(),
                            m_iconOff.pixmap(QSize(size, size)));
    }
}

QSize CBookmarkBooleanIconDelegate::sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    size.setWidth(qMin(option.rect.width(), option.rect.height()));
    return size;
}
