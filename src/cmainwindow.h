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
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "inavigationactions.h"
class QSortFilterProxyModel;
class QTreeView;
class CBookmarkView;
class CBookmarkItemModel;
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CBookmarkItem;
class CNavAnchorItemModel;
class CNavTagView;
class CNavTagItemModel;
class CTagSortFilterProxyModel;
class QLineEdit;
class QSplitter;


class CMainWindow : public QMainWindow, INavigationActions
{
    Q_OBJECT
public:
    explicit CMainWindow(QWidget *parent = 0);
    virtual ~CMainWindow();
private slots:
    void project_opened();
    void project_closed();

    void updateActionState();
    void updateOpenUrlActionState();
    void updateQuickEditActions();
    void updateWindowTitle();

    void bookmarkView_showContextMenu(const QPoint &pos);
    void bookmarkView_header_showContextMenu(const QPoint &pos);
    void bookmarkView_doubleClicked(const QModelIndex &index);

    void navAnchorView_selectionModel_selectionChanged();
    void navAnchorView_showContextMenu(const QPoint &pos);

    void navTagView_selectionModel_selectionChanged();
    void navTagView_showContextMenu(const QPoint &pos);

    void bookmarkSearchLineEdit_textChanged(const QString &text);
    void tagSearchLineEdit_textChanged(const QString &text);

    // slots: file menu action
    void actionCreate_triggered();
    void actionOpen_triggered();
    void actionSave_triggered();
    void actionClose_triggered();
    void actionBookmarkCollectionImport_triggered();
    void actionBookmarkCollectionExport_triggered();
    void actionQuit_triggered();

    // slots: bookmark menu action
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
    void actionBookmarkRemove_triggered();

    void actionEmptyTrash_triggered();

    // slots: tag menu action
    void actionTagAdd_triggered();
    void actionTagEdit_triggered();
    void actionTagRemove_triggered();

    // slots: about menu action
    void actionAbout_triggered();
    void actionAboutQt_triggered();
protected:
    void closeEvent(QCloseEvent *event);
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
    void createMenuBar();
    void createToolbars();
    void createFileActions();
    void createTagActions();
    void createBookmarkActions();
    void createAboutActions();

    void fillToolbarFile();
    void fillToolbarTag();
    void fillToolbarBookmark();
    void fillToolbarAbout();

    void fillMenuFile();
    void fillMenuTag();
    void fillMenuBookmark();
    void fillMenuToolbars();
    void fillMenuAbout();
    QMenu *createOpenUrlMenu();
    QMenu *createFavoriteMenu();
    QMenu *createReadItLaterMenu();
    QMenu *createRatingMenu();

    void createStatusBar();

    void createBookmarkView();
    void createAnchorView();
    void createTagView();
    void createBookmarkSearch();
    void createTagSearch();
    void composeCentralWidget();

    void configureNetwork();
    void configureProject();
    void configureActionUpdater();   
    void resetClearingFlags();

    void openLastOpenedProject();

    void updateBookmarkFilter();
    void updateBookmarkAnchorFilter();
    void updateBookmarkTagFilter();
private:
    void readSettings();
    void writeSettings();

    QString readSettings_lastOpenedBookmarks() const;
    void writeSettings_lastOpenedBookmarks();

    QString readSettings_lastBookmarkDirectory() const;
    void writeSettings_lastBookmarkDirectory(const QString &path);
private:
    // menu: file actions
    QAction *m_actionBookmarkCollectionImport;
    QAction *m_actionBookmarkCollectionExport;
    QAction *m_actionQuit;

    // menu: tag actions
    QAction *m_actionTagAdd;
    QAction *m_actionTagEdit;
    QAction *m_actionTagRemove;

    // menu: bookmark actions
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
    QAction *m_actionBookmarkRemove;

    QAction *m_actionEmptyTrash;

    // menu: about actions
    QAction *m_actionAbout;
    QAction *m_actionAboutQt;

    // window toolbars
    QToolBar *m_fileToolBar;
    QToolBar *m_tagToolBar;
    QToolBar *m_bookmarkToolBar;
    QToolBar *m_aboutToolBar;

    // window menubar
    QMenuBar *m_windowMenuBar;
    QMenu *m_fileMenu;
    QMenu *m_tagMenu;
    QMenu *m_bookmarkMenu;
    QMenu *m_viewMenu;
    QMenu *m_toolbarMenu;
    QMenu *m_aboutMenu;

    // window status bar
    QStatusBar *m_windowStatusBar;

    // bookmark view
    CBookmarkView *m_bookmarkView;
    CBookmarkFilter *m_bookmarkFilter;
    CBookmarkFilterDataModel *m_bookmarkFilterDataModel;
    CBookmarkItemModel *m_bookmarkItemModel;

    // anchor view
    QTreeView *m_navAnchorView;
    CNavAnchorItemModel *m_navAnchorItemModel;

    // tag view
    CNavTagView *m_navTagView;
    CNavTagItemModel *m_navTagItemModel;
    CTagSortFilterProxyModel *m_navTagSortFilterProxyModel;

    // search
    QLineEdit *m_tagSearchLineEdit;
    QLineEdit *m_bookmarkSearchLineEdit;

    // widget layouts
    QSplitter *m_navSplitter;
    QSplitter *m_splitter;

    // we should sync the selection between anchorView and tagView
    // These flags are used to avoid the recursive call slots:
    // * navAnchorView_selectionModel_selectionChanged()
    // * navTagView_selectionModel_selectionChanged()
    bool m_isClearingNavAnchor;
    bool m_isClearingNavTag;
};


#endif // CMAINWINDOW_H
