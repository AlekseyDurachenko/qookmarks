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
#include "mainwindow.h"
#include "aboutdialog.h"
#include "bookmarkeditdialog.h"
#include "bookmarkfilterdatamodel.h"
#include "bookmarkfilter.h"
#include "bookmarkheaderview.h"
#include "bookmarkimportwizard.h"
#include "bookmarkitemmodel.h"
#include "bookmarkview.h"
#include "browser.h"
#include "iconmgr.h"
#include "icontheme.h"
#include "navanchoritemmodel.h"
#include "navtagitemmodel.h"
#include "navtagview.h"
#include "settings.h"
#include "tageditdialog.h"
#include "tagsortfilterproxymodel.h"
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QStack>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createFileActions();
    createTagActions();
    createBookmarkActions();
    createAboutActions();

    createToolbars();
    fillFileToolbar();
    fillTagToolbar();
    fillBookmarkToolbar();
    fillAboutToolbar();

    createMenuBar();
    fillFileMenu();
    fillTagMenu();
    fillBookmarkMenu();
    fillToolbarsMenu();
    fillAboutMenu();

    createStatusBar();

    createBookmarkView();
    createAnchorView();
    createTagView();
    createBookmarkSearch();
    createTagSearch();
    composeCentralWidget();

    configureNetwork();
    configureProject();
    configureActionUpdater();
    resetClearingFlags();

    readSettings();
    updateAll();

    openLastOpenedProject();
}

MainWindow::~MainWindow()
{
    writeSettings();
    writeSettings_lastOpenedBookmarks();
}

void MainWindow::updateAll()
{
    updateWindowTitle();
    updateWidgetState();
    updateActionState();
    updateOpenUrlActionState();
    updateQuickEditActions();
}

void MainWindow::updateWidgetState()
{
    m_bookmarkView->setEnabled(GPrj()->isOpen());
    m_navTagView->setEnabled(GPrj()->isOpen());
    m_navAnchorView->setEnabled(GPrj()->isOpen());
    m_bookmarkSearchLineEdit->setEnabled(GPrj()->isOpen());
    m_tagSearchLineEdit->setEnabled(GPrj()->isOpen());
}

void MainWindow::updateActionState()
{
    m_tagMenu->setEnabled(GPrj()->isOpen());
    m_bookmarkMenu->setEnabled(GPrj()->isOpen());

    m_bookmarkCollectionImportAction->setEnabled(GPrj()->isOpen());
    m_bookmarkCollectionExportAction->setEnabled(GPrj()->isOpen());
    m_bookmarkAddAction->setEnabled(GPrj()->isOpen());

    if (!GPrj()->isOpen())
    {
        m_bookmarkOpenUrlAction->setEnabled(false);
        m_bookmarkOpenUrlMenu->setEnabled(false);
        m_bookmarkSelectAllAction->setEnabled(false);
        m_bookmarkEditAction->setEnabled(false);
        m_bookmarkSendToTrashAction->setEnabled(false);
        m_bookmarkRestoreAction->setEnabled(false);
        m_bookmarkRemoveAction->setEnabled(false);

        m_emptyTrashAction->setEnabled(false);

        m_tagAddAction->setEnabled(false);
        m_tagEditAction->setEnabled(false);
        m_tagRemoveAction->setEnabled(false);

        return;
    }

    int bookmarkCount = m_bookmarkFilterDataModel->count();
    int selectedBookmarkCount = m_bookmarkView->selectionModel()->selectedRows().count();
    int selectedTagCount = m_navTagView->selectionModel()->selectedRows().count();

    bool hasTrashBookmarks = false;
    bool hasNotTrashBookmarks = false;
    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        if (bookmarkItem->data().isTrash())
            hasTrashBookmarks = true;
        else
            hasNotTrashBookmarks = true;

        if (hasTrashBookmarks && hasNotTrashBookmarks)
            break;
    }

    m_bookmarkOpenUrlAction->setEnabled(selectedBookmarkCount == 1);
    m_bookmarkOpenUrlMenu->setEnabled(selectedBookmarkCount);

    m_bookmarkSelectAllAction->setEnabled(bookmarkCount);
    m_bookmarkEditAction->setEnabled(selectedBookmarkCount == 1);
    m_bookmarkSendToTrashAction->setEnabled(hasNotTrashBookmarks);
    m_bookmarkRestoreAction->setEnabled(hasTrashBookmarks);
    m_bookmarkRemoveAction->setEnabled(selectedBookmarkCount);

    m_emptyTrashAction->setEnabled(GBookmarkMgr()->trashCount());

    m_tagAddAction->setEnabled(selectedTagCount <= 1);
    m_tagEditAction->setEnabled(selectedTagCount == 1);
    m_tagRemoveAction->setEnabled(selectedTagCount);
}

void MainWindow::updateOpenUrlActionState()
{
    int selectedBookmarkCount = m_bookmarkView->selectionModel()->selectedRows().count();

    QStack<QMenu *> stack;
    stack.push(m_bookmarkOpenUrlMenu);

    while (!stack.isEmpty())
    {
        QMenu *menu = stack.pop();
        foreach (QAction *action, menu->actions())
        {
            if (action->menu())
            {
               stack.push(action->menu());
            }
            else
            {
                action->setEnabled(Browser::canOpenUrl(
                        action->data().toMap().value("browser").toByteArray(),
                        static_cast<Browser::WindowType>(
                            action->data().toMap().value("windowType").toInt()),
                        selectedBookmarkCount));
            }
        }
    }
}

void MainWindow::updateQuickEditActions()
{
    int selectedCount = m_bookmarkView->selectionModel()->selectedRows().count();
    m_favoriteMenu->setEnabled(selectedCount);
    m_readItLaterMenu->setEnabled(selectedCount);
    m_ratingMenu->setEnabled(selectedCount);
    if (selectedCount == 0)
        return;

    bool hasFavorite = false;
    bool hasNotFavorite = false;
    bool hasReadItLater = false;
    bool hasNotReadItLater = false;
    QSet<int> rating;
    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        if (bookmarkItem->data().isFavorite())
            hasFavorite = true;
        else
            hasNotFavorite = true;

        if (bookmarkItem->data().isReadItLater())
            hasReadItLater = true;
        else
            hasNotReadItLater = true;

        rating.insert(bookmarkItem->data().rating());

        if (hasFavorite && hasNotFavorite
                && hasReadItLater && hasNotReadItLater
                && rating.count() > 1)
            break;
    }

    // update favorite yes/no actions
    if (hasFavorite && hasNotFavorite)
        foreach (QAction *action, m_favoriteMenu->actions())
            action->setChecked(false);
    else
        foreach (QAction *action, m_favoriteMenu->actions())
            if (action->data().toBool() == true && hasFavorite)
                action->setChecked(true);
            else if (action->data().toBool() == false && hasNotFavorite)
                action->setChecked(true);

    // update read it later yes/no actions
    if (hasReadItLater && hasNotReadItLater)
        foreach (QAction *action, m_readItLaterMenu->actions())
            action->setChecked(false);
    else
        foreach (QAction *action, m_readItLaterMenu->actions())
            if (action->data().toBool() == true && hasReadItLater)
                action->setChecked(true);
            else if (action->data().toBool() == false && hasNotReadItLater)
                action->setChecked(true);

    // update rating actions
    if (rating.count() != 1)
        foreach (QAction *action, m_ratingMenu->actions())
            action->setChecked(false);
    else
        foreach (QAction *action, m_ratingMenu->actions())
            if (action->data().toInt() == *rating.begin())
                action->setChecked(true);
}

void MainWindow::updateWindowTitle()
{
    if (GPrj()->isOpen())
        setWindowTitle(tr("%1 - %2").arg(appName(), GPrj()->path()));
    else
        setWindowTitle(tr("%1").arg(appName()));
}

void MainWindow::bookmarkView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_bookmarkOpenUrlAction);
    menu.addAction(m_bookmarkOpenUrlMenu->menuAction());
    menu.addSeparator();
    menu.addAction(m_bookmarkSelectAllAction);
    menu.addSeparator();
    menu.addAction(m_favoriteMenu->menuAction());
    menu.addAction(m_readItLaterMenu->menuAction());
    menu.addAction(m_ratingMenu->menuAction());
    menu.addSeparator();
    menu.addAction(m_bookmarkAddAction);
    menu.addAction(m_bookmarkEditAction);
    menu.addSeparator();
    menu.addAction(m_bookmarkSendToTrashAction);
    menu.addAction(m_bookmarkRestoreAction);
    menu.addSeparator();
    menu.addAction(m_bookmarkRemoveAction);
    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
}

void MainWindow::bookmarkView_header_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    for (int i = 0; i < m_bookmarkView->header()->count(); ++i)
    {
        QString label = m_bookmarkItemModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        // HACK: in the bookmark model this labels are empty
        if (i == 5)
            label = tr("Favorite");
        else if (i == 6)
            label = tr("Read it later");
        else if (i == 7)
            label = tr("Trash");

        QAction *action = new QAction(label, &menu);
        action->setData(i);
        action->setCheckable(true);
        action->setChecked(!m_bookmarkView->header()->isSectionHidden(i));
        menu.addAction(action);
    }

    QAction *action = menu.exec(m_bookmarkView->header()->viewport()->mapToGlobal(pos));
    if (action)
    {
        int index = action->data().toInt();
        m_bookmarkView->header()->setSectionHidden(index, !m_bookmarkView->header()->isSectionHidden(index));
    }
}

void MainWindow::bookmarkView_doubleClicked(const QModelIndex &index)
{
    Browser::openUrl(BookmarkItem::variantToPtr(index.data(Qt::UserRole))->data().url());
}

void MainWindow::navAnchorView_showContextMenu(const QPoint &pos)
{
    if (!m_navAnchorView->selectionModel()->selectedRows().isEmpty()
            && m_navAnchorView->currentIndex().data(Qt::UserRole).toInt()
                == NavAnchorItemModel::Trash)
    {
        QMenu menu(this);
        menu.addAction(m_emptyTrashAction);
        menu.exec(m_navAnchorView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::navAnchorView_selectionModel_selectionChanged()
{
    if (m_isClearingNavTag)
        return;

    m_isClearingNavAnchor = true;
    m_navTagView->clearSelection();
    m_isClearingNavAnchor = false;

    updateBookmarkFilter();
}

void MainWindow::navTagView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_tagAddAction);
    menu.addAction(m_tagEditAction);
    menu.addAction(m_tagRemoveAction);
    menu.addSeparator();
    menu.addAction(m_bookmarkAddAction);
    menu.exec(m_navTagView->viewport()->mapToGlobal(pos));
}

void MainWindow::navTagView_selectionModel_selectionChanged()
{
    if (m_isClearingNavAnchor)
        return;

    m_isClearingNavTag = true;
    m_navAnchorView->clearSelection();
    m_isClearingNavTag = false;

    updateBookmarkFilter();
}

void MainWindow::bookmarkSearchLineEdit_textChanged(const QString &text)
{
    m_bookmarkView->setFilterFixedString(text);
}

void MainWindow::tagSearchLineEdit_textChanged(const QString &text)
{
    m_navTagSortFilterProxyModel->setFilterFixedString(text);
    m_navTagView->expandAll();
}

void MainWindow::createAction_triggered()
{
    if (GPrj()->isOpen())
    {
        int ret = QMessageBox::Ignore;
        if (GPrj()->hasChanges())
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Opened bookmarks was changed. What do you want?"),
                    QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
        }
        else
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Bookmarks is open. Close current bookmarks?"),
                    QMessageBox::Yes|QMessageBox::Cancel);
        }

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
        else if (ret == QMessageBox::Save)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }

        GPrj()->close();
    }

    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Create bookmarks"), readSettings_lastBookmarkDirectory(),
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    if (QFile(Prj::xmlPath(dirName)).exists())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Project is already created. Replace the project?"),
                QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    QString reason;
    if (!GPrj()->create(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);
    else
        if (!GPrj()->open(dirName, &reason))
            QMessageBox::warning(this, tr("Warning"), reason);

    writeSettings_lastBookmarkDirectory(dirName);
}

void MainWindow::openAction_triggered()
{
    if (GPrj()->isOpen())
    {
        int ret = QMessageBox::Ignore;
        if (GPrj()->hasChanges())
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Opened bookmarks was changed. What do you want?"),
                    QMessageBox::Save|QMessageBox::Ignore|QMessageBox::Cancel);
        }
        else
        {
            ret = QMessageBox::question(this, tr("Question"),
                    tr("Bookmarks is open. Close current bookmarks?"),
                    QMessageBox::Yes|QMessageBox::Cancel);
        }

        if (ret == QMessageBox::Cancel)
        {
            return;
        }
        else if (ret == QMessageBox::Save)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }

        GPrj()->close();
    }

    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Open bookmarks"), readSettings_lastBookmarkDirectory(),
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    QString reason;
    if (!GPrj()->open(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);

    writeSettings_lastBookmarkDirectory(dirName);
}

void MainWindow::saveAction_triggered()
{
    QString reason;
    if (!GPrj()->save(&reason))
        QMessageBox::critical(this, tr("Critical"), reason);
}

void MainWindow::closeAction_triggered()
{
    if (GPrj()->hasChanges())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Opened bookmarks was changed. Save the changes?"),
                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                return;
            }
        }
    }

    // HACK: removing so slow if selection is exists
    m_bookmarkView->selectionModel()->clear();

    GPrj()->close();
}

void MainWindow::bookmarkCollectionImportAction_triggered()
{
    BookmarkImportWizard importWizard;
    importWizard.exec();
}

void MainWindow::bookmarkCollectionExportAction_triggered()
{
    ;
}

void MainWindow::quitAction_triggered()
{
    close();
}

void MainWindow::bookmarkOpenUrlAction_triggered()
{
    Browser::openUrl(m_bookmarkView->selectedBookmarks().first()->data().url());
}

void MainWindow::bookmarkOpenUrlExtAction_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (Browser::openUrl(action->data().toMap().value("browser").toByteArray(),
                         static_cast<Browser::WindowType>(action->data().toMap().value("windowType").toInt()),
                         m_bookmarkView->selectedUrls()) == false)
        QMessageBox::warning(this, tr("Warning"), tr("Can't open the url(s)"));
}

void MainWindow::bookmarkSelectAllAction_triggered()
{
    m_bookmarkView->selectAll();
}

void MainWindow::favoriteAction_triggered()
{
    bool favorite = qobject_cast<QAction *>(sender())->data().toBool();
    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        Bookmark data = bookmarkItem->data();
        data.setFavorite(favorite);
        bookmarkItem->setData(data);
    }
}

void MainWindow::readItLaterAction_triggered()
{
    bool readItLater = qobject_cast<QAction *>(sender())->data().toBool();
    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        Bookmark data = bookmarkItem->data();
        data.setReadItLater(readItLater);
        bookmarkItem->setData(data);
    }
}

void MainWindow::ratingAction_triggered()
{
    int rating = qobject_cast<QAction *>(sender())->data().toInt();
    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        Bookmark data = bookmarkItem->data();
        data.setRating(rating);
        bookmarkItem->setData(data);
    }
}

void MainWindow::bookmarkAddAction_triggered()
{
    BookmarkEditDialog newBookmarkDialog(this);
    newBookmarkDialog.setWindowTitle(tr("Create bookmark"));
    newBookmarkDialog.setCheckedTags(m_bookmarkFilter->tags());
    if (QUrl(QApplication::clipboard()->text()).isValid())
    {
        Bookmark data;
        data.setUrl(QUrl(QApplication::clipboard()->text()));
        newBookmarkDialog.setData(data);
    }

    if (newBookmarkDialog.exec() == QDialog::Accepted)
    {
        Bookmark data = newBookmarkDialog.toData();
        if (GBookmarkMgr()->find(data.url()))
        {
            QMessageBox::warning(this, tr("Warning"),
                    tr("The url \"%1\" is already exists")
                            .arg(data.url().toString()));
        }
        else
        {
            Bookmark bookmark = newBookmarkDialog.toData();
            QIcon favicon = newBookmarkDialog.toFavicon();
            QSet<TagItem *> checkedTags = newBookmarkDialog.toCheckedTags();

            BookmarkItem *bookmarkItem = GBookmarkMgr()->add(bookmark);
            GIconMgr()->saveIcon(bookmark.url(), favicon);

            foreach (TagItem *tag, checkedTags)
                tag->bookmarkAdd(bookmarkItem);
        }
    }
}

void MainWindow::bookmarkEditAction_triggered()
{
    BookmarkItem *bookmarkItem = m_bookmarkView->selectedBookmarks().first();

    BookmarkEditDialog editBookmarkDialog(this);
    editBookmarkDialog.setWindowTitle(tr("Edit bookmark"));
    editBookmarkDialog.setData(bookmarkItem->data());
    editBookmarkDialog.setFavicon(GIconMgr()->icon(bookmarkItem->data().url()));
    editBookmarkDialog.setCheckedTags(bookmarkItem->tags());
    if (editBookmarkDialog.exec() == QDialog::Accepted)
    {
        Bookmark bookmark = editBookmarkDialog.toData();
        QIcon favicon = editBookmarkDialog.toFavicon();
        QSet<TagItem *> checkedTags = editBookmarkDialog.toCheckedTags();
        QSet<TagItem *> prevTags = bookmarkItem->tags();

        bookmarkItem->setData(bookmark);
        if (!favicon.isNull())
            GIconMgr()->saveIcon(bookmark.url(), favicon);

        foreach (TagItem *tagItem, checkedTags)
        {
            tagItem->bookmarkAdd(bookmarkItem);
            prevTags.remove(tagItem);
        }

        foreach (TagItem *tagItem, prevTags)
            tagItem->bookmarkRemove(bookmarkItem);
    }
}

void MainWindow::bookmarkSendToTrashAction_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to send to trash the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        Bookmark data = bookmarkItem->data();
        data.setTrash(true);
        bookmarkItem->setData(data);
    }
}

void MainWindow::bookmarkRestoreAction_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to restore the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (BookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        Bookmark data = bookmarkItem->data();
        data.setTrash(false);
        bookmarkItem->setData(data);
    }
}

void MainWindow::bookmarkRemoveAction_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to remove the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    QList<QUrl> urls = m_bookmarkView->selectedUrls();

    // HACK: for prevent the freeze
    m_bookmarkView->selectionModel()->clear();

    foreach (const QUrl &url, urls)
        GBookmarkMgr()->removeAt(GBookmarkMgr()->find(url)->index());
}

void MainWindow::emptyTrashAction_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to empty the trash?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = GBookmarkMgr()->count()-1; i >= 0; --i)
        if (GBookmarkMgr()->at(i)->data().isTrash())
            GBookmarkMgr()->removeAt(i);
}

void MainWindow::tagAddAction_triggered()
{
    TagItem *item = GTagMgr()->rootItem();
    const QModelIndexList &selectedIndexes = m_navTagView->selectionModel()->selectedRows();
    if (!selectedIndexes.isEmpty())
        item = TagItem::variantToPtr(selectedIndexes.first().data(Qt::UserRole));

    TagEditDialog newTagDialog(TagEditDialog::New, item, this);
    if (newTagDialog.exec() == QDialog::Accepted)
        item->add(newTagDialog.toData());
}

void MainWindow::tagEditAction_triggered()
{
    TagItem *item = TagItem::variantToPtr(
                m_navTagView->selectionModel()->selectedRows().first()
                    .data(Qt::UserRole));

    TagEditDialog editTagDialog(TagEditDialog::Edit, item->parent(), this);
    editTagDialog.setData(item->data());
    if (editTagDialog.exec() == QDialog::Accepted)
        item->setData(editTagDialog.toData());
}

void MainWindow::tagRemoveAction_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to remove the selected tags?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    // we can't just remove the all selected tags, because it is
    // hierarchical structure, and item may be removed recursively
    // before remove method will be called
    QList<QStringList> tags;
    foreach (const QModelIndex &index,
             m_navTagView->selectionModel()->selectedRows())
        tags.append(TagItem::variantToPtr(index.data(Qt::UserRole))->path());

    foreach (const QStringList &tag, tags)
    {
        TagItem *tagItem = GTagMgr()->findByPath(tag);
        if (tagItem)
            tagItem->parent()->removeAt(tagItem->index());
    }
}

void MainWindow::aboutAction_triggered()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::aboutQtAction_triggered()
{
    qApp->aboutQt();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (GPrj()->hasChanges())
    {
        if (QMessageBox::question(this, tr("Question"),
                tr("Opened bookmarks was changed. Save the changes?"),
                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
            QString reason;
            if (!GPrj()->save(&reason))
            {
                QMessageBox::critical(this, tr("Critical"), reason);
                event->ignore();
            }
        }
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::actMoveTags(const QList<QStringList> &tags,
        const QStringList &parentTag)
{
    TagItem *parentItem = GTagMgr()->findByPath(parentTag);
    if (!parentItem)
        return;

    foreach (const QStringList &tag, tags)
    {
        TagItem *item = GTagMgr()->findByPath(tag);
        if (!item
                || item == parentItem
                || item->parent() == parentItem
                || item->aboveOf(parentItem))
            continue;

        item->moveTo(parentItem);
    }
}

void MainWindow::actSetTag(const QList<QUrl> &bookmarks,
        const QStringList &tag)
{
    TagItem *parentItem = GTagMgr()->findByPath(tag);
    if (!parentItem || parentItem == GTagMgr()->rootItem())
        return;

    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        foreach (TagItem *item, bookmarkItem->tags())
            if (parentItem != item)
                item->bookmarkRemove(bookmarkItem);

        parentItem->bookmarkAdd(bookmarkItem);
    }
}

void MainWindow::actAddTag(const QList<QUrl> &bookmarks,
        const QStringList &tag)
{
    TagItem *parentItem = GTagMgr()->findByPath(tag);
    if (!parentItem || parentItem == GTagMgr()->rootItem())
        return;

    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        parentItem->bookmarkAdd(bookmarkItem);
    }
}

void MainWindow::actClearTags(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to send the selected bookmarks into the trash?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
            if (!bookmarkItem)
                continue;

            foreach (TagItem *tagItem, bookmarkItem->tags())
                tagItem->bookmarkRemove(bookmarkItem);
        }
    }
}

void MainWindow::actFavorite(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        Bookmark data = bookmarkItem->data();
        data.setFavorite(true);
        bookmarkItem->setData(data);
    }
}

void MainWindow::actReadItLater(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        Bookmark data = bookmarkItem->data();
        data.setReadItLater(true);
        bookmarkItem->setData(data);
    }

}

void MainWindow::actTrash(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to move to trash the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            BookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
            if (!bookmarkItem)
                continue;

            Bookmark data = bookmarkItem->data();
            data.setTrash(true);
            bookmarkItem->setData(data);
        }
    }
}

void MainWindow::createMenuBar()
{
    m_fileMenu = new QMenu(tr("&File"), this);
    m_tagMenu = new QMenu(tr("&Tag"), this);
    m_bookmarkMenu = new QMenu(tr("&Bookmark"), this);
    m_viewMenu = new QMenu(tr("&View"), this);
    m_toolbarMenu = new QMenu(tr("&Toolbars"), this);
    m_aboutMenu = new QMenu(tr("&About"), this);

    m_windowMenuBar = new QMenuBar(this);
    m_windowMenuBar->addMenu(m_fileMenu);
    m_windowMenuBar->addMenu(m_tagMenu);
    m_windowMenuBar->addMenu(m_bookmarkMenu);
    m_windowMenuBar->addMenu(m_viewMenu);
    m_viewMenu->addMenu(m_toolbarMenu);
    m_windowMenuBar->addMenu(m_aboutMenu);
    setMenuBar(m_windowMenuBar);
}

void MainWindow::createToolbars()
{
    m_fileToolBar = new QToolBar(tr("File"), this);
    m_fileToolBar->setObjectName("m_fileToolBar");
    addToolBar(m_fileToolBar);

    m_tagToolBar = new QToolBar(tr("Tag"), this);
    m_tagToolBar->setObjectName("m_tagToolBar");
    addToolBar(m_tagToolBar);

    m_bookmarkToolBar = new QToolBar(tr("Bookmark"), this);
    m_bookmarkToolBar->setObjectName("m_bookmarkToolBar");
    addToolBar(m_bookmarkToolBar);

    m_aboutToolBar = new QToolBar(tr("About"), this);
    m_aboutToolBar->setObjectName("m_aboutToolBar");
    addToolBar(m_aboutToolBar);
}

void MainWindow::createFileActions()
{
    m_bookmarkCollectionImportAction = new QAction(this);
    m_bookmarkCollectionImportAction->setText("Bookmark collection import...");
    m_bookmarkCollectionImportAction->setIcon(IconTheme::icon("action-collection-import"));
    connect(m_bookmarkCollectionImportAction, SIGNAL(triggered()),
            this, SLOT(bookmarkCollectionImportAction_triggered()));

    m_bookmarkCollectionExportAction = new QAction(this);
    m_bookmarkCollectionExportAction->setText("Bookmark collection export...");
    m_bookmarkCollectionExportAction->setIcon(IconTheme::icon("action-collection-export"));
    connect(m_bookmarkCollectionExportAction, SIGNAL(triggered()),
            this, SLOT(bookmarkCollectionExportAction_triggered()));

    m_quitAction = new QAction(this);
    m_quitAction->setText("Quit...");
    m_quitAction->setIcon(IconTheme::icon("action-quit"));
    connect(m_quitAction, SIGNAL(triggered()),
            this, SLOT(quitAction_triggered()));
}

void MainWindow::createTagActions()
{
    m_tagAddAction = new QAction(tr("Add tag..."), this);
    m_tagAddAction->setIcon(IconTheme::icon("action-tag-add"));
    connect(m_tagAddAction, SIGNAL(triggered()),
            this, SLOT(tagAddAction_triggered()));

    m_tagEditAction = new QAction(tr("Edit tag..."), this);
    m_tagEditAction->setIcon(IconTheme::icon("action-tag-edit"));
    connect(m_tagEditAction, SIGNAL(triggered()),
            this, SLOT(tagEditAction_triggered()));

    m_tagRemoveAction = new QAction(tr("Remove tag(s)..."), this);
    m_tagRemoveAction->setIcon(IconTheme::icon("action-tag-remove"));
    connect(m_tagRemoveAction, SIGNAL(triggered()),
            this, SLOT(tagRemoveAction_triggered()));
}

void MainWindow::createBookmarkActions()
{
    m_bookmarkOpenUrlAction = new  QAction(tr("Open url"), this);
    m_bookmarkOpenUrlAction->setIcon(IconTheme::icon("browser-default"));
    connect(m_bookmarkOpenUrlAction, SIGNAL(triggered()),
            this, SLOT(bookmarkOpenUrlAction_triggered()));

    m_bookmarkOpenUrlMenu = createOpenUrlMenu();

    m_bookmarkSelectAllAction = new QAction(tr("Select all bookmarks"), this);
    m_bookmarkSelectAllAction->setIcon(IconTheme::icon("action-bookmark-select-all"));
    connect(m_bookmarkSelectAllAction, SIGNAL(triggered()),
            this, SLOT(bookmarkSelectAllAction_triggered()));

    m_favoriteMenu = createFavoriteMenu();
    m_readItLaterMenu = createReadItLaterMenu();
    m_ratingMenu = createRatingMenu();

    m_bookmarkAddAction = new QAction(tr("Add bookmark..."), this);
    m_bookmarkAddAction->setIcon(IconTheme::icon("action-bookmark-add"));
    connect(m_bookmarkAddAction, SIGNAL(triggered()),
            this, SLOT(bookmarkAddAction_triggered()));

    m_bookmarkEditAction = new QAction(tr("Edit bookmark..."), this);
    m_bookmarkEditAction->setIcon(IconTheme::icon("action-bookmark-edit"));
    connect(m_bookmarkEditAction, SIGNAL(triggered()),
            this, SLOT(bookmarkEditAction_triggered()));

    m_bookmarkSendToTrashAction = new QAction(tr("Send bookmark(s) to the trash..."), this);
    m_bookmarkSendToTrashAction->setIcon(IconTheme::icon("action-bookmark-send-to-trash"));
    connect(m_bookmarkSendToTrashAction, SIGNAL(triggered()),
            this, SLOT(bookmarkSendToTrashAction_triggered()));

    m_bookmarkRestoreAction = new QAction(tr("Restore bookmark(s)..."), this);
    m_bookmarkRestoreAction->setIcon(IconTheme::icon("action-bookmark-restore"));
    connect(m_bookmarkRestoreAction, SIGNAL(triggered()),
            this, SLOT(bookmarkRestoreAction_triggered()));

    m_bookmarkRemoveAction = new QAction(tr("Remove bookmark(s) permanently..."), this);
    m_bookmarkRemoveAction->setIcon(IconTheme::icon("action-bookmark-remove"));
    connect(m_bookmarkRemoveAction, SIGNAL(triggered()),
            this, SLOT(bookmarkRemoveAction_triggered()));


    m_emptyTrashAction = new QAction(tr("Empty trash..."), this);
    m_emptyTrashAction->setIcon(IconTheme::icon("action-empty-trash"));
    connect(m_emptyTrashAction, SIGNAL(triggered()),
            this, SLOT(emptyTrashAction_triggered()));
}

void MainWindow::createAboutActions()
{
    m_aboutAction = new QAction(this);
    m_aboutAction->setText("About...");
    m_aboutAction->setIcon(IconTheme::icon("action-about"));
    connect(m_aboutAction, SIGNAL(triggered()),
            this, SLOT(aboutAction_triggered()));

    m_aboutQtAction = new QAction(this);
    m_aboutQtAction->setText("About Qt...");
    m_aboutQtAction->setIcon(IconTheme::icon("action-about-qt"));
    connect(m_aboutQtAction, SIGNAL(triggered()),
            this, SLOT(aboutQtAction_triggered()));
}

void MainWindow::fillFileToolbar()
{
    m_fileToolBar->addAction(GPrj()->createAction());
    m_fileToolBar->addAction(GPrj()->openAction());
    m_fileToolBar->addAction(GPrj()->saveAction());
    m_fileToolBar->addAction(GPrj()->closeAction());
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_bookmarkCollectionImportAction);
    m_fileToolBar->addAction(m_bookmarkCollectionExportAction);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_quitAction);
}

void MainWindow::fillTagToolbar()
{
    m_tagToolBar->addAction(m_tagAddAction);
    m_tagToolBar->addAction(m_tagEditAction);
    m_tagToolBar->addAction(m_tagRemoveAction);
}

void MainWindow::fillBookmarkToolbar()
{
    QToolButton *openUrlButton = new QToolButton(this);
    openUrlButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    openUrlButton->setDefaultAction(m_bookmarkOpenUrlAction);
    m_bookmarkToolBar->addWidget(openUrlButton);
    QToolButton *openUrlExtButton = new QToolButton(this);
    openUrlExtButton->setPopupMode(QToolButton::InstantPopup);
    openUrlExtButton->setMenu(m_bookmarkOpenUrlMenu);
    openUrlExtButton->setArrowType(Qt::DownArrow);
    openUrlExtButton->setStyleSheet("QToolButton::menu-indicator { image: none; }");
    openUrlExtButton->setFocusPolicy(Qt::NoFocus);
    m_bookmarkToolBar->addWidget(openUrlExtButton);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_bookmarkSelectAllAction);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_favoriteMenu->menuAction());
    m_bookmarkToolBar->addAction(m_readItLaterMenu->menuAction());
    m_bookmarkToolBar->addAction(m_ratingMenu->menuAction());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_bookmarkAddAction);
    m_bookmarkToolBar->addAction(m_bookmarkEditAction);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_bookmarkSendToTrashAction);
    m_bookmarkToolBar->addAction(m_bookmarkRestoreAction);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_bookmarkRemoveAction);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_emptyTrashAction);
}

void MainWindow::fillAboutToolbar()
{
    m_aboutToolBar->addAction(m_aboutAction);
    m_aboutToolBar->addAction(m_aboutQtAction);
}

void MainWindow::fillFileMenu()
{
    m_fileMenu->addAction(GPrj()->createAction());
    m_fileMenu->addAction(GPrj()->openAction());
    m_fileMenu->addAction(GPrj()->saveAction());
    m_fileMenu->addAction(GPrj()->closeAction());
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_bookmarkCollectionImportAction);
    m_fileMenu->addAction(m_bookmarkCollectionExportAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);
}

void MainWindow::fillTagMenu()
{
    m_tagMenu->addAction(m_tagAddAction);
    m_tagMenu->addAction(m_tagEditAction);
    m_tagMenu->addAction(m_tagRemoveAction);
}

void MainWindow::fillBookmarkMenu()
{
    m_bookmarkMenu->addAction(m_bookmarkOpenUrlAction);
    m_bookmarkMenu->addAction(m_bookmarkOpenUrlMenu->menuAction());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_bookmarkSelectAllAction);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_favoriteMenu->menuAction());
    m_bookmarkMenu->addAction(m_readItLaterMenu->menuAction());
    m_bookmarkMenu->addAction(m_ratingMenu->menuAction());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_bookmarkAddAction);
    m_bookmarkMenu->addAction(m_bookmarkEditAction);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_bookmarkSendToTrashAction);
    m_bookmarkMenu->addAction(m_bookmarkRestoreAction);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_bookmarkRemoveAction);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_emptyTrashAction);
}

void MainWindow::fillToolbarsMenu()
{
    m_toolbarMenu->addAction(m_fileToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_tagToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_bookmarkToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_aboutToolBar->toggleViewAction());
}

void MainWindow::fillAboutMenu()
{
    m_aboutMenu->addAction(m_aboutAction);
    m_aboutMenu->addAction(m_aboutQtAction);
}

QMenu *MainWindow::createOpenUrlMenu()
{
    QMenu *openUrlMenu = new QMenu(tr("Open url(s) in"), this);

    // open url's in current window
    foreach (const QByteArray &browser, Browser::browsers())
    {
        if (!Browser::hasWindowType(browser, Browser::CurrentWindow))
            continue;

        QVariantMap map;
        map["browser"] = browser;
        map["windowType"] = Browser::CurrentWindow;

        QAction *action = new QAction(Browser::browserName(browser), this);
        action->setIcon(Browser::browserIcon(browser));
        action->setData(map);
        connect(action, SIGNAL(triggered()),
                this, SLOT(bookmarkOpenUrlExtAction_triggered()));

        openUrlMenu->addAction(action);
    }

    QMenu *openUrlNewWindowMenu = new QMenu(tr("Open url(s) in new window in"), this);
    openUrlMenu->addMenu(openUrlNewWindowMenu);
    QMenu *openUrlNewPrivateWindowMenu = new QMenu(tr("Open url(s) in new private window in"), this);
    openUrlMenu->addMenu(openUrlNewPrivateWindowMenu);

    // open url's in new window
    foreach (const QByteArray &browser, Browser::browsers())
    {
        if (!Browser::hasWindowType(browser, Browser::NewWindow))
            continue;

        QVariantMap map;
        map["browser"] = browser;
        map["windowType"] = Browser::NewWindow;

        QAction *action = new QAction(Browser::browserName(browser), this);
        action->setIcon(Browser::browserIcon(browser));
        action->setData(map);
        connect(action, SIGNAL(triggered()),
                this, SLOT(bookmarkOpenUrlExtAction_triggered()));

        openUrlNewWindowMenu->addAction(action);
    }

    // open url's in new private window
    foreach (const QByteArray &browser, Browser::browsers())
    {
        if (!Browser::hasWindowType(browser, Browser::NewPrivateWindow))
            continue;

        QVariantMap map;
        map["browser"] = browser;
        map["windowType"] = Browser::NewPrivateWindow;

        QAction *action = new QAction(Browser::browserName(browser), this);
        action->setIcon(Browser::browserIcon(browser));
        action->setData(map);
        connect(action, SIGNAL(triggered()),
                this, SLOT(bookmarkOpenUrlExtAction_triggered()));

        openUrlNewPrivateWindowMenu->addAction(action);
    }

    return openUrlMenu;
}

QMenu *MainWindow::createFavoriteMenu()
{
    QActionGroup *group = new QActionGroup(this);
    QMenu *menu = new QMenu(tr("Favorite"), this);
    menu->setIcon(IconTheme::icon("anchor-bookmark-favorite"));

    QAction *actionYes = new QAction(tr("Yes"), this);
    connect(actionYes, SIGNAL(triggered()),
            this, SLOT(favoriteAction_triggered()));
    actionYes->setCheckable(true);
    actionYes->setData(true);
    group->addAction(actionYes);
    menu->addAction(actionYes);

    QAction *actionNo = new QAction(tr("No"), this);
    connect(actionNo, SIGNAL(triggered()),
            this, SLOT(favoriteAction_triggered()));
    actionNo->setCheckable(true);
    actionNo->setData(false);
    group->addAction(actionNo);
    menu->addAction(actionNo);

    return menu;
}

QMenu *MainWindow::createReadItLaterMenu()
{
    QActionGroup *group = new QActionGroup(this);
    QMenu *menu = new QMenu(tr("Read it later"), this);
    menu->setIcon(IconTheme::icon("anchor-bookmark-readitlater"));

    QAction *actionYes = new QAction(tr("Yes"), this);
    connect(actionYes, SIGNAL(triggered()),
            this, SLOT(readItLaterAction_triggered()));
    actionYes->setCheckable(true);
    actionYes->setData(true);
    group->addAction(actionYes);
    menu->addAction(actionYes);

    QAction *actionNo = new QAction(tr("No"), this);
    connect(actionNo, SIGNAL(triggered()),
            this, SLOT(readItLaterAction_triggered()));
    actionNo->setCheckable(true);
    actionNo->setData(false);
    group->addAction(actionNo);
    menu->addAction(actionNo);

    return menu;
}

QMenu *MainWindow::createRatingMenu()
{
    QActionGroup *group = new QActionGroup(this);
    QMenu *menu = new QMenu(tr("Rating"), this);
    menu->setIcon(IconTheme::icon("anchor-bookmark-rated"));

    for (int i = BookmarkMinRating; i <= BookmarkMaxRating; ++i)
    {
        QAction *action = new QAction(QString::number(i), this);
        connect(action, SIGNAL(triggered()),
                this, SLOT(ratingAction_triggered()));
        action->setCheckable(true);
        action->setData(i);
        group->addAction(action);
        menu->addAction(action);
    }

    return menu;
}

void MainWindow::createStatusBar()
{
    m_windowStatusBar = new QStatusBar(this);
    setStatusBar(m_windowStatusBar);
}

void MainWindow::createBookmarkView()
{
    m_bookmarkFilter= new BookmarkFilter(this);
    m_bookmarkFilterDataModel = new BookmarkFilterDataModel(this);
    m_bookmarkFilterDataModel->setFilter(m_bookmarkFilter);
    m_bookmarkItemModel = new BookmarkItemModel(this);
    m_bookmarkItemModel->setDataModel(m_bookmarkFilterDataModel);
    m_bookmarkView = new BookmarkView(this);
    m_bookmarkView->setBookmarkModel(m_bookmarkItemModel);
    m_bookmarkView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_bookmarkView->setDragDropMode(QAbstractItemView::DragOnly);
    connect(m_bookmarkView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bookmarkView_showContextMenu(QPoint)));
    connect(m_bookmarkView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(bookmarkView_doubleClicked(QModelIndex)));

    m_bookmarkView->header()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_bookmarkView->header(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bookmarkView_header_showContextMenu(QPoint)));
}

void MainWindow::createAnchorView()
{
    m_navAnchorItemModel = new NavAnchorItemModel(this);
    m_navAnchorItemModel->setNavigationActions(this);
    m_navAnchorView = new QTreeView(this);
    m_navAnchorView->setModel(m_navAnchorItemModel);
    m_navAnchorView->setHeaderHidden(true);
    m_navAnchorView->setRootIsDecorated(true);
    m_navAnchorView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_navAnchorView->setDragDropMode(QAbstractItemView::DropOnly);
    m_navAnchorView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_navAnchorView->viewport()->setAcceptDrops(true);
    m_navAnchorView->setDropIndicatorShown(true);
    connect(m_navAnchorView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(navAnchorView_selectionModel_selectionChanged()));
    connect(m_navAnchorView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(navAnchorView_showContextMenu(QPoint)));
}

void MainWindow::createTagView()
{
    m_navTagItemModel = new NavTagItemModel(this);
    m_navTagItemModel->setNavigationActions(this);
    m_navTagSortFilterProxyModel = new TagSortFilterProxyModel(this);
    m_navTagSortFilterProxyModel->setSourceModel(m_navTagItemModel);
    m_navTagSortFilterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_navTagSortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_navTagSortFilterProxyModel->setDynamicSortFilter(true);
    m_navTagView = new NavTagView(this);
    m_navTagView->setSortingEnabled(true);
    m_navTagView->sortByColumn(0, Qt::AscendingOrder);
    m_navTagView->setModel(m_navTagSortFilterProxyModel);
    m_navTagView->setHeaderHidden(true);
    m_navTagView->setRootIsDecorated(true);
    m_navTagView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_navTagView->setDragDropMode(QAbstractItemView::DragDrop);
    m_navTagView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_navTagView->viewport()->setAcceptDrops(true);
    m_navTagView->setDropIndicatorShown(true);
    connect(m_navTagView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(navTagView_selectionModel_selectionChanged()));
    connect(m_navTagView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(navTagView_showContextMenu(QPoint)));
}

void MainWindow::createBookmarkSearch()
{
    m_bookmarkSearchLineEdit = new QLineEdit(this);
    m_bookmarkSearchLineEdit->setPlaceholderText(tr("type search text here"));
#if QT_VERSION >= 0x050200
    m_bookmarkSearchLineEdit->setClearButtonEnabled(true);
#endif
    connect(m_bookmarkSearchLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(bookmarkSearchLineEdit_textChanged(QString)));
}

void MainWindow::createTagSearch()
{
    m_tagSearchLineEdit = new QLineEdit(this);
    m_tagSearchLineEdit->setPlaceholderText(tr("type search text here"));
#if QT_VERSION >= 0x050200
    m_tagSearchLineEdit->setClearButtonEnabled(true);
#endif
    connect(m_tagSearchLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(tagSearchLineEdit_textChanged(QString)));
}

void MainWindow::composeCentralWidget()
{
    // + -- Anchor --- + ---------- Bookmarks --------------- +
    // |               | ___________search_line______________ |
    // | - All (N)     | Bookmark_1 filed_2 filed_3 filed_4   |
    // | - ...         | Bookmark_2 filed_2 filed_3 filed_4   |
    // | - Trash (N)   | Bookmark_3 filed_2 filed_3 filed_4   |
    // + --- Tag ----- +                                      |
    // | + Folder1 (N) |                                      |
    // |   + Sub (N)   |                                      |
    // | + FOlder2 (N) |                                      |
    // | __Search_line_|                                      |
    // + ------------- + ------------------------------------ +
    QWidget *bookmarkWidget = new QWidget(this);
    QVBoxLayout *bookmarkLayout = new QVBoxLayout;
    bookmarkWidget->setLayout(bookmarkLayout);
    bookmarkLayout->addWidget(m_bookmarkSearchLineEdit);
    bookmarkLayout->addWidget(m_bookmarkView);
    bookmarkLayout->setMargin(0);

    QWidget *tagWidget = new QWidget(this);
    QVBoxLayout *tagLayout = new QVBoxLayout;
    tagWidget->setLayout(tagLayout);
    tagLayout->addWidget(m_navTagView);
    tagLayout->addWidget(m_tagSearchLineEdit);
    tagLayout->setMargin(0);

    m_navSplitter = new QSplitter(Qt::Vertical, this);
    m_navSplitter->addWidget(m_navAnchorView);
    m_navSplitter->addWidget(tagWidget);
    m_navSplitter->setStretchFactor(0, 0);
    m_navSplitter->setStretchFactor(1, 1);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(m_navSplitter);
    m_splitter->addWidget(bookmarkWidget);
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    setCentralWidget(m_splitter);
}

void MainWindow::configureNetwork()
{
    GNetworkMgr()->setNetwork(new QNetworkAccessManager(this));
}

void MainWindow::configureProject()
{
    connect(GPrj()->createAction(), SIGNAL(triggered()),
            this, SLOT(createAction_triggered()));
    connect(GPrj()->openAction(), SIGNAL(triggered()),
            this, SLOT(openAction_triggered()));
    connect(GPrj()->saveAction(), SIGNAL(triggered()),
            this, SLOT(saveAction_triggered()));
    connect(GPrj()->closeAction(), SIGNAL(triggered()),
            this, SLOT(closeAction_triggered()));
    connect(GPrj(), SIGNAL(opened()), this, SLOT(updateAll()));
    connect(GPrj(), SIGNAL(closed()), this, SLOT(updateAll()));
}

void MainWindow::configureActionUpdater()
{
    // if someting of bookmark manager changed we should update the actions
    connect(GBookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(updateActionState()));
    connect(GBookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(updateActionState()));
    connect(GBookmarkMgr(), SIGNAL(tagsChanged(BookmarkItem*)),
            this, SLOT(updateActionState()));
    connect(GBookmarkMgr(), SIGNAL(dataChanged(BookmarkItem*)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(inserted(TagItem*,int,int)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(removed(TagItem*,int,int)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(moved(TagItem*,int,int,TagItem*,int)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(dataChanged(TagItem*)),
            this, SLOT(updateActionState()));

    // or if selection of bookmark view is changed
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateActionState()));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateOpenUrlActionState()));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateQuickEditActions()));
}

void MainWindow::resetClearingFlags()
{
    m_isClearingNavAnchor = false;
    m_isClearingNavTag = false;
}

void MainWindow::openLastOpenedProject()
{
    QString reason, path = readSettings_lastOpenedBookmarks();;
    if (!path.isEmpty())
        if (!GPrj()->open(path, &reason))
            QMessageBox::warning(this, tr("Warning"), reason);
}

void MainWindow::updateBookmarkFilter()
{
    if (!m_navAnchorView->selectionModel()->selectedRows().isEmpty())
        updateBookmarkAnchorFilter();
    else if (!m_navTagView->selectionModel()->selectedRows().isEmpty())
        updateBookmarkTagFilter();

    updateActionState();
    updateOpenUrlActionState();
    updateQuickEditActions();
}

void MainWindow::updateBookmarkAnchorFilter()
{
    // default filter options (equal to CNavAnchorItemModel::All)
    m_bookmarkFilter->setInclusiveOption(~BookmarkFilter::FilterOptions(BookmarkFilter::Trash));
    m_bookmarkFilter->setRatingRange(BookmarkMinRating, BookmarkMaxRating);
    m_bookmarkFilter->clearTags();

    switch (m_navAnchorView->currentIndex().data(Qt::UserRole).toInt())
    {
    case NavAnchorItemModel::All:
        break;
    case NavAnchorItemModel::Untagged:
        m_bookmarkFilter->setTag(GTagMgr()->rootItem());
        break;
    case NavAnchorItemModel::Favorites:
        m_bookmarkFilter->setInclusiveOption(
                    BookmarkFilter::FilterOptions(
                        BookmarkFilter::Favorite|BookmarkFilter::NotTrash));
        break;
    case NavAnchorItemModel::ReadItLater:
        m_bookmarkFilter->setInclusiveOption(
                    BookmarkFilter::FilterOptions(
                        BookmarkFilter::ReadItLater|BookmarkFilter::NotTrash));
        break;
    case NavAnchorItemModel::Rated:
        m_bookmarkFilter->setRatingRange(
                    BookmarkMinRating+1, BookmarkMaxRating);
        break;
    case NavAnchorItemModel::Trash:
        m_bookmarkFilter->setInclusiveOption(
                    BookmarkFilter::FilterOptions(BookmarkFilter::Trash));
        break;
    }

    m_bookmarkFilter->update();
}

void MainWindow::updateBookmarkTagFilter()
{
    QSet<TagItem *> selectedTags;
    foreach (const QModelIndex &index,
             m_navTagView->selectionModel()->selectedRows())
        selectedTags.insert(TagItem::variantToPtr(index.data(Qt::UserRole)));

    m_bookmarkFilter->setInclusiveOption(~BookmarkFilter::FilterOptions(BookmarkFilter::Trash));
    m_bookmarkFilter->setRatingRange(BookmarkMinRating, BookmarkMaxRating);
    m_bookmarkFilter->setTags(selectedTags);
    m_bookmarkFilter->update();
}

void MainWindow::readSettings()
{
    G_SETTINGS_INIT();
    restoreState(settings.value("MainWindow/state",
                                saveState()).toByteArray());
    restoreGeometry(settings.value("MainWindow/geometry",
                                   saveGeometry()).toByteArray());

    m_navSplitter->restoreState(
                settings.value("MainWindow/m_navSplitter/state",
                               m_navSplitter->saveState()).toByteArray());
    m_splitter->restoreState(
                settings.value("MainWindow/m_splitter/state",
                               m_splitter->saveState()).toByteArray());
    m_bookmarkView->header()->restoreState(
                settings.value("MainWindow/m_bookmarkView_header/state",
                               m_bookmarkView->header()->saveState()).toByteArray());
}

void MainWindow::writeSettings()
{
    G_SETTINGS_INIT();
    settings.setValue("MainWindow/state", saveState());
    settings.setValue("MainWindow/geometry", saveGeometry());

    settings.setValue("MainWindow/m_navSplitter/state",
                      m_navSplitter->saveState());
    settings.setValue("MainWindow/m_splitter/state",
                      m_splitter->saveState());
    settings.setValue("MainWindow/m_bookmarkView_header/state",
                      m_bookmarkView->header()->saveState());
}

QString MainWindow::readSettings_lastOpenedBookmarks() const
{
    G_SETTINGS_INIT();
    return settings.value("lastBookmarks", "").toString();
}

void MainWindow::writeSettings_lastOpenedBookmarks()
{
    G_SETTINGS_INIT();
    settings.setValue("lastBookmarks", GPrj()->path());
}

QString MainWindow::readSettings_lastBookmarkDirectory() const
{
    G_SETTINGS_INIT();
    return settings.value("lastBookmarksDirectory", "").toString();
}

void MainWindow::writeSettings_lastBookmarkDirectory(const QString &path)
{
    G_SETTINGS_INIT();
    settings.setValue("lastBookmarksDirectory", path);
}
