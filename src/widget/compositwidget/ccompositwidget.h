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
#include <QNetworkAccessManager>
class CNavigationItemModel;
class CNavigationView;
class CBookmarkView;
class CBookmarkFilteredItemModel;
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CPrj;
class CBookmarkItem;


class CCompositWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCompositWidget(CPrj *manager, QWidget *parent = 0);
    virtual ~CCompositWidget();

    inline CBookmarkView *bookmarkView() const;

    inline QAction *actionBookmarkAdd() const;
    inline QAction *actionBookmarkEdit() const;
    inline QAction *actionBookmarkRemove() const;
private slots:
    void navigation_selection_selectionChanged();
    void actionBookmarkAdd_triggered();
    void actionBookmarkEdit_triggered();
    void actionBookmarkRemove_triggered();
    void actionBookmarkScreenshot_triggered();
    void actionBookmarkDownload_triggered();
    void bookmarkView_showContextMenu(const QPoint &pos);
    void updateActions();

    void download_next();
    void download_finishied();

    void screenshot_next();
    void screenshot_finished();
private:
    CNavigationView *m_navigationView;
    CBookmarkView *m_bookmarkView;
    CBookmarkFilter *m_filter;
    CBookmarkFilterDataModel *m_dataModel;
    CBookmarkFilteredItemModel *m_bookmarkItemModel;
    CNavigationItemModel *m_navigationItemModel;
    CPrj *m_project;

    QNetworkAccessManager *m_network;
    QAction *m_actionBookmarkAdd;
    QAction *m_actionBookmarkEdit;
    QAction *m_actionBookmarkRemove;
    QAction *m_actionBookmarkScreenshot;
    QAction *m_actionBookmarkDownload;

    QList<CBookmarkItem *> m_list;
    QList<CBookmarkItem *> m_listDl;
};

CBookmarkView *CCompositWidget::bookmarkView() const
{
    return m_bookmarkView;
}

QAction *CCompositWidget::actionBookmarkAdd() const
{
    return m_actionBookmarkAdd;
}

QAction *CCompositWidget::actionBookmarkEdit() const
{
    return m_actionBookmarkEdit;
}

QAction *CCompositWidget::actionBookmarkRemove() const
{
    return m_actionBookmarkRemove;
}


#endif // CCOMPOSITWIDGET_H
