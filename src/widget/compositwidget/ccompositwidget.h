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
#include <QModelIndex>
#include <QSplitter>
#include "inavigationactions.h"
class QSortFilterProxyModel;
class CNavigationItemModel;
class CNavigationView;
class CBookmarkView;
class CBookmarkItemModel;
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CPrj;
class QTreeView;
class CBookmarkItem;
class CNavAnchorItemModel;
class CNavTagItemModel;
class CTagSortFilterProxyModel;
class CNavTagView;
class QMenu;


class CCompositWidget : public QWidget, INavigationActions
{
    Q_OBJECT
public:
    explicit CCompositWidget(QWidget *parent = 0);
    virtual ~CCompositWidget();

    inline CBookmarkView *bookmarkView() const;
private slots:
    void navAnchorView_selectionModel_selectionChanged();
    void navTagView_selectionModel_selectionChanged();

    void bookmarkView_showContextMenu(const QPoint &pos);
    void navAnchorView_showContextMenu(const QPoint &pos);
    void navTagView_showContextMenu(const QPoint &pos);

    void bookmarkView_doubleClicked(const QModelIndex &index);

    void actionBookmarkOpenUrl_triggered();
    void actionBookmarkOpenUrlExt_triggered();
    void actionBookmarkSelectAll_triggered();
    void actionFavorite_triggered();
    void actionReadItLater_triggered();
    void actionRating_triggered();
    void actionBookmarkAdd_triggered();
    void actionBookmarkEdit_triggered();
    void actionBookmarkSendToTrash_triggered();
    void actionBookmarkRestore_triggered();
    void actionTagAdd_triggered();
    void actionTagEdit_triggered();
    void actionTagRemove_triggered();
    void actionEmptyTrash_triggered();

    void updateActionState();
    void updateOpenUrlActionState();
    void updateQuickEditActions();
protected:
    virtual void navActMoveTags(const QList<QStringList> &tags,
                                const QStringList &parentTag);
    virtual void navActSetTag(const QList<QUrl> &bookmarks,
                              const QStringList& tag);
    virtual void navActAddTag(const QList<QUrl> &bookmarks,
                              const QStringList &tag);
    virtual void navActClearTags(const QList<QUrl> &bookmarks);
    virtual void navActFavorite(const QList<QUrl> &bookmarks);
    virtual void navActReadItLater(const QList<QUrl> &bookmarks);
    virtual void navActTrash(const QList<QUrl> &bookmarks);
private:
    void updateBookmarkFilter();
    void updateBookmarkAnchorFilter();
    void updateBookmarkTagFilter();
    QMenu *createOpenUrlMenu();
    QMenu *createFavoriteMenu();
    QMenu *createReadItLaterMenu();
    QMenu *createRatingMenu();
private:
    CBookmarkView *m_bookmarkView;
    CBookmarkFilter *m_bookmarkFilter;
    CBookmarkFilterDataModel *m_bookmarkFilterDataModel;
    CBookmarkItemModel *m_bookmarkItemModel;

    QTreeView *m_navAnchorView;
    CNavAnchorItemModel *m_navAnchorItemModel;

    CNavTagView *m_navTagView;
    CNavTagItemModel *m_navTagItemModel;
    CTagSortFilterProxyModel *m_navTagSortFilterProxyModel;

    QSplitter *m_navSplitter;
    QSplitter *m_splitter;
    // we should sync the selection between anchorView and tagView
    // These flags are used to avoid the recursive call slots:
    // * navAnchorView_selectionModel_selectionChanged()
    // * navTagView_selectionModel_selectionChanged()
    bool m_isClearingNavAnchor;
    bool m_isClearingNavTag;
    // set of actions
    QAction *m_actionBookmarkOpenUrl;
    QMenu   *m_menuBookmarkOpenUrl;
    QAction *m_actionBookmarkSelectAll;
    QMenu   *m_menuFavorite;
    QMenu   *m_menuReadItLater;
    QMenu   *m_menuRating;
    QAction *m_actionBookmarkAdd;
    QAction *m_actionBookmarkEdit;
    QAction *m_actionBookmarkSendToTrash;
    QAction *m_actionBookmarkRestore;
    QAction *m_actionTagAdd;
    QAction *m_actionTagEdit;
    QAction *m_actionTagRemove;
    QAction *m_actionEmptyTrash;
};

CBookmarkView *CCompositWidget::bookmarkView() const
{
    return m_bookmarkView;
}

//class CCompositWidget : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit CCompositWidget(QWidget *parent = 0);
//    virtual ~CCompositWidget();

//    inline CBookmarkView *bookmarkView() const;

//    inline QAction *actionBookmarkAdd() const;
//    inline QAction *actionBookmarkEdit() const;
//    inline QAction *actionBookmarkRemove() const;
//private slots:
//    void navigation_selection_selectionChanged();

//    void actionTagAdd_triggered();
//    void actionTagEdit_triggered();
//    void actionTagRemove_triggered();

//    void actionBookmarkAdd_triggered();
//    void actionBookmarkEdit_triggered();
//    void actionBookmarkRemove_triggered();
//    void actionBookmarkScreenshot_triggered();
//    void actionBookmarkDownload_triggered();
//    void bookmarkView_showContextMenu(const QPoint &pos);
//    void bookmarkView_doubleClicked(const QModelIndex &index);
//    void navigationView_showContextMenu(const QPoint &pos);
//    void updateActions();

//    void download_next();
//    void download_finishied();

//    void screenshot_next();
//    void screenshot_finished();

//    void download_openUrl();
//private:
//    CNavigationView *m_navigationView;
//    CBookmarkView *m_bookmarkView;
//    CBookmarkFilter *m_filter;
//    CBookmarkFilterDataModel *m_dataModel;
//    CBookmarkItemModel *m_bookmarkItemModel;
//    CNavigationItemModel *m_navigationItemModel;

//    QNetworkAccessManager *m_network;
//    QAction *m_actionBookmarkAdd;
//    QAction *m_actionBookmarkEdit;
//    QAction *m_actionBookmarkRemove;
//    QAction *m_actionBookmarkScreenshot;
//    QAction *m_actionBookmarkDownload;

//    QAction *m_actionTagAdd;
//    QAction *m_actionTagEdit;
//    QAction *m_actionTagRemove;

//    QList<CBookmarkItem *> m_list;
//    QList<CBookmarkItem *> m_listDl;
//};

//CBookmarkView *CCompositWidget::bookmarkView() const
//{
//    return m_bookmarkView;
//}

//QAction *CCompositWidget::actionBookmarkAdd() const
//{
//    return m_actionBookmarkAdd;
//}

//QAction *CCompositWidget::actionBookmarkEdit() const
//{
//    return m_actionBookmarkEdit;
//}

//QAction *CCompositWidget::actionBookmarkRemove() const
//{
//    return m_actionBookmarkRemove;
//}


#endif // CCOMPOSITWIDGET_H
