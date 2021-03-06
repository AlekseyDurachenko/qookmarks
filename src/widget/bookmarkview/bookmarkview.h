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
#ifndef BOOKMARKVIEW_H
#define BOOKMARKVIEW_H

#include <QTreeView>
class QSortFilterProxyModel;
class BookmarkItemModel;
class BookmarkItem;


class BookmarkView : public QTreeView
{
    Q_OBJECT
public:
    BookmarkView(QWidget *parent = 0);

    inline BookmarkItemModel *bookmarkModel() const;
    void setBookmarkModel(BookmarkItemModel *model);

    QList<BookmarkItem *> selectedBookmarks() const;
    QList<QUrl> selectedUrls() const;
public slots:
    void setFilterFixedString(const QString &filter);
protected:
    virtual void setModel(QAbstractItemModel *model);
private:
    BookmarkItemModel *m_bookmarkItemModel;
    QSortFilterProxyModel *m_sortFilterProxyModel;
};

BookmarkItemModel *BookmarkView::bookmarkModel() const
{
    return m_bookmarkItemModel;
}


#endif // CBOOKMARKVIEW_H
