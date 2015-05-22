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
#include "bookmarkratingdelegate.h"
#include "bookmarkitem.h"
#include "consts.h"
#include "icontheme.h"
#include <QHeaderView>
#include <QPainter>
#include <QTreeView>


BookmarkRatingDelegate::BookmarkRatingDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    m_starIcon = IconTheme::icon("rating-bookmark-star");
    m_halfIcon = IconTheme::icon("rating-bookmark-star-half-disabled");
    m_disabledIcon = IconTheme::icon("rating-bookmark-star-disabled");
}

void BookmarkRatingDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    drawFocus(painter, option, option.rect);

    const int rating = index.data(Qt::DisplayRole).toInt();
    const int size = sizeHint(option, index).height();
    const int top  = option.rect.top();
    int left = option.rect.left();
    int i = 2;

    for (; i <= rating; i += 2, left += size)
        painter->drawPixmap(left, top, m_starIcon.pixmap(QSize(size, size)));

    if (rating % 2 != 0)
    {
        painter->drawPixmap(left, top, m_halfIcon.pixmap(QSize(size, size)));
        i += 2;
        left += size;
    }

    for (; i <= BookmarkMaxRating; i +=2, left += size)
        painter->drawPixmap(left, top, m_disabledIcon.pixmap(QSize(size, size)));
}

QSize BookmarkRatingDelegate::sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    QSize size = QItemDelegate::sizeHint(option, index);
    size.setWidth(size.height()*5+size.height()/2);
    return size;
}
