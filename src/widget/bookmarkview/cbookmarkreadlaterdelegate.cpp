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
#include "cbookmarkreadlaterdelegate.h"
#include "cbookmarkitem.h"
#include <QPainter>
#include <QDebug>


static CBookmarkItem *indexToItem(const QModelIndex &index)
{
    return static_cast<CBookmarkItem *>(index.data(Qt::UserRole).value<void *>());
}

CBookmarkReadLaterDelegate::CBookmarkReadLaterDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void CBookmarkReadLaterDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    drawFocus(painter, option, option.rect);

    CBookmarkItem *item = indexToItem(index);
    int size = qMin(option.rect.width(), option.rect.height());

    if (item->data().isReadLater())
    {
        painter->drawPixmap(option.rect.left() + (option.rect.width()-size)/2,
                            option.rect.top() + (option.rect.height()-size)/2,
                            QIcon(":/icons/bookmark-readlater.png")
                                    .pixmap(QSize(size, size)));
    }
    else
    {
        painter->drawPixmap(option.rect.left() + (option.rect.width()-size)/2,
                            option.rect.top() + (option.rect.height()-size)/2,
                            QIcon(":/icons/bookmark-readlater.png")
                                    .pixmap(QSize(size, size), QIcon::Disabled));
    }
}
