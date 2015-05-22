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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "actinterface.h"
class QSortFilterProxyModel;
class QTreeView;
class BookmarkView;
class BookmarkItemModel;
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CBookmarkItem;
class NavAnchorItemModel;
class NavTagView;
class NavTagItemModel;
class CTagSortFilterProxyModel;
class QLineEdit;
class QSplitter;


class MainWindow : public QMainWindow, ActInterface
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
private slots:
    void updateAll();
    void updateWidgetState();
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
    void createAction_triggered();
    void openAction_triggered();
    void saveAction_triggered();
    void closeAction_triggered();
    void bookmarkCollectionImportAction_triggered();
    void bookmarkCollectionExportAction_triggered();
    void quitAction_triggered();

    // slots: bookmark menu action
    void bookmarkOpenUrlAction_triggered();
    void bookmarkOpenUrlExtAction_triggered();
    void bookmarkSelectAllAction_triggered();
    void favoriteAction_triggered();
    void readItLaterAction_triggered();
    void ratingAction_triggered();
    void bookmarkAddAction_triggered();
    void bookmarkEditAction_triggered();
    void bookmarkSendToTrashAction_triggered();
    void bookmarkRestoreAction_triggered();
    void bookmarkRemoveAction_triggered();

    void emptyTrashAction_triggered();

    // slots: tag menu action
    void tagAddAction_triggered();
    void tagEditAction_triggered();
    void tagRemoveAction_triggered();

    // slots: about menu action
    void aboutAction_triggered();
    void aboutQtAction_triggered();
protected:
    void closeEvent(QCloseEvent *event);
protected:
    virtual void actMoveTags(const QList<QStringList> &tags,
                                const QStringList &parentTag);
    virtual void actSetTag(const QList<QUrl> &bookmarks,
                              const QStringList& tag);
    virtual void actAddTag(const QList<QUrl> &bookmarks,
                              const QStringList &tag);
    virtual void actClearTags(const QList<QUrl> &bookmarks);
    virtual void actFavorite(const QList<QUrl> &bookmarks);
    virtual void actReadItLater(const QList<QUrl> &bookmarks);
    virtual void actTrash(const QList<QUrl> &bookmarks);
private:
    void createMenuBar();
    void createToolbars();
    void createFileActions();
    void createTagActions();
    void createBookmarkActions();
    void createAboutActions();

    void fillFileToolbar();
    void fillTagToolbar();
    void fillBookmarkToolbar();
    void fillAboutToolbar();

    void fillFileMenu();
    void fillTagMenu();
    void fillBookmarkMenu();
    void fillToolbarsMenu();
    void fillAboutMenu();
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
    QAction *m_bookmarkCollectionImportAction;
    QAction *m_bookmarkCollectionExportAction;
    QAction *m_quitAction;

    // menu: tag actions
    QAction *m_tagAddAction;
    QAction *m_tagEditAction;
    QAction *m_tagRemoveAction;

    // menu: bookmark actions
    QAction *m_bookmarkOpenUrlAction;
    QMenu   *m_bookmarkOpenUrlMenu;
    QAction *m_bookmarkSelectAllAction;
    QMenu   *m_favoriteMenu;
    QMenu   *m_readItLaterMenu;
    QMenu   *m_ratingMenu;
    QAction *m_bookmarkAddAction;
    QAction *m_bookmarkEditAction;
    QAction *m_bookmarkSendToTrashAction;
    QAction *m_bookmarkRestoreAction;
    QAction *m_bookmarkRemoveAction;

    QAction *m_emptyTrashAction;

    // menu: about actions
    QAction *m_aboutAction;
    QAction *m_aboutQtAction;

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
    BookmarkView *m_bookmarkView;
    CBookmarkFilter *m_bookmarkFilter;
    CBookmarkFilterDataModel *m_bookmarkFilterDataModel;
    BookmarkItemModel *m_bookmarkItemModel;

    // anchor view
    QTreeView *m_navAnchorView;
    NavAnchorItemModel *m_navAnchorItemModel;

    // tag view
    NavTagView *m_navTagView;
    NavTagItemModel *m_navTagItemModel;
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


#endif // MAINWINDOW_H
