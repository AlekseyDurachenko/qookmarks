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
#include "cmainwindow.h"
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>
#include <QStack>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include "caboutdialog.h"
#include "cbookmarkeditdialog.h"
#include "cbookmarkfilterdatamodel.h"
#include "cbookmarkfilter.h"
#include "cbookmarkheaderview.h"
#include "cbookmarkimportwizard.h"
#include "cbookmarkitemmodel.h"
#include "cbookmarkview.h"
#include "ciconmgr.h"
#include "cnavanchoritemmodel.h"
#include "cnavtagitemmodel.h"
#include "cnavtagview.h"
#include "ctageditdialog.h"
#include "ctagsortfilterproxymodel.h"
#include "browser.h"
#include "icontheme.h"
#include "settings.h"


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createFileActions();
    createTagActions();
    createBookmarkActions();
    createAboutActions();

    createToolbars();
    fillToolbarFile();
    fillToolbarTag();
    fillToolbarBookmark();
    fillToolbarAbout();

    createMenuBar();
    fillMenuFile();
    fillMenuTag();
    fillMenuBookmark();
    fillMenuToolbars();
    fillMenuAbout();

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

CMainWindow::~CMainWindow()
{
    writeSettings();
    writeSettings_lastOpenedBookmarks();
}

void CMainWindow::updateAll()
{
    updateWindowTitle();
    updateWidgetState();
    updateActionState();
    updateOpenUrlActionState();
    updateQuickEditActions();
}

void CMainWindow::updateWidgetState()
{
    m_bookmarkView->setEnabled(GPrj()->isOpen());
    m_navTagView->setEnabled(GPrj()->isOpen());
    m_navAnchorView->setEnabled(GPrj()->isOpen());
    m_bookmarkSearchLineEdit->setEnabled(GPrj()->isOpen());
    m_tagSearchLineEdit->setEnabled(GPrj()->isOpen());
}

void CMainWindow::updateActionState()
{
    m_tagMenu->setEnabled(GPrj()->isOpen());
    m_bookmarkMenu->setEnabled(GPrj()->isOpen());

    m_actionBookmarkCollectionImport->setEnabled(GPrj()->isOpen());
    m_actionBookmarkCollectionExport->setEnabled(GPrj()->isOpen());
    m_actionBookmarkAdd->setEnabled(GPrj()->isOpen());

    if (!GPrj()->isOpen())
    {
        m_actionBookmarkOpenUrl->setEnabled(false);
        m_menuBookmarkOpenUrl->setEnabled(false);
        m_actionBookmarkSelectAll->setEnabled(false);
        m_actionBookmarkEdit->setEnabled(false);
        m_actionBookmarkSendToTrash->setEnabled(false);
        m_actionBookmarkRestore->setEnabled(false);
        m_actionBookmarkRemove->setEnabled(false);

        m_actionEmptyTrash->setEnabled(false);

        m_actionTagAdd->setEnabled(false);
        m_actionTagEdit->setEnabled(false);
        m_actionTagRemove->setEnabled(false);

        return;
    }

    int bookmarkCount = m_bookmarkFilterDataModel->count();
    int selectedBookmarkCount = m_bookmarkView->selectionModel()->selectedRows().count();
    int selectedTagCount = m_navTagView->selectionModel()->selectedRows().count();

    bool hasTrashBookmarks = false;
    bool hasNotTrashBookmarks = false;
    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        if (bookmarkItem->data().isTrash())
            hasTrashBookmarks = true;
        else
            hasNotTrashBookmarks = true;

        if (hasTrashBookmarks && hasNotTrashBookmarks)
            break;
    }

    m_actionBookmarkOpenUrl->setEnabled(selectedBookmarkCount == 1);
    m_menuBookmarkOpenUrl->setEnabled(selectedBookmarkCount);

    m_actionBookmarkSelectAll->setEnabled(bookmarkCount);
    m_actionBookmarkEdit->setEnabled(selectedBookmarkCount == 1);
    m_actionBookmarkSendToTrash->setEnabled(hasNotTrashBookmarks);
    m_actionBookmarkRestore->setEnabled(hasTrashBookmarks);
    m_actionBookmarkRemove->setEnabled(selectedBookmarkCount);

    m_actionEmptyTrash->setEnabled(GBookmarkMgr()->trashCount());

    m_actionTagAdd->setEnabled(selectedTagCount <= 1);
    m_actionTagEdit->setEnabled(selectedTagCount == 1);
    m_actionTagRemove->setEnabled(selectedTagCount);
}

void CMainWindow::updateOpenUrlActionState()
{
    int selectedBookmarkCount = m_bookmarkView->selectionModel()->selectedRows().count();

    QStack<QMenu *> stack;
    stack.push(m_menuBookmarkOpenUrl);

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

void CMainWindow::updateQuickEditActions()
{
    int selectedCount = m_bookmarkView->selectionModel()->selectedRows().count();
    m_menuFavorite->setEnabled(selectedCount);
    m_menuReadItLater->setEnabled(selectedCount);
    m_menuRating->setEnabled(selectedCount);
    if (selectedCount == 0)
        return;

    bool hasFavorite = false;
    bool hasNotFavorite = false;
    bool hasReadItLater = false;
    bool hasNotReadItLater = false;
    QSet<int> rating;
    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
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
        foreach (QAction *action, m_menuFavorite->actions())
            action->setChecked(false);
    else
        foreach (QAction *action, m_menuFavorite->actions())
            if (action->data().toBool() == true && hasFavorite)
                action->setChecked(true);
            else if (action->data().toBool() == false && hasNotFavorite)
                action->setChecked(true);

    // update read it later yes/no actions
    if (hasReadItLater && hasNotReadItLater)
        foreach (QAction *action, m_menuReadItLater->actions())
            action->setChecked(false);
    else
        foreach (QAction *action, m_menuReadItLater->actions())
            if (action->data().toBool() == true && hasReadItLater)
                action->setChecked(true);
            else if (action->data().toBool() == false && hasNotReadItLater)
                action->setChecked(true);

    // update rating actions
    if (rating.count() != 1)
        foreach (QAction *action, m_menuRating->actions())
            action->setChecked(false);
    else
        foreach (QAction *action, m_menuRating->actions())
            if (action->data().toInt() == *rating.begin())
                action->setChecked(true);
}

void CMainWindow::updateWindowTitle()
{
    if (GPrj()->isOpen())
        setWindowTitle(tr("%1 - %2").arg(appName(), GPrj()->path()));
    else
        setWindowTitle(tr("%1").arg(appName()));
}

void CMainWindow::bookmarkView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionBookmarkOpenUrl);
    menu.addAction(m_menuBookmarkOpenUrl->menuAction());
    menu.addSeparator();
    menu.addAction(m_actionBookmarkSelectAll);
    menu.addSeparator();
    menu.addAction(m_menuFavorite->menuAction());
    menu.addAction(m_menuReadItLater->menuAction());
    menu.addAction(m_menuRating->menuAction());
    menu.addSeparator();
    menu.addAction(m_actionBookmarkAdd);
    menu.addAction(m_actionBookmarkEdit);
    menu.addSeparator();
    menu.addAction(m_actionBookmarkSendToTrash);
    menu.addAction(m_actionBookmarkRestore);
    menu.addSeparator();
    menu.addAction(m_actionBookmarkRemove);
    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
}

void CMainWindow::bookmarkView_header_showContextMenu(const QPoint &pos)
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

void CMainWindow::bookmarkView_doubleClicked(const QModelIndex &index)
{
    Browser::openUrl(CBookmarkItem::variantToPtr(index.data(Qt::UserRole))->data().url());
}

void CMainWindow::navAnchorView_showContextMenu(const QPoint &pos)
{
    if (!m_navAnchorView->selectionModel()->selectedRows().isEmpty()
            && m_navAnchorView->currentIndex().data(Qt::UserRole).toInt()
                == CNavAnchorItemModel::Trash)
    {
        QMenu menu(this);
        menu.addAction(m_actionEmptyTrash);
        menu.exec(m_navAnchorView->viewport()->mapToGlobal(pos));
    }
}

void CMainWindow::navAnchorView_selectionModel_selectionChanged()
{
    if (m_isClearingNavTag)
        return;

    m_isClearingNavAnchor = true;
    m_navTagView->clearSelection();
    m_isClearingNavAnchor = false;

    updateBookmarkFilter();
}

void CMainWindow::navTagView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionTagAdd);
    menu.addAction(m_actionTagEdit);
    menu.addAction(m_actionTagRemove);
    menu.addSeparator();
    menu.addAction(m_actionBookmarkAdd);
    menu.exec(m_navTagView->viewport()->mapToGlobal(pos));
}

void CMainWindow::navTagView_selectionModel_selectionChanged()
{
    if (m_isClearingNavAnchor)
        return;

    m_isClearingNavTag = true;
    m_navAnchorView->clearSelection();
    m_isClearingNavTag = false;

    updateBookmarkFilter();
}

void CMainWindow::bookmarkSearchLineEdit_textChanged(const QString &text)
{
    m_bookmarkView->setFilterFixedString(text);
}

void CMainWindow::tagSearchLineEdit_textChanged(const QString &text)
{
    m_navTagSortFilterProxyModel->setFilterFixedString(text);
    m_navTagView->expandAll();
}

void CMainWindow::actionCreate_triggered()
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

    if (QFile(CPrj::xmlPath(dirName)).exists())
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

void CMainWindow::actionOpen_triggered()
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

void CMainWindow::actionSave_triggered()
{
    QString reason;
    if (!GPrj()->save(&reason))
        QMessageBox::critical(this, tr("Critical"), reason);
}

void CMainWindow::actionClose_triggered()
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

void CMainWindow::actionBookmarkCollectionImport_triggered()
{
    CBookmarkImportWizard importWizard;
    importWizard.exec();
}

void CMainWindow::actionBookmarkCollectionExport_triggered()
{
    ;
}

void CMainWindow::actionQuit_triggered()
{
    close();
}

void CMainWindow::actionBookmarkOpenUrl_triggered()
{
    Browser::openUrl(m_bookmarkView->selectedBookmarks().first()->data().url());
}

void CMainWindow::actionBookmarkOpenUrlExt_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (Browser::openUrl(action->data().toMap().value("browser").toByteArray(),
                         static_cast<Browser::WindowType>(action->data().toMap().value("windowType").toInt()),
                         m_bookmarkView->selectedUrls()) == false)
        QMessageBox::warning(this, tr("Warning"), tr("Can't open the url(s)"));
}

void CMainWindow::actionBookmarkSelectAll_triggered()
{
    m_bookmarkView->selectAll();
}

void CMainWindow::actionFavorite_triggered()
{
    bool favorite = qobject_cast<QAction *>(sender())->data().toBool();
    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        CBookmark data = bookmarkItem->data();
        data.setFavorite(favorite);
        bookmarkItem->setData(data);
    }
}

void CMainWindow::actionReadItLater_triggered()
{
    bool readItLater = qobject_cast<QAction *>(sender())->data().toBool();
    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        CBookmark data = bookmarkItem->data();
        data.setReadItLater(readItLater);
        bookmarkItem->setData(data);
    }
}

void CMainWindow::actionRating_triggered()
{
    int rating = qobject_cast<QAction *>(sender())->data().toInt();
    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        CBookmark data = bookmarkItem->data();
        data.setRating(rating);
        bookmarkItem->setData(data);
    }
}

void CMainWindow::actionBookmarkAdd_triggered()
{
    CBookmarkEditDialog newBookmarkDialog(this);
    newBookmarkDialog.setWindowTitle(tr("Create bookmark"));
    newBookmarkDialog.setCheckedTags(m_bookmarkFilter->tags());
    if (QUrl(QApplication::clipboard()->text()).isValid())
    {
        CBookmark data;
        data.setUrl(QUrl(QApplication::clipboard()->text()));
        newBookmarkDialog.setData(data);
    }

    if (newBookmarkDialog.exec() == QDialog::Accepted)
    {
        CBookmark data = newBookmarkDialog.toData();
        if (GBookmarkMgr()->find(data.url()))
        {
            QMessageBox::warning(this, tr("Warning"),
                    tr("The url \"%1\" is already exists")
                            .arg(data.url().toString()));
        }
        else
        {
            CBookmark bookmark = newBookmarkDialog.toData();
            QIcon favicon = newBookmarkDialog.toFavicon();
            QSet<CTagItem *> checkedTags = newBookmarkDialog.toCheckedTags();

            CBookmarkItem *bookmarkItem = GBookmarkMgr()->add(bookmark);
            GIconMgr()->saveIcon(bookmark.url(), favicon);

            foreach (CTagItem *tag, checkedTags)
                tag->bookmarkAdd(bookmarkItem);
        }
    }
}

void CMainWindow::actionBookmarkEdit_triggered()
{
    CBookmarkItem *bookmarkItem = m_bookmarkView->selectedBookmarks().first();

    CBookmarkEditDialog editBookmarkDialog(this);
    editBookmarkDialog.setWindowTitle(tr("Edit bookmark"));
    editBookmarkDialog.setData(bookmarkItem->data());
    editBookmarkDialog.setFavicon(GIconMgr()->icon(bookmarkItem->data().url()));
    editBookmarkDialog.setCheckedTags(bookmarkItem->tags());
    if (editBookmarkDialog.exec() == QDialog::Accepted)
    {
        CBookmark bookmark = editBookmarkDialog.toData();
        QIcon favicon = editBookmarkDialog.toFavicon();
        QSet<CTagItem *> checkedTags = editBookmarkDialog.toCheckedTags();
        QSet<CTagItem *> prevTags = bookmarkItem->tags();

        bookmarkItem->setData(bookmark);
        if (!favicon.isNull())
            GIconMgr()->saveIcon(bookmark.url(), favicon);

        foreach (CTagItem *tagItem, checkedTags)
        {
            tagItem->bookmarkAdd(bookmarkItem);
            prevTags.remove(tagItem);
        }

        foreach (CTagItem *tagItem, prevTags)
            tagItem->bookmarkRemove(bookmarkItem);
    }
}

void CMainWindow::actionBookmarkSendToTrash_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to send to trash the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        CBookmark data = bookmarkItem->data();
        data.setTrash(true);
        bookmarkItem->setData(data);
    }
}

void CMainWindow::actionBookmarkRestore_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to restore the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (CBookmarkItem *bookmarkItem, m_bookmarkView->selectedBookmarks())
    {
        CBookmark data = bookmarkItem->data();
        data.setTrash(false);
        bookmarkItem->setData(data);
    }
}

void CMainWindow::actionBookmarkRemove_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to remove the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (const QUrl &url, m_bookmarkView->selectedUrls())
        GBookmarkMgr()->removeAt(GBookmarkMgr()->find(url)->index());
}

void CMainWindow::actionEmptyTrash_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to empty the trash?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = GBookmarkMgr()->count()-1; i >= 0; --i)
        if (GBookmarkMgr()->at(i)->data().isTrash())
            GBookmarkMgr()->removeAt(i);
}

void CMainWindow::actionTagAdd_triggered()
{
    CTagItem *item = GTagMgr()->rootItem();
    const QModelIndexList &selectedIndexes = m_navTagView->selectionModel()->selectedRows();
    if (!selectedIndexes.isEmpty())
        item = CTagItem::variantToPtr(selectedIndexes.first().data(Qt::UserRole));

    CTagEditDialog newTagDialog(CTagEditDialog::New, item, this);
    if (newTagDialog.exec() == QDialog::Accepted)
        item->add(newTagDialog.toData());
}

void CMainWindow::actionTagEdit_triggered()
{
    CTagItem *item = CTagItem::variantToPtr(
                m_navTagView->selectionModel()->selectedRows().first()
                    .data(Qt::UserRole));

    CTagEditDialog editTagDialog(CTagEditDialog::Edit, item->parent(), this);
    editTagDialog.setData(item->data());
    if (editTagDialog.exec() == QDialog::Accepted)
        item->setData(editTagDialog.toData());
}

void CMainWindow::actionTagRemove_triggered()
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
        tags.append(CTagItem::variantToPtr(index.data(Qt::UserRole))->path());

    foreach (const QStringList &tag, tags)
    {
        CTagItem *tagItem = GTagMgr()->findByPath(tag);
        if (tagItem)
            tagItem->parent()->removeAt(tagItem->index());
    }
}

void CMainWindow::actionAbout_triggered()
{
    CAboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void CMainWindow::actionAboutQt_triggered()
{
    qApp->aboutQt();
}

void CMainWindow::closeEvent(QCloseEvent *event)
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

void CMainWindow::navActMoveTags(const QList<QStringList> &tags,
        const QStringList &parentTag)
{
    CTagItem *parentItem = GTagMgr()->findByPath(parentTag);
    if (!parentItem)
        return;

    foreach (const QStringList &tag, tags)
    {
        CTagItem *item = GTagMgr()->findByPath(tag);
        if (!item
                || item == parentItem
                || item->parent() == parentItem
                || item->aboveOf(parentItem))
            continue;

        item->moveTo(parentItem);
    }
}

void CMainWindow::navActSetTag(const QList<QUrl> &bookmarks,
        const QStringList &tag)
{
    CTagItem *parentItem = GTagMgr()->findByPath(tag);
    if (!parentItem || parentItem == GTagMgr()->rootItem())
        return;

    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        foreach (CTagItem *item, bookmarkItem->tags())
            if (parentItem != item)
                item->bookmarkRemove(bookmarkItem);

        parentItem->bookmarkAdd(bookmarkItem);
    }
}

void CMainWindow::navActAddTag(const QList<QUrl> &bookmarks,
        const QStringList &tag)
{
    CTagItem *parentItem = GTagMgr()->findByPath(tag);
    if (!parentItem || parentItem == GTagMgr()->rootItem())
        return;

    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        parentItem->bookmarkAdd(bookmarkItem);
    }
}

void CMainWindow::navActClearTags(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to send the selected bookmarks into the trash?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            CBookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
            if (!bookmarkItem)
                continue;

            foreach (CTagItem *tagItem, bookmarkItem->tags())
                tagItem->bookmarkRemove(bookmarkItem);
        }
    }
}

void CMainWindow::navActFavorite(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        CBookmark data = bookmarkItem->data();
        data.setFavorite(true);
        bookmarkItem->setData(data);
    }
}

void CMainWindow::navActReadItLater(const QList<QUrl> &bookmarks)
{
    foreach (const QUrl &url, bookmarks)
    {
        CBookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
        if (!bookmarkItem)
            continue;

        CBookmark data = bookmarkItem->data();
        data.setReadItLater(true);
        bookmarkItem->setData(data);
    }

}

void CMainWindow::navActTrash(const QList<QUrl> &bookmarks)
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to move to trash the selected bookmarks?"),
            QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Yes)
    {
        foreach (const QUrl &url, bookmarks)
        {
            CBookmarkItem *bookmarkItem = GBookmarkMgr()->find(url);
            if (!bookmarkItem)
                continue;

            CBookmark data = bookmarkItem->data();
            data.setTrash(true);
            bookmarkItem->setData(data);
        }
    }
}

void CMainWindow::createMenuBar()
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

void CMainWindow::createToolbars()
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

void CMainWindow::createFileActions()
{
    m_actionBookmarkCollectionImport = new QAction(this);
    m_actionBookmarkCollectionImport->setText("Bookmark collection import...");
    m_actionBookmarkCollectionImport->setIcon(IconTheme::icon("action-collection-import"));
    connect(m_actionBookmarkCollectionImport, SIGNAL(triggered()),
            this, SLOT(actionBookmarkCollectionImport_triggered()));

    m_actionBookmarkCollectionExport = new QAction(this);
    m_actionBookmarkCollectionExport->setText("Bookmark collection export...");
    m_actionBookmarkCollectionExport->setIcon(IconTheme::icon("action-collection-export"));
    connect(m_actionBookmarkCollectionExport, SIGNAL(triggered()),
            this, SLOT(actionBookmarkCollectionExport_triggered()));

    m_actionQuit = new QAction(this);
    m_actionQuit->setText("Quit...");
    m_actionQuit->setIcon(IconTheme::icon("action-quit"));
    connect(m_actionQuit, SIGNAL(triggered()),
            this, SLOT(actionQuit_triggered()));
}

void CMainWindow::createTagActions()
{
    m_actionTagAdd = new QAction(tr("Add tag..."), this);
    m_actionTagAdd->setIcon(IconTheme::icon("action-tag-add"));
    connect(m_actionTagAdd, SIGNAL(triggered()),
            this, SLOT(actionTagAdd_triggered()));

    m_actionTagEdit = new QAction(tr("Edit tag..."), this);
    m_actionTagEdit->setIcon(IconTheme::icon("action-tag-edit"));
    connect(m_actionTagEdit, SIGNAL(triggered()),
            this, SLOT(actionTagEdit_triggered()));

    m_actionTagRemove = new QAction(tr("Remove tag(s)..."), this);
    m_actionTagRemove->setIcon(IconTheme::icon("action-tag-remove"));
    connect(m_actionTagRemove, SIGNAL(triggered()),
            this, SLOT(actionTagRemove_triggered()));
}

void CMainWindow::createBookmarkActions()
{
    m_actionBookmarkOpenUrl = new  QAction(tr("Open url"), this);
    m_actionBookmarkOpenUrl->setIcon(IconTheme::icon("browser-default"));
    connect(m_actionBookmarkOpenUrl, SIGNAL(triggered()),
            this, SLOT(actionBookmarkOpenUrl_triggered()));

    m_menuBookmarkOpenUrl = createOpenUrlMenu();

    m_actionBookmarkSelectAll = new QAction(tr("Select all bookmarks"), this);
    m_actionBookmarkSelectAll->setIcon(IconTheme::icon("action-bookmark-select-all"));
    connect(m_actionBookmarkSelectAll, SIGNAL(triggered()),
            this, SLOT(actionBookmarkSelectAll_triggered()));

    m_menuFavorite = createFavoriteMenu();
    m_menuReadItLater = createReadItLaterMenu();
    m_menuRating = createRatingMenu();

    m_actionBookmarkAdd = new QAction(tr("Add bookmark..."), this);
    m_actionBookmarkAdd->setIcon(IconTheme::icon("action-bookmark-add"));
    connect(m_actionBookmarkAdd, SIGNAL(triggered()),
            this, SLOT(actionBookmarkAdd_triggered()));

    m_actionBookmarkEdit = new QAction(tr("Edit bookmark..."), this);
    m_actionBookmarkEdit->setIcon(IconTheme::icon("action-bookmark-edit"));
    connect(m_actionBookmarkEdit, SIGNAL(triggered()),
            this, SLOT(actionBookmarkEdit_triggered()));

    m_actionBookmarkSendToTrash = new QAction(tr("Send bookmark(s) to the trash..."), this);
    m_actionBookmarkSendToTrash->setIcon(IconTheme::icon("action-bookmark-send-to-trash"));
    connect(m_actionBookmarkSendToTrash, SIGNAL(triggered()),
            this, SLOT(actionBookmarkSendToTrash_triggered()));

    m_actionBookmarkRestore = new QAction(tr("Restore bookmark(s)..."), this);
    m_actionBookmarkRestore->setIcon(IconTheme::icon("action-bookmark-restore"));
    connect(m_actionBookmarkRestore, SIGNAL(triggered()),
            this, SLOT(actionBookmarkRestore_triggered()));

    m_actionBookmarkRemove = new QAction(tr("Remove bookmark(s) permanently..."), this);
    m_actionBookmarkRemove->setIcon(IconTheme::icon("action-bookmark-remove"));
    connect(m_actionBookmarkRemove, SIGNAL(triggered()),
            this, SLOT(actionBookmarkRemove_triggered()));


    m_actionEmptyTrash = new QAction(tr("Empty trash..."), this);
    m_actionEmptyTrash->setIcon(IconTheme::icon("action-empty-trash"));
    connect(m_actionEmptyTrash, SIGNAL(triggered()),
            this, SLOT(actionEmptyTrash_triggered()));
}

void CMainWindow::createAboutActions()
{
    m_actionAbout = new QAction(this);
    m_actionAbout->setText("About...");
    m_actionAbout->setIcon(IconTheme::icon("action-about"));
    connect(m_actionAbout, SIGNAL(triggered()),
            this, SLOT(actionAbout_triggered()));

    m_actionAboutQt = new QAction(this);
    m_actionAboutQt->setText("About Qt...");
    m_actionAboutQt->setIcon(IconTheme::icon("action-about-qt"));
    connect(m_actionAboutQt, SIGNAL(triggered()),
            this, SLOT(actionAboutQt_triggered()));
}

void CMainWindow::fillToolbarFile()
{
    m_fileToolBar->addAction(GPrj()->actionCreate());
    m_fileToolBar->addAction(GPrj()->actionOpen());
    m_fileToolBar->addAction(GPrj()->actionSave());
    m_fileToolBar->addAction(GPrj()->actionClose());
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_actionBookmarkCollectionImport);
    m_fileToolBar->addAction(m_actionBookmarkCollectionExport);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_actionQuit);
}

void CMainWindow::fillToolbarTag()
{
    m_tagToolBar->addAction(m_actionTagAdd);
    m_tagToolBar->addAction(m_actionTagEdit);
    m_tagToolBar->addAction(m_actionTagRemove);
}

void CMainWindow::fillToolbarBookmark()
{
    QToolButton *openUrlButton = new QToolButton(this);
    openUrlButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    openUrlButton->setDefaultAction(m_actionBookmarkOpenUrl);
    m_bookmarkToolBar->addWidget(openUrlButton);
    QToolButton *openUrlExtButton = new QToolButton(this);
    openUrlExtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    openUrlExtButton->setPopupMode(QToolButton::MenuButtonPopup);
    openUrlExtButton->setMenu(m_menuBookmarkOpenUrl);
    openUrlExtButton->setText(m_menuBookmarkOpenUrl->menuAction()->text());
    openUrlExtButton->setIcon(m_actionBookmarkOpenUrl->icon());
    m_bookmarkToolBar->addWidget(openUrlExtButton);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_actionBookmarkSelectAll);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_menuFavorite->menuAction());
    m_bookmarkToolBar->addAction(m_menuReadItLater->menuAction());
    m_bookmarkToolBar->addAction(m_menuRating->menuAction());
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_actionBookmarkAdd);
    m_bookmarkToolBar->addAction(m_actionBookmarkEdit);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_actionBookmarkSendToTrash);
    m_bookmarkToolBar->addAction(m_actionBookmarkRestore);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_actionBookmarkRemove);
    m_bookmarkToolBar->addSeparator();
    m_bookmarkToolBar->addAction(m_actionEmptyTrash);
}

void CMainWindow::fillToolbarAbout()
{
    m_aboutToolBar->addAction(m_actionAbout);
    m_aboutToolBar->addAction(m_actionAboutQt);
}

void CMainWindow::fillMenuFile()
{
    m_fileMenu->addAction(GPrj()->actionCreate());
    m_fileMenu->addAction(GPrj()->actionOpen());
    m_fileMenu->addAction(GPrj()->actionSave());
    m_fileMenu->addAction(GPrj()->actionClose());
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionBookmarkCollectionImport);
    m_fileMenu->addAction(m_actionBookmarkCollectionExport);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionQuit);
}

void CMainWindow::fillMenuTag()
{
    m_tagMenu->addAction(m_actionTagAdd);
    m_tagMenu->addAction(m_actionTagEdit);
    m_tagMenu->addAction(m_actionTagRemove);
}

void CMainWindow::fillMenuBookmark()
{
    m_bookmarkMenu->addAction(m_actionBookmarkOpenUrl);
    m_bookmarkMenu->addAction(m_menuBookmarkOpenUrl->menuAction());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_actionBookmarkSelectAll);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_menuFavorite->menuAction());
    m_bookmarkMenu->addAction(m_menuReadItLater->menuAction());
    m_bookmarkMenu->addAction(m_menuRating->menuAction());
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_actionBookmarkAdd);
    m_bookmarkMenu->addAction(m_actionBookmarkEdit);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_actionBookmarkSendToTrash);
    m_bookmarkMenu->addAction(m_actionBookmarkRestore);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_actionBookmarkRemove);
    m_bookmarkMenu->addSeparator();
    m_bookmarkMenu->addAction(m_actionEmptyTrash);
}

void CMainWindow::fillMenuToolbars()
{
    m_toolbarMenu->addAction(m_fileToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_tagToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_bookmarkToolBar->toggleViewAction());
    m_toolbarMenu->addAction(m_aboutToolBar->toggleViewAction());
}

void CMainWindow::fillMenuAbout()
{
    m_aboutMenu->addAction(m_actionAbout);
    m_aboutMenu->addAction(m_actionAboutQt);
}

QMenu *CMainWindow::createOpenUrlMenu()
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
                this, SLOT(actionBookmarkOpenUrlExt_triggered()));

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
                this, SLOT(actionBookmarkOpenUrlExt_triggered()));

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
                this, SLOT(actionBookmarkOpenUrlExt_triggered()));

        openUrlNewPrivateWindowMenu->addAction(action);
    }

    return openUrlMenu;
}

QMenu *CMainWindow::createFavoriteMenu()
{
    QActionGroup *group = new QActionGroup(this);
    QMenu *menu = new QMenu(tr("Favorite"), this);
    menu->setIcon(IconTheme::icon("anchor-bookmark-favorite"));

    QAction *actionYes = new QAction(tr("Yes"), this);
    connect(actionYes, SIGNAL(triggered()),
            this, SLOT(actionFavorite_triggered()));
    actionYes->setCheckable(true);
    actionYes->setData(true);
    group->addAction(actionYes);
    menu->addAction(actionYes);

    QAction *actionNo = new QAction(tr("No"), this);
    connect(actionNo, SIGNAL(triggered()),
            this, SLOT(actionFavorite_triggered()));
    actionNo->setCheckable(true);
    actionNo->setData(false);
    group->addAction(actionNo);
    menu->addAction(actionNo);

    return menu;
}

QMenu *CMainWindow::createReadItLaterMenu()
{
    QActionGroup *group = new QActionGroup(this);
    QMenu *menu = new QMenu(tr("Read it later"), this);
    menu->setIcon(IconTheme::icon("anchor-bookmark-readitlater"));

    QAction *actionYes = new QAction(tr("Yes"), this);
    connect(actionYes, SIGNAL(triggered()),
            this, SLOT(actionReadItLater_triggered()));
    actionYes->setCheckable(true);
    actionYes->setData(true);
    group->addAction(actionYes);
    menu->addAction(actionYes);

    QAction *actionNo = new QAction(tr("No"), this);
    connect(actionNo, SIGNAL(triggered()),
            this, SLOT(actionReadItLater_triggered()));
    actionNo->setCheckable(true);
    actionNo->setData(false);
    group->addAction(actionNo);
    menu->addAction(actionNo);

    return menu;
}

QMenu *CMainWindow::createRatingMenu()
{
    QActionGroup *group = new QActionGroup(this);
    QMenu *menu = new QMenu(tr("Rating"), this);
    menu->setIcon(IconTheme::icon("anchor-bookmark-rated"));

    for (int i = Bookmark::MinRating; i <= Bookmark::MaxRating; ++i)
    {
        QAction *action = new QAction(QString::number(i), this);
        connect(action, SIGNAL(triggered()),
                this, SLOT(actionRating_triggered()));
        action->setCheckable(true);
        action->setData(i);
        group->addAction(action);
        menu->addAction(action);
    }

    return menu;
}

void CMainWindow::createStatusBar()
{
    m_windowStatusBar = new QStatusBar(this);
    setStatusBar(m_windowStatusBar);
}

void CMainWindow::createBookmarkView()
{
    m_bookmarkFilter= new CBookmarkFilter(this);
    m_bookmarkFilterDataModel = new CBookmarkFilterDataModel(this);
    m_bookmarkFilterDataModel->setFilter(m_bookmarkFilter);
    m_bookmarkItemModel = new CBookmarkItemModel(this);
    m_bookmarkItemModel->setDataModel(m_bookmarkFilterDataModel);
    m_bookmarkView = new CBookmarkView(this);
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

void CMainWindow::createAnchorView()
{
    m_navAnchorItemModel = new CNavAnchorItemModel(this);
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

void CMainWindow::createTagView()
{
    m_navTagItemModel = new CNavTagItemModel(this);
    m_navTagItemModel->setNavigationActions(this);
    m_navTagSortFilterProxyModel = new CTagSortFilterProxyModel(this);
    m_navTagSortFilterProxyModel->setSourceModel(m_navTagItemModel);
    m_navTagSortFilterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_navTagSortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_navTagSortFilterProxyModel->setDynamicSortFilter(true);
    m_navTagView = new CNavTagView(this);
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

void CMainWindow::createBookmarkSearch()
{
    m_bookmarkSearchLineEdit = new QLineEdit(this);
    m_bookmarkSearchLineEdit->setPlaceholderText(tr("type search text here"));
#if QT_VERSION >= 0x050200
    m_bookmarkSearchLineEdit->setClearButtonEnabled(true);
#endif
    connect(m_bookmarkSearchLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(bookmarkSearchLineEdit_textChanged(QString)));
}

void CMainWindow::createTagSearch()
{
    m_tagSearchLineEdit = new QLineEdit(this);
    m_tagSearchLineEdit->setPlaceholderText(tr("type search text here"));
#if QT_VERSION >= 0x050200
    m_tagSearchLineEdit->setClearButtonEnabled(true);
#endif
    connect(m_tagSearchLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(tagSearchLineEdit_textChanged(QString)));
}

void CMainWindow::composeCentralWidget()
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

void CMainWindow::configureNetwork()
{
    GNetworkMgr()->setNetwork(new QNetworkAccessManager(this));
}

void CMainWindow::configureProject()
{
    connect(GPrj()->actionCreate(), SIGNAL(triggered()),
            this, SLOT(actionCreate_triggered()));
    connect(GPrj()->actionOpen(), SIGNAL(triggered()),
            this, SLOT(actionOpen_triggered()));
    connect(GPrj()->actionSave(), SIGNAL(triggered()),
            this, SLOT(actionSave_triggered()));
    connect(GPrj()->actionClose(), SIGNAL(triggered()),
            this, SLOT(actionClose_triggered()));
    connect(GPrj(), SIGNAL(opened()), this, SLOT(updateAll()));
    connect(GPrj(), SIGNAL(closed()), this, SLOT(updateAll()));
}

void CMainWindow::configureActionUpdater()
{
    // if someting of bookmark manager changed we should update the actions
    connect(GBookmarkMgr(), SIGNAL(inserted(int,int)),
            this, SLOT(updateActionState()));
    connect(GBookmarkMgr(), SIGNAL(removed(int,int)),
            this, SLOT(updateActionState()));
    connect(GBookmarkMgr(), SIGNAL(tagsChanged(CBookmarkItem*)),
            this, SLOT(updateActionState()));
    connect(GBookmarkMgr(), SIGNAL(dataChanged(CBookmarkItem*)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(inserted(CTagItem*,int,int)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(removed(CTagItem*,int,int)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(moved(CTagItem*,int,int,CTagItem*,int)),
            this, SLOT(updateActionState()));
    connect(GTagMgr(), SIGNAL(dataChanged(CTagItem*)),
            this, SLOT(updateActionState()));

    // or if selection of bookmark view is changed
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateActionState()));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateOpenUrlActionState()));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateQuickEditActions()));
}

void CMainWindow::resetClearingFlags()
{
    m_isClearingNavAnchor = false;
    m_isClearingNavTag = false;
}

void CMainWindow::openLastOpenedProject()
{
    QString reason, path = readSettings_lastOpenedBookmarks();;
    if (!path.isEmpty())
        if (!GPrj()->open(path, &reason))
            QMessageBox::warning(this, tr("Warning"), reason);
}

void CMainWindow::updateBookmarkFilter()
{
    if (!m_navAnchorView->selectionModel()->selectedRows().isEmpty())
        updateBookmarkAnchorFilter();
    else if (!m_navTagView->selectionModel()->selectedRows().isEmpty())
        updateBookmarkTagFilter();

    updateActionState();
    updateOpenUrlActionState();
    updateQuickEditActions();
}

void CMainWindow::updateBookmarkAnchorFilter()
{
    // default filter options (equal to CNavAnchorItemModel::All)
    m_bookmarkFilter->setInclusiveOption(~Bookmark::FilterOptions(Bookmark::Trash));
    m_bookmarkFilter->setRatingRange(Bookmark::MinRating, Bookmark::MaxRating);
    m_bookmarkFilter->clearTags();

    switch (m_navAnchorView->currentIndex().data(Qt::UserRole).toInt())
    {
    case CNavAnchorItemModel::All:
        break;
    case CNavAnchorItemModel::Untagged:
        m_bookmarkFilter->setTag(GTagMgr()->rootItem());
        break;
    case CNavAnchorItemModel::Favorites:
        m_bookmarkFilter->setInclusiveOption(
                    Bookmark::FilterOptions(
                        Bookmark::Favorite|Bookmark::NotTrash));
        break;
    case CNavAnchorItemModel::ReadItLater:
        m_bookmarkFilter->setInclusiveOption(
                    Bookmark::FilterOptions(
                        Bookmark::ReadItLater|Bookmark::NotTrash));
        break;
    case CNavAnchorItemModel::Rated:
        m_bookmarkFilter->setRatingRange(
                    Bookmark::MinRating+1, Bookmark::MaxRating);
        break;
    case CNavAnchorItemModel::Trash:
        m_bookmarkFilter->setInclusiveOption(
                    Bookmark::FilterOptions(Bookmark::Trash));
        break;
    }

    m_bookmarkFilter->update();
}

void CMainWindow::updateBookmarkTagFilter()
{
    QSet<CTagItem *> selectedTags;
    foreach (const QModelIndex &index,
             m_navTagView->selectionModel()->selectedRows())
        selectedTags.insert(CTagItem::variantToPtr(index.data(Qt::UserRole)));

    m_bookmarkFilter->setInclusiveOption(~Bookmark::FilterOptions(Bookmark::Trash));
    m_bookmarkFilter->setRatingRange(Bookmark::MinRating, Bookmark::MaxRating);
    m_bookmarkFilter->setTags(selectedTags);
    m_bookmarkFilter->update();
}

void CMainWindow::readSettings()
{
    G_SETTINGS_INIT();
    restoreState(settings.value("CMainWindow/state",
                                saveState()).toByteArray());
    restoreGeometry(settings.value("CMainWindow/geometry",
                                   saveGeometry()).toByteArray());

    m_navSplitter->restoreState(
                settings.value("CMainWindow/m_navSplitter/state",
                               m_navSplitter->saveState()).toByteArray());
    m_splitter->restoreState(
                settings.value("CMainWindow/m_splitter/state",
                               m_splitter->saveState()).toByteArray());
    m_bookmarkView->header()->restoreState(
                settings.value("CMainWindow/m_bookmarkView_header/state",
                               m_bookmarkView->header()->saveState()).toByteArray());
}

void CMainWindow::writeSettings()
{
    G_SETTINGS_INIT();
    settings.setValue("CMainWindow/state", saveState());
    settings.setValue("CMainWindow/geometry", saveGeometry());

    settings.setValue("CMainWindow/m_navSplitter/state",
                      m_navSplitter->saveState());
    settings.setValue("CMainWindow/m_splitter/state",
                      m_splitter->saveState());
    settings.setValue("CMainWindow/m_bookmarkView_header/state",
                      m_bookmarkView->header()->saveState());
}

QString CMainWindow::readSettings_lastOpenedBookmarks() const
{
    G_SETTINGS_INIT();
    return settings.value("lastBookmarks", "").toString();
}

void CMainWindow::writeSettings_lastOpenedBookmarks()
{
    G_SETTINGS_INIT();
    settings.setValue("lastBookmarks", GPrj()->path());
}

QString CMainWindow::readSettings_lastBookmarkDirectory() const
{
    G_SETTINGS_INIT();
    return settings.value("lastBookmarksDirectory", "").toString();
}

void CMainWindow::writeSettings_lastBookmarkDirectory(const QString &path)
{
    G_SETTINGS_INIT();
    settings.setValue("lastBookmarksDirectory", path);
}
