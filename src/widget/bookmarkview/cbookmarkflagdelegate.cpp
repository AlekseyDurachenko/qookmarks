// Copyright 2014-2015, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "cbookmarkflagdelegate.h"
#include "cbookmarkitem.h"
#include <QPainter>


static CBookmarkItem *indexToItem(const QModelIndex &index)
{
    return static_cast<CBookmarkItem *>(index.data(Qt::UserRole).value<void *>());
}

CBookmarkFlagDelegate::CBookmarkFlagDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    m_favoriteIconOn  = QIcon(":/icons/bookmark-favorites.png");
    m_favoriteIconOff = QIcon(":/icons/bookmark-noflag.png");

    m_readItLaterIconOn  = QIcon(":/icons/bookmark-readlater.png");
    m_readItLaterIconOff = QIcon(":/icons/bookmark-noflag.png");

    m_trashIconOn  = QIcon(":/icons/bookmark-trash.png");
    m_trashIconOff = QIcon(":/icons/bookmark-noflag.png");
}

void CBookmarkFlagDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    drawFocus(painter, option, option.rect);

    int size = qMin(option.rect.width(), option.rect.height());
    CBookmarkItem *item = indexToItem(index);


    painter->drawPixmap(option.rect.left(),
                        option.rect.top(),
                        (item->data().isFavorite() ? m_favoriteIconOn
                                                   : m_favoriteIconOff)
                            .pixmap(QSize(size, size)));
    painter->drawPixmap(option.rect.left() + size/3   + size,
                        option.rect.top(),
                        (item->data().isFavorite() ? m_readItLaterIconOn
                                                   : m_readItLaterIconOff)
                            .pixmap(QSize(size, size)));
    painter->drawPixmap(option.rect.left() + size/3*2 + size*2,
                        option.rect.top(),
                        (item->data().isTrash() ? m_trashIconOn
                                                : m_trashIconOff)
                            .pixmap(QSize(size, size)));
}

QSize CBookmarkFlagDelegate::sizeHint(const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    size.setWidth(qMin(option.rect.width(), option.rect.height())*4);
    return size;
}
