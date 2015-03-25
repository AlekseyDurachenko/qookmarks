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
#ifndef CCOMPOSITWIDGET_H
#define CCOMPOSITWIDGET_H

#include <QWidget>
class CNavigationItemModel;
class CBookmarkFilteredItemModel;
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CNavigationView;
class CBookmarkView;
class CManager;


class CCompositWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCompositWidget(QWidget *parent = 0);
    virtual ~CCompositWidget();

    inline CManager *manager() const;
    void setManager(CManager *manager);
private slots:
    void manager_destroyed();
    void navigation_selection_selectionChanged();
private:
    CNavigationView *m_navigationView;
    CBookmarkView *m_bookmarkView;
    CNavigationItemModel *m_navigationItemModel;
    CBookmarkFilteredItemModel *m_bookmarkItemModel;
    CBookmarkFilterDataModel *m_dataModel;
    CBookmarkFilter *m_filter;
    CManager *m_manager;
};

CManager *CCompositWidget::manager() const
{
    return m_manager;
}


#endif // CCOMPOSITWIDGET_H
