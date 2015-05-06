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
#include "cbookmarkheaderview.h"


CBookmarkHeaderView::CBookmarkHeaderView(Qt::Orientation orientation,
        QWidget *parent) : QHeaderView(orientation, parent)
{
}

QSize CBookmarkHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize size = QHeaderView::sectionSizeFromContents(logicalIndex);

    if (isSortIndicatorShown())
    {
        int margin = style()->pixelMetric(QStyle::PM_HeaderMargin);
        if (orientation() == Qt::Horizontal)
            size.rwidth() -= size.height() + margin;
        else
            size.rheight() -= size.width() + margin;
    }

    return size;
}
