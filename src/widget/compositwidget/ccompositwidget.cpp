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
#include "ccompositwidget.h"
#include <QHBoxLayout>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <QDesktopServices>
#include "cnavigationview.h"
#include "cbookmarkview.h"
#include "cnavigationitemmodel.h"
#include "cnavtagitemmodel.h"
#include "cbookmarkfilter.h"
#include "cbookmarkitemmodel.h"
#include "cbookmarkfilterdatamodel.h"
#include "cmanager.h"
#include "ctagmgr.h"
#include "cbookmarkitem.h"
#include "cbookmarkmgr.h"
#include "cwebpagescreenshot.h"
#include "cwebpagedownloader.h"
#include "cbookmarkheaderview.h"
#include "cprj.h"
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include "ctagitem.h"
#include "ctagmgr.h"
#include "ctageditdialog.h"
#include "cbookmarkeditdialog.h"
#include "singleton.h"
#include "cprj.h"
#include "ciconmgr.h"
#include "settings.h"
#include "cnavtagview.h"
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QActionGroup>
#include <QProcess>
#include <QStack>
#include "cnavanchoritemmodel.h"
#include "ctagitemmodel.h"
#include "ctagsortfilterproxymodel.h"
#include <QPushButton>
#include "browser.h"
#include "browser.h"


CCompositWidget::CCompositWidget(QWidget *parent) :
    QWidget(parent)
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
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateOpenUrlActionState()));

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

    m_navTagItemModel = new CNavTagItemModel(this);
    m_navTagItemModel->setNavigationActions(this);
    m_navTagSortFilterProxyModel = new CTagSortFilterProxyModel(this);
    m_navTagSortFilterProxyModel->setSourceModel(m_navTagItemModel);
    m_navTagSortFilterProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
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

    // + ------------- + ------------------------------------ +
    // | - All (N)     | Bookmark_1 filed_2 filed_3 filed_4   |
    // | - ...         | Bookmark_2 filed_2 filed_3 filed_4   |
    // | - Trash (N)   | Bookmark_3 filed_2 filed_3 filed_4   |
    // + ------------- +                                      |
    // | + Folder1 (N) |                                      |
    // |   + Sub (N)   |                                      |
    // | + FOlder2 (N) |                                      |
    // + ------------- + ------------------------------------ +
    m_navSplitter = new QSplitter(Qt::Vertical, this);
    m_navSplitter->addWidget(m_navAnchorView);
    m_navSplitter->addWidget(m_navTagView);
    m_navSplitter->setStretchFactor(0, 0);
    m_navSplitter->setStretchFactor(1, 1);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(m_navSplitter);
    m_splitter->addWidget(m_bookmarkView);
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_splitter);
    setLayout(hbox);

    // default values
    m_isClearingNavAnchor = false;
    m_isClearingNavTag = false;


    // create actions
    m_actionBookmarkOpenUrl = new  QAction(tr("Open url"), this);
    connect(m_actionBookmarkOpenUrl, SIGNAL(triggered()),
            this, SLOT(actionBookmarkOpenUrl_triggered()));

    m_menuBookmarkOpenUrl = createOpenUrlMenu();

    m_actionBookmarkSelectAll = new QAction(tr("Select all bookmarks"), this);
    connect(m_actionBookmarkSelectAll, SIGNAL(triggered()),
            this, SLOT(actionBookmarkSelectAll_triggered()));

    m_actionBookmarkAdd = new QAction(tr("Add bookmark..."), this);
    connect(m_actionBookmarkAdd, SIGNAL(triggered()),
            this, SLOT(actionBookmarkAdd_triggered()));

    m_actionBookmarkEdit = new QAction(tr("Edit bookmark..."), this);
    connect(m_actionBookmarkEdit, SIGNAL(triggered()),
            this, SLOT(actionBookmarkEdit_triggered()));

    m_actionBookmarkSendToTrash = new QAction(tr("Send bookmark(s) to the trash..."), this);
    connect(m_actionBookmarkSendToTrash, SIGNAL(triggered()),
            this, SLOT(actionBookmarkSendToTrash_triggered()));

    m_actionBookmarkRestore = new QAction(tr("Restore bookmark(s)..."), this);
    connect(m_actionBookmarkRestore, SIGNAL(triggered()),
            this, SLOT(actionBookmarkRestore_triggered()));

    m_actionTagAdd = new QAction(tr("Add tag..."), this);
    connect(m_actionTagAdd, SIGNAL(triggered()),
            this, SLOT(actionTagAdd_triggered()));

    m_actionTagEdit = new QAction(tr("Edit tag..."), this);
    connect(m_actionTagEdit, SIGNAL(triggered()),
            this, SLOT(actionTagEdit_triggered()));

    m_actionTagRemove = new QAction(tr("Remove tag(s)..."), this);
    connect(m_actionTagRemove, SIGNAL(triggered()),
            this, SLOT(actionTagRemove_triggered()));

    m_actionEmptyTrash = new QAction(tr("Empty trash..."), this);
    connect(m_actionEmptyTrash, SIGNAL(triggered()),
            this, SLOT(actionEmptyTrash_triggered()));

    // update actions on project changing
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
    // update actions on bookmark selection changed
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateActionState()));
}

CCompositWidget::~CCompositWidget()
{
}

void CCompositWidget::navAnchorView_selectionModel_selectionChanged()
{
    if (m_isClearingNavTag)
        return;

    m_isClearingNavAnchor = true;
    m_navTagView->clearSelection();
    m_isClearingNavAnchor = false;

    updateBookmarkFilter();
}

void CCompositWidget::navTagView_selectionModel_selectionChanged()
{
    if (m_isClearingNavAnchor)
        return;

    m_isClearingNavTag = true;
    m_navAnchorView->clearSelection();
    m_isClearingNavTag = false;

    updateBookmarkFilter();
}

void CCompositWidget::bookmarkView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionBookmarkOpenUrl);
    menu.addAction(m_menuBookmarkOpenUrl->menuAction());
    menu.addSeparator();
    menu.addAction(m_actionBookmarkSelectAll);
    menu.addSeparator();
    menu.addAction(m_actionBookmarkAdd);
    menu.addAction(m_actionBookmarkEdit);
    menu.addAction(m_actionBookmarkSendToTrash);
    menu.addAction(m_actionBookmarkRestore);
    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
}

void CCompositWidget::navAnchorView_showContextMenu(const QPoint &pos)
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

void CCompositWidget::navTagView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionTagAdd);
    menu.addAction(m_actionTagEdit);
    menu.addAction(m_actionTagRemove);
    menu.addSeparator();
    menu.addAction(m_actionBookmarkAdd);
    menu.exec(m_navTagView->viewport()->mapToGlobal(pos));
}

void CCompositWidget::bookmarkView_doubleClicked(const QModelIndex &index)
{
    Browser::openUrl(CBookmarkItem::variantToPtr(index.data(Qt::UserRole))->data().url());
}

void CCompositWidget::actionBookmarkOpenUrl_triggered()
{
    Browser::openUrl(m_bookmarkView->selectedBookmarks().first()->data().url());
}

void CCompositWidget::actionBookmarkOpenUrlExt_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (Browser::openUrl(action->data().toMap().value("browser").toByteArray(),
                         static_cast<Browser::WindowType>(action->data().toMap().value("windowType").toInt()),
                         m_bookmarkView->selectedUrls()) == false)
        QMessageBox::warning(this, tr("Warning"), tr("Can't open the url(s)"));
}

void CCompositWidget::actionBookmarkSelectAll_triggered()
{
    m_bookmarkView->selectAll();
}

void CCompositWidget::actionBookmarkAdd_triggered()
{
    CBookmarkEditDialog newBookmarkDialog(this);
    newBookmarkDialog.setWindowTitle(tr("Create bookmark"));

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

void CCompositWidget::actionBookmarkEdit_triggered()
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

void CCompositWidget::actionBookmarkSendToTrash_triggered()
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

void CCompositWidget::actionBookmarkRestore_triggered()
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

void CCompositWidget::actionTagAdd_triggered()
{
    CTagItem *item = GTagMgr()->rootItem();
    const QModelIndexList &selectedIndexes = m_navTagView->selectionModel()->selectedRows();
    if (!selectedIndexes.isEmpty())
        item = CTagItem::variantToPtr(selectedIndexes.first().data(Qt::UserRole));

    CTagEditDialog newTagDialog(CTagEditDialog::New, item, this);
    if (newTagDialog.exec() == QDialog::Accepted)
        item->add(newTagDialog.toData());
}

void CCompositWidget::actionTagEdit_triggered()
{
    CTagItem *item = CTagItem::variantToPtr(
                m_navTagView->selectionModel()->selectedRows().first()
                    .data(Qt::UserRole));

    CTagEditDialog editTagDialog(CTagEditDialog::Edit, item->parent(), this);
    editTagDialog.setData(item->data());
    if (editTagDialog.exec() == QDialog::Accepted)
        item->setData(editTagDialog.toData());
}

void CCompositWidget::actionTagRemove_triggered()
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

void CCompositWidget::actionEmptyTrash_triggered()
{
    if (QMessageBox::question(this, tr("Question"),
            tr("Are you sure you want to empty the trash?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    for (int i = GBookmarkMgr()->count()-1; i >= 0; --i)
        if (GBookmarkMgr()->at(i)->data().isTrash())
            GBookmarkMgr()->removeAt(i);
}

void CCompositWidget::updateActionState()
{
    int bookmarkCount = m_bookmarkFilterDataModel->count();
    int selectedBookmarkCount = m_bookmarkView->selectionModel()->selectedRows().count();
    int selectedTagCount = m_navTagView->selectionModel()->selectedRows().count();

    bool hasTrashBookmarks = false;
    bool hasNotTrashBookmarks = false;
    foreach (QModelIndex index, m_bookmarkView->selectionModel()->selectedRows())
    {
        if (CBookmarkItem::variantToPtr(index.data(Qt::UserRole))->data().isTrash())
            hasTrashBookmarks = true;
        else
            hasNotTrashBookmarks = true;

        if (hasTrashBookmarks && hasNotTrashBookmarks)
            break;
    }

    m_actionBookmarkOpenUrl->setEnabled(selectedBookmarkCount == 1);
    m_menuBookmarkOpenUrl->setEnabled(selectedBookmarkCount);

    m_actionBookmarkEdit->setEnabled(selectedBookmarkCount == 1);
    m_actionBookmarkSelectAll->setEnabled(bookmarkCount);
    m_actionBookmarkSendToTrash->setEnabled(hasNotTrashBookmarks);
    m_actionBookmarkRestore->setEnabled(hasTrashBookmarks);

    m_actionTagAdd->setEnabled(selectedTagCount <= 1);
    m_actionTagEdit->setEnabled(selectedTagCount == 1);
    m_actionTagRemove->setEnabled(selectedTagCount);

    m_actionEmptyTrash->setEnabled(GBookmarkMgr()->trashCount());
}

void CCompositWidget::updateOpenUrlActionState()
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

void CCompositWidget::navActMoveTags(const QList<QStringList> &tags,
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

void CCompositWidget::navActSetTag(const QList<QUrl> &bookmarks,
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

void CCompositWidget::navActAddTag(const QList<QUrl> &bookmarks,
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

void CCompositWidget::navActClearTags(const QList<QUrl> &bookmarks)
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

void CCompositWidget::navActFavorite(const QList<QUrl> &bookmarks)
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

void CCompositWidget::navActReadItLater(const QList<QUrl> &bookmarks)
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

void CCompositWidget::navActTrash(const QList<QUrl> &bookmarks)
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

void CCompositWidget::updateBookmarkFilter()
{
    if (!m_navAnchorView->selectionModel()->selectedRows().isEmpty())
        updateBookmarkAnchorFilter();
    else if (!m_navTagView->selectionModel()->selectedRows().isEmpty())
        updateBookmarkTagFilter();

    updateActionState();
}

void CCompositWidget::updateBookmarkAnchorFilter()
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

void CCompositWidget::updateBookmarkTagFilter()
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

QMenu *CCompositWidget::createOpenUrlMenu()
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
        connect(action, SIGNAL(triggered()),
                this, SLOT(actionBookmarkOpenUrlExt_triggered()));
        action->setData(map);

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
        connect(action, SIGNAL(triggered()),
                this, SLOT(actionBookmarkOpenUrlExt_triggered()));
        action->setData(map);

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
        connect(action, SIGNAL(triggered()),
                this, SLOT(actionBookmarkOpenUrlExt_triggered()));
        action->setData(map);

        openUrlNewPrivateWindowMenu->addAction(action);
    }

    return openUrlMenu;
}



//    int selectedAnchorCount = m_navAnchorView->selectionModel()->selectedRows().count();
//    CNavAnchorItemModel::AnchorType anchorType = CNavAnchorItemModel::All;
//    if (selectedAnchorCount)
//        anchorType = static_cast<CNavAnchorItemModel::AnchorType>
//                (m_navAnchorView->currentIndex().data(Qt::UserRole).toInt());
//static QString md5(const QString &str)
//{
//    QCryptographicHash hash(QCryptographicHash::Md5);
//    hash.addData(str.toUtf8());
//    return hash.result().toHex();
//}

//static QString sha1(const QString &str)
//{
//    QCryptographicHash hash(QCryptographicHash::Sha1);
//    hash.addData(str.toUtf8());
//    return hash.result().toHex();
//}

//CCompositWidget::CCompositWidget(QWidget *parent) :
//    QWidget(parent)
//{
//    m_network = new QNetworkAccessManager(this);

//    m_filter = new CBookmarkFilter(this);
//    m_dataModel = new CBookmarkFilterDataModel(this);
//    m_dataModel->setFilter(m_filter);
//    m_bookmarkItemModel = new CBookmarkItemModel(m_dataModel, this);

//    m_bookmarkView = new CBookmarkView(this);
//    m_bookmarkView->setContextMenuPolicy(Qt::CustomContextMenu);
//    m_bookmarkView->setBookmarkModel(m_bookmarkItemModel);

//    //m_sortFilterItemModel = new QSortFilterProxyModel(this);
//    //m_sortFilterItemModel->setSourceModel(m_bookmarkItemModel);
//    //m_sortFilterItemModel->setSortCaseSensitivity(Qt::CaseInsensitive);
//    //m_sortFilterItemModel->setDynamicSortFilter(true);
//    //m_bookmarkView->setModel(m_sortFilterItemModel);
//    connect(m_bookmarkView, SIGNAL(customContextMenuRequested(QPoint)),
//            this, SLOT(bookmarkView_showContextMenu(QPoint)));
//    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
//            this, SLOT(updateActions()));
//    connect(m_bookmarkView, SIGNAL(doubleClicked(QModelIndex)),
//            this, SLOT(bookmarkView_doubleClicked(QModelIndex)));

//    m_navigationItemModel = new CNavigationItemModel(this);
//    m_navigationView = new CNavigationView(this);
//    m_navigationView->setModel(m_navigationItemModel);
//    m_navigationItemModel->setNavigationActions(m_navigationView);
//    connect(m_navigationView, SIGNAL(customContextMenuRequested(QPoint)),
//            this, SLOT(navigationView_showContextMenu(QPoint)));
//    connect(m_navigationView->selectionModel(),
//            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
//            this, SLOT(navigation_selection_selectionChanged()));


//    m_actionBookmarkAdd = new QAction(tr("Add..."), this);
//    m_actionBookmarkEdit = new QAction(tr("Edit..."), this);
//    m_actionBookmarkRemove = new QAction(tr("Remove..."), this);
//    m_actionBookmarkScreenshot = new QAction(tr("Screenshot..."), this);
//    m_actionBookmarkDownload = new QAction(tr("Download..."), this);

//    connect(m_actionBookmarkAdd, SIGNAL(triggered()),
//            this, SLOT(actionBookmarkAdd_triggered()));
//    connect(m_actionBookmarkEdit, SIGNAL(triggered()),
//            this, SLOT(actionBookmarkEdit_triggered()));
//    connect(m_actionBookmarkRemove, SIGNAL(triggered()),
//            this, SLOT(actionBookmarkRemove_triggered()));
//    connect(m_actionBookmarkScreenshot, SIGNAL(triggered()),
//            this, SLOT(actionBookmarkScreenshot_triggered()));
//    connect(m_actionBookmarkDownload, SIGNAL(triggered()),
//            this, SLOT(actionBookmarkDownload_triggered()));


//    m_actionTagAdd = new QAction(tr("Add..."), this);
//    m_actionTagEdit = new QAction(tr("Edit..."), this);
//    m_actionTagRemove = new QAction(tr("Remove..."), this);

//    connect(m_actionTagAdd, SIGNAL(triggered()),
//            this, SLOT(actionTagAdd_triggered()));
//    connect(m_actionTagEdit, SIGNAL(triggered()),
//            this, SLOT(actionTagEdit_triggered()));
//    connect(m_actionTagRemove, SIGNAL(triggered()),
//            this, SLOT(actionTagRemove_triggered()));


//    QSplitter *s = new QSplitter(Qt::Vertical, this);

//    CNavAnchorItemModel *anchorModel = new CNavAnchorItemModel(this);
//    anchorModel->setNavigationActions(m_navigationView);
//    QTreeView *anchorView = new QTreeView(this);
//    anchorView->setHeaderHidden(true);
//    anchorView->setModel(anchorModel);
//    anchorView->setRootIsDecorated(true);
//    anchorView->setSelectionMode(QAbstractItemView::SingleSelection);
//    anchorView->setDragDropMode(QAbstractItemView::DropOnly);
//    anchorView->setContextMenuPolicy(Qt::CustomContextMenu);
//    anchorView->viewport()->setAcceptDrops(true);
//    anchorView->setDropIndicatorShown(true);

//    CNavTagItemModel *tagItemModel = new CNavTagItemModel(this);
//    tagItemModel->setNavigationActions(m_navigationView);
//    QTreeView *x = new QTreeView(this);
//    x->setModel(tagItemModel);
//    x->setHeaderHidden(true);
//    x->setRootIsDecorated(true);
//    x->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    x->setDragDropMode(QAbstractItemView::DragDrop);
//    x->setContextMenuPolicy(Qt::CustomContextMenu);
//    x->viewport()->setAcceptDrops(true);
//    x->setDropIndicatorShown(true);

//    s->addWidget(anchorView);
//    //s->addWidget(m_navigationView);
//    s->addWidget(x);
//    s->setStretchFactor(1, 1);

//    m_navigationView->setRootIsDecorated(false);
//    m_navigationView->setHeaderHidden(true);
//    m_navigationView->hide();

//    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
//    //splitter->addWidget(anchorView);
//    //splitter->addWidget(m_navigationView);
//    splitter->addWidget(s);
//    splitter->addWidget(m_bookmarkView);
//    splitter->setStretchFactor(1, 1);

//    QHBoxLayout *hbox = new QHBoxLayout;
//    hbox->addWidget(splitter);
//    setLayout(hbox);
//}

//CCompositWidget::~CCompositWidget()
//{
//}

//void CCompositWidget::navigation_selection_selectionChanged()
//{
//    foreach (const QModelIndex &index,
//             m_navigationView->selectionModel()->selectedRows())
//    {
//        m_filter->setInclusiveOption(~Bookmark::FilterOptions(Bookmark::Trash));
//        m_filter->setRatingRange(Bookmark::MinRating, Bookmark::MaxRating);

//        QSet<CTagItem *> tags;
//        if (index.internalPointer()
//                && (index.internalPointer()
//                    != GTagMgr()->rootItem()))
//        {
//            tags.insert(static_cast<CTagItem *>(index.internalPointer()));
//        }
//        else
//        {
//            CNavigationItemModel::TopLevelItem type =
//                    static_cast<CNavigationItemModel::TopLevelItem>(
//                        index.data(Qt::UserRole).toInt());

//            switch (type)
//            {
//            case CNavigationItemModel::Favorites:
//                m_filter->setInclusiveOption(
//                            Bookmark::FilterOptions(
//                                Bookmark::Favorite|Bookmark::NotTrash));
//                break;
//            case CNavigationItemModel::ReadLater:
//                m_filter->setInclusiveOption(
//                            Bookmark::FilterOptions(
//                                Bookmark::ReadLater|Bookmark::NotTrash));
//                break;
//            case CNavigationItemModel::Rated:
//                m_filter->setRatingRange(
//                            Bookmark::MinRating+1, Bookmark::MaxRating);
//                break;
//            case CNavigationItemModel::Trash:
//                m_filter->setInclusiveOption(
//                            Bookmark::FilterOptions(Bookmark::Trash));
//                break;
//            case CNavigationItemModel::Untagged:
//                tags.insert(GTagMgr()->rootItem());
//                break;
//            default:
//                ;
//            }
//        }

//        m_filter->setTags(tags);
//        m_filter->update();

//        break; // only first selected item
//    }
//}

//void CCompositWidget::actionTagAdd_triggered()
//{
//    CTagItem *item = 0;
//    foreach (const QModelIndex &index,
//             m_navigationView->selectionModel()->selectedRows())
//    {
//        item = reinterpret_cast<CTagItem *>(
//                    index.internalPointer());
//        break;
//    }

//    if (item)
//    {
//        CTagEditDialog dlg(CTagEditDialog::New, item, this);
//        if (dlg.exec() == QDialog::Accepted)
//            item->add(dlg.toData());
//    }
//}

//void CCompositWidget::actionTagEdit_triggered()
//{
//    CTagItem *item = 0;
//    foreach (const QModelIndex &index,
//             m_navigationView->selectionModel()->selectedRows())
//    {
//        item = reinterpret_cast<CTagItem *>(
//                    index.internalPointer());
//        break;
//    }

//    if (item && item != GTagMgr()->rootItem())
//    {
//        CTagEditDialog dlg(CTagEditDialog::Edit, item->parent(), this);
//        dlg.setData(item->data());
//        if (dlg.exec() == QDialog::Accepted)
//            item->setData(dlg.toData());
//    }
//}

//void CCompositWidget::actionTagRemove_triggered()
//{
//    if (QMessageBox::question(this, tr("Question"), tr("Remove tag?"),
//            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
//        return;

//    foreach (const QModelIndex &index,
//             m_navigationView->selectionModel()->selectedRows())
//    {
//        CTagItem *item = reinterpret_cast<CTagItem *>(
//                    index.internalPointer());
//        item->parent()->removeAt(item->index());
//    }
//}

//void CCompositWidget::actionBookmarkAdd_triggered()
//{
//    CBookmarkEditDialog newBookmarkDialog(this);
//    newBookmarkDialog.setWindowTitle(tr("New bookmark"));

//    if (newBookmarkDialog.exec() == QDialog::Accepted)
//    {
//        CBookmark data = newBookmarkDialog.toData();
//        if (GBookmarkMgr()->find(data.url()))
//        {
//            QMessageBox::warning(this, tr("Warning"),
//                                 tr("The url \"%1\" is already exists")
//                                    .arg(data.url().toString()));
//        }
//        else
//        {
//            CBookmark bookmark = newBookmarkDialog.toData();
//            QIcon favicon = newBookmarkDialog.toFavicon();
//            QSet<CTagItem *> checkedTags = newBookmarkDialog.toCheckedTags();

//            CBookmarkItem *bookmarkItem = GBookmarkMgr()->add(bookmark);
//            GIconMgr()->saveIcon(bookmark.url(), favicon);

//            foreach (CTagItem *tag, checkedTags)
//                tag->bookmarkAdd(bookmarkItem);
//        }
//    }
//}

//void CCompositWidget::actionBookmarkEdit_triggered()
//{
//    CBookmarkItem *bookmarkItem = 0;
//    foreach (const QModelIndex &index,
//             m_bookmarkView->selectionModel()->selectedRows())
//    {
//        bookmarkItem = CBookmarkItem::variantToPtr(index.data(Qt::UserRole));
//        break;
//    }

//    if (!bookmarkItem)
//        return;

//    CBookmarkEditDialog editBookmarkDialog(this);
//    editBookmarkDialog.setWindowTitle(tr("Edit bookmark"));
//    editBookmarkDialog.setData(bookmarkItem->data());
//    editBookmarkDialog.setFavicon(GIconMgr()->icon(bookmarkItem->data().url()));
//    editBookmarkDialog.setCheckedTags(bookmarkItem->tags());
//    if (editBookmarkDialog.exec() == QDialog::Accepted)
//    {
//        CBookmark bookmark = editBookmarkDialog.toData();
//        QIcon favicon = editBookmarkDialog.toFavicon();
//        QSet<CTagItem *> checkedTags = editBookmarkDialog.toCheckedTags();

//        bookmarkItem->setData(bookmark);
//        if (!favicon.isNull())
//            GIconMgr()->saveIcon(bookmark.url(), favicon);

//        // TODO: optimize me!!!
//        foreach (CTagItem *tagItem, bookmarkItem->tags())
//            tagItem->bookmarkRemove(bookmarkItem);
//        foreach (CTagItem *tagItem, checkedTags)
//            tagItem->bookmarkAdd(bookmarkItem);
//    }
//}

//void CCompositWidget::actionBookmarkRemove_triggered()
//{
//    if (QMessageBox::question(this, tr("Question"), tr("Remove bookmark(s)?"),
//            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
//        return;

//    foreach (const QModelIndex &index,
//             m_bookmarkView->selectionModel()->selectedRows())
//    {
//        CBookmarkItem *item = CBookmarkItem::variantToPtr(index.data(Qt::UserRole));
//        CBookmark data = item->data();
//        data.setTrash(true);
//        item->setData(data);
//    }
//}

//void CCompositWidget::actionBookmarkScreenshot_triggered()
//{
//    foreach (const QModelIndex &index,
//             m_bookmarkView->selectionModel()->selectedRows())
//    {
//        m_list << CBookmarkItem::variantToPtr(index.data(Qt::UserRole));
//    }

//    screenshot_next();
//}

//void CCompositWidget::actionBookmarkDownload_triggered()
//{
//    foreach (const QModelIndex &index,
//             m_bookmarkView->selectionModel()->selectedRows())
//    {
//        m_listDl << CBookmarkItem::variantToPtr(index.data(Qt::UserRole));
//    }

//    download_next();
//}

//void CCompositWidget::bookmarkView_showContextMenu(const QPoint &pos)
//{
//    QMenu menu(this);
//    menu.addAction(m_actionBookmarkAdd);
//    menu.addAction(m_actionBookmarkEdit);
//    menu.addAction(m_actionBookmarkRemove);
//    menu.addSeparator();
//    //actionFavorite->setIcon(QIcon(":/icons/bookmark-favorites.png"));
//    //actionFavorite->setCheckable(true);
//    QMenu *test = new QMenu("Favorite", this);
//    QActionGroup *group = new QActionGroup(this);
//    QAction *yes = test->addAction("Yes");
//    QAction *no = test->addAction("No");
//    group->addAction(yes);
//    group->addAction(no);
//    yes->setCheckable(true);
//    no->setCheckable(true);
//    yes->setChecked(true);

//    QAction *readItLaterFavorite = new QAction("Read it later", this);
//    readItLaterFavorite->setCheckable(true);

//    menu.addAction(test->menuAction());
//    menu.addAction(readItLaterFavorite);

//    menu.addSeparator();
//    menu.addAction(m_actionBookmarkScreenshot);
//    menu.addAction(m_actionBookmarkDownload);

//    if (m_bookmarkView->selectionModel()->selectedRows().count() == 1)
//    {
//        menu.addSeparator();
//        QMenu *downloadMenu = new QMenu(tr("Downloaded web pages"), &menu);
//        menu.addAction(menu.addMenu(downloadMenu));

//        CBookmarkItem *item = CBookmarkItem::variantToPtr(
//                    m_bookmarkView->selectionModel()->selectedRows().first().data(Qt::UserRole));

//        QString subdir = sha1(item->data().url().toString()) + "_" + md5(item->data().url().toString());
//        QString path = GPrj()->downloadsPath();
//        if (QDir(path + "/" + subdir).exists())
//        {
//            foreach (const QString &dirName, QDir(path + "/" + subdir).entryList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name|QDir::Reversed))
//            {
//                QAction *action = new QAction(dirName, &menu);
//                connect(action, SIGNAL(triggered()), this, SLOT(download_openUrl()));
//                downloadMenu->addAction(action);
//                action->setData(path + "/" + subdir + "/" + dirName + "/" + "index.html");
//            }
//        }
//    }

//    if (m_bookmarkView->selectionModel()->selectedRows().count() == 1)
//    {
//        QMenu *downloadMenu = new QMenu(tr("Screenshots"), &menu);
//        menu.addAction(menu.addMenu(downloadMenu));

//        CBookmarkItem *item = CBookmarkItem::variantToPtr(
//                    m_bookmarkView->selectionModel()->selectedRows().first().data(Qt::UserRole));

//        QString subdir = sha1(item->data().url().toString()) + "_" + md5(item->data().url().toString());
//        QString path = GPrj()->screenshotPath();
//        if (QDir(path + "/" + subdir).exists())
//        {
//            foreach (const QString &fileName, QDir(path + "/" + subdir).entryList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name|QDir::Reversed))
//            {
//                if (!fileName.endsWith(".png"))
//                    continue;

//                QAction *action = new QAction(fileName, &menu);
//                connect(action, SIGNAL(triggered()), this, SLOT(download_openUrl()));
//                downloadMenu->addAction(action);
//                action->setData(path + "/" + subdir + "/" + fileName);
//            }
//        }
//    }

//    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
//}

//void CCompositWidget::bookmarkView_doubleClicked(const QModelIndex &index)
//{
//    CBookmarkItem *item  = CBookmarkItem::variantToPtr(index.data(Qt::UserRole));
//    if (!item)
//        return;

//    QDesktopServices::openUrl(item->data().url());
//}

//void CCompositWidget::navigationView_showContextMenu(const QPoint &pos)
//{
//    QMenu menu(this);
//    menu.addAction(m_actionTagAdd);
//    menu.addAction(m_actionTagEdit);
//    menu.addAction(m_actionTagRemove);
//    menu.exec(m_navigationView->viewport()->mapToGlobal(pos));
//}

//void CCompositWidget::updateActions()
//{
//    int selectedCount = m_bookmarkView->selectionModel()->selectedRows().count();
//    m_actionBookmarkEdit->setEnabled((selectedCount == 1));
//    m_actionBookmarkRemove->setEnabled(selectedCount);
//    m_actionBookmarkScreenshot->setEnabled(selectedCount);
//}

//void CCompositWidget::screenshot_next()
//{
//    if (m_list.isEmpty())
//        return;
//    CBookmarkItem *item = m_list.takeFirst();
//    qDebug() << item->data().url() << " left " << m_list.count();

//    CWebPageScreenshot *screenshot = new CWebPageScreenshot(m_network, this);
//    connect(screenshot, SIGNAL(finished()), screenshot, SLOT(deleteLater()));
//    connect(screenshot, SIGNAL(finished()), this, SLOT(screenshot_finished()));
//    connect(screenshot, SIGNAL(finished()), this, SLOT(screenshot_next()));

//    screenshot->setUrl(item->data().url());
//    screenshot->setScreenshotSize(QSize(1280, 1024));
//    screenshot->start();
//}

//void CCompositWidget::screenshot_finished()
//{
//    CWebPageScreenshot *screenshot = qobject_cast<CWebPageScreenshot *>(sender());
//    if (screenshot->error() != CWebPageScreenshot::NoError)
//        return;

//    QImage image = screenshot->screenshot();

//    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz.png");
//    QString subdir = sha1(screenshot->url().toString()) + "_" + md5(screenshot->url().toString());
//    QString path = GPrj()->screenshotPath();
//    QString res = path + "/" + subdir + "/" + fileName;
//    if (!QDir(path + "/" + subdir).exists())
//        QDir().mkpath(path + "/" + subdir);

//    QFile file(path + "/" + subdir + "/url.txt");
//    file.open(QIODevice::WriteOnly);
//    file.write(screenshot->url().toString().toUtf8());
//    file.close();


//    image.save(res);
//}

//void CCompositWidget::download_openUrl()
//{
//    QAction *action = qobject_cast<QAction *>(sender());
//    QDesktopServices::openUrl("file://" + action->data().toString());
//}

//void CCompositWidget::download_next()
//{
//    if (m_listDl.isEmpty())
//        return;
//    CBookmarkItem *item = m_listDl.takeFirst();
//    qDebug() << item->data().url() << " left " << m_listDl.count();


//    CWebPageDownloader *download = new CWebPageDownloader(m_network, this);
//    connect(download, SIGNAL(finished()), download, SLOT(deleteLater()));
//    connect(download, SIGNAL(finished()), this, SLOT(download_finishied()));
//    connect(download, SIGNAL(finished()), this, SLOT(download_next()));

//    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz");
//    QString subdir = sha1(item->data().url().toString()) + "_" + md5(item->data().url().toString());
//    QString path = GPrj()->downloadsPath();
//    QString res = path + "/" + subdir + "/" + fileName;
//    if (!QDir(res).exists())
//        QDir().mkpath(res);

//    QFile file(path + "/" + subdir + "/url.txt");
//    file.open(QIODevice::WriteOnly);
//    file.write(item->data().url().toString().toUtf8());
//    file.close();

//    download->setUrl(item->data().url());
//    download->setFileName(res + "/index.html");
//    download->start();
//}

//void CCompositWidget::download_finishied()
//{
//}


