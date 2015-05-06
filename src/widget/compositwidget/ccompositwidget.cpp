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
#include <QDesktopServices>
#include "cnavigationview.h"
#include "cbookmarkview.h"
#include "cnavigationitemmodel.h"
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
#include <QSettings>
#include <QSortFilterProxyModel>

static QString md5(const QString &str)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(str.toUtf8());
    return hash.result().toHex();
}

static QString sha1(const QString &str)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(str.toUtf8());
    return hash.result().toHex();
}

CCompositWidget::CCompositWidget(QWidget *parent) :
    QWidget(parent)
{
    m_network = new QNetworkAccessManager(this);

    m_filter = new CBookmarkFilter(this);
    m_dataModel = new CBookmarkFilterDataModel(this);
    m_dataModel->setFilter(m_filter);
    m_bookmarkItemModel = new CBookmarkItemModel(m_dataModel, this);
    m_bookmarkView = new CBookmarkView(this);
    m_sortFilterItemModel = new QSortFilterProxyModel(this);
    m_sortFilterItemModel->setSourceModel(m_bookmarkItemModel);
    m_sortFilterItemModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    m_sortFilterItemModel->setDynamicSortFilter(true);
    m_bookmarkView->setModel(m_sortFilterItemModel);
    connect(m_bookmarkView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bookmarkView_showContextMenu(QPoint)));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateActions()));
    connect(m_bookmarkView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(bookmarkView_doubleClicked(QModelIndex)));

    m_bookmarkView->setHeader(new CBookmarkHeaderView(Qt::Horizontal, m_bookmarkView));
    m_bookmarkView->header()->setMinimumSectionSize(10);
    m_bookmarkView->setSortingEnabled(true);
#if QT_VERSION >= 0x050000
    m_bookmarkView->header()->setSectionsMovable(true);
    m_bookmarkView->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_bookmarkView->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_bookmarkView->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    m_bookmarkView->header()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
#else
    m_bookmarkView->header()->setMovable(true);
    m_bookmarkView->header()->setResizeMode(4, QHeaderView::ResizeToContents);
    m_bookmarkView->header()->setResizeMode(5, QHeaderView::ResizeToContents);
    m_bookmarkView->header()->setResizeMode(6, QHeaderView::ResizeToContents);
    m_bookmarkView->header()->setResizeMode(7, QHeaderView::ResizeToContents);
#endif

    m_navigationItemModel = new CNavigationItemModel(this);
    m_navigationView = new CNavigationView(this);
    m_navigationView->setModel(m_navigationItemModel);
    m_navigationItemModel->setNavigationActions(m_navigationView);
    connect(m_navigationView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(navigationView_showContextMenu(QPoint)));
    connect(m_navigationView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(navigation_selection_selectionChanged()));


    m_actionBookmarkAdd = new QAction(tr("Add..."), this);
    m_actionBookmarkEdit = new QAction(tr("Edit..."), this);
    m_actionBookmarkRemove = new QAction(tr("Remove..."), this);
    m_actionBookmarkScreenshot = new QAction(tr("Screenshot..."), this);
    m_actionBookmarkDownload = new QAction(tr("Download..."), this);

    connect(m_actionBookmarkAdd, SIGNAL(triggered()),
            this, SLOT(actionBookmarkAdd_triggered()));
    connect(m_actionBookmarkEdit, SIGNAL(triggered()),
            this, SLOT(actionBookmarkEdit_triggered()));
    connect(m_actionBookmarkRemove, SIGNAL(triggered()),
            this, SLOT(actionBookmarkRemove_triggered()));
    connect(m_actionBookmarkScreenshot, SIGNAL(triggered()),
            this, SLOT(actionBookmarkScreenshot_triggered()));
    connect(m_actionBookmarkDownload, SIGNAL(triggered()),
            this, SLOT(actionBookmarkDownload_triggered()));


    m_actionTagAdd = new QAction(tr("Add..."), this);
    m_actionTagEdit = new QAction(tr("Edit..."), this);
    m_actionTagRemove = new QAction(tr("Remove..."), this);

    connect(m_actionTagAdd, SIGNAL(triggered()),
            this, SLOT(actionTagAdd_triggered()));
    connect(m_actionTagEdit, SIGNAL(triggered()),
            this, SLOT(actionTagEdit_triggered()));
    connect(m_actionTagRemove, SIGNAL(triggered()),
            this, SLOT(actionTagRemove_triggered()));


    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(m_navigationView);
    splitter->addWidget(m_bookmarkView);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(splitter);
    setLayout(hbox);
}

CCompositWidget::~CCompositWidget()
{
}

void CCompositWidget::navigation_selection_selectionChanged()
{
    foreach (const QModelIndex &index,
             m_navigationView->selectionModel()->selectedRows())
    {
        m_filter->setInclusiveOption(~Bookmark::FilterOptions(Bookmark::Trash));
        m_filter->setRatingRange(Bookmark::MinRating, Bookmark::MaxRating);

        QSet<CTagItem *> tags;
        if (index.internalPointer()
                && (index.internalPointer()
                    != GTagMgr()->rootItem()))
        {
            tags.insert(static_cast<CTagItem *>(index.internalPointer()));
        }
        else
        {
            CNavigationItemModel::TopLevelItem type =
                    static_cast<CNavigationItemModel::TopLevelItem>(
                        index.data(Qt::UserRole).toInt());

            switch (type)
            {
            case CNavigationItemModel::Favorites:
                m_filter->setInclusiveOption(
                            Bookmark::FilterOptions(
                                Bookmark::Favorite|Bookmark::NotTrash));
                break;
            case CNavigationItemModel::ReadLater:
                m_filter->setInclusiveOption(
                            Bookmark::FilterOptions(
                                Bookmark::ReadLater|Bookmark::NotTrash));
                break;
            case CNavigationItemModel::Rated:
                m_filter->setRatingRange(
                            Bookmark::MinRating+1, Bookmark::MaxRating);
                break;
            case CNavigationItemModel::Trash:
                m_filter->setInclusiveOption(
                            Bookmark::FilterOptions(Bookmark::Trash));
                break;
            case CNavigationItemModel::Untagged:
                tags.insert(GTagMgr()->rootItem());
                break;
            default:
                ;
            }
        }

        m_filter->setTags(tags);
        m_filter->update();

        break; // only first selected item
    }
}

void CCompositWidget::actionTagAdd_triggered()
{
    CTagItem *item = 0;
    foreach (const QModelIndex &index,
             m_navigationView->selectionModel()->selectedRows())
    {
        item = reinterpret_cast<CTagItem *>(
                    index.internalPointer());
        break;
    }

    if (item)
    {
        CTagEditDialog dlg(CTagEditDialog::New, item, this);
        if (dlg.exec() == QDialog::Accepted)
            item->add(dlg.toData());
    }
}

void CCompositWidget::actionTagEdit_triggered()
{
    CTagItem *item = 0;
    foreach (const QModelIndex &index,
             m_navigationView->selectionModel()->selectedRows())
    {
        item = reinterpret_cast<CTagItem *>(
                    index.internalPointer());
        break;
    }

    if (item && item != GTagMgr()->rootItem())
    {
        CTagEditDialog dlg(CTagEditDialog::Edit, item->parent(), this);
        dlg.setData(item->data());
        if (dlg.exec() == QDialog::Accepted)
            item->setData(dlg.toData());
    }
}

void CCompositWidget::actionTagRemove_triggered()
{
    if (QMessageBox::question(this, tr("Question"), tr("Remove tag?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (const QModelIndex &index,
             m_navigationView->selectionModel()->selectedRows())
    {
        CTagItem *item = reinterpret_cast<CTagItem *>(
                    index.internalPointer());
        item->parent()->removeAt(item->index());
    }
}

void CCompositWidget::actionBookmarkAdd_triggered()
{
    CBookmarkEditDialog newBookmarkDialog(this);
    newBookmarkDialog.setWindowTitle(tr("New bookmark"));

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
    CBookmarkItem *bookmarkItem = 0;
    foreach (const QModelIndex &indexF,
             m_bookmarkView->selectionModel()->selectedRows())
    {
        QModelIndex index = m_sortFilterItemModel->mapToSource(indexF);
        bookmarkItem = reinterpret_cast<CBookmarkItem *>(index.internalPointer());
        break;
    }

    if (!bookmarkItem)
        return;

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

        bookmarkItem->setData(bookmark);
        if (!favicon.isNull())
            GIconMgr()->saveIcon(bookmark.url(), favicon);

        // TODO: optimize me!!!
        foreach (CTagItem *tagItem, bookmarkItem->tags())
            tagItem->bookmarkRemove(bookmarkItem);
        foreach (CTagItem *tagItem, checkedTags)
            tagItem->bookmarkAdd(bookmarkItem);
    }
}

void CCompositWidget::actionBookmarkRemove_triggered()
{
    if (QMessageBox::question(this, tr("Question"), tr("Remove bookmark(s)?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (const QModelIndex &indexF,
             m_bookmarkView->selectionModel()->selectedRows())
    {
        QModelIndex index = m_sortFilterItemModel->mapToSource(indexF);
        CBookmarkItem *item = reinterpret_cast<CBookmarkItem *>(
                    index.internalPointer());
        CBookmark data = item->data();
        data.setTrash(true);
        item->setData(data);
    }
}

void CCompositWidget::actionBookmarkScreenshot_triggered()
{
    foreach (const QModelIndex &indexF,
             m_bookmarkView->selectionModel()->selectedRows())
    {
        QModelIndex index = m_sortFilterItemModel->mapToSource(indexF);
        m_list << reinterpret_cast<CBookmarkItem *>(index.internalPointer());
    }

    screenshot_next();
}

void CCompositWidget::actionBookmarkDownload_triggered()
{
    foreach (const QModelIndex &indexF,
             m_bookmarkView->selectionModel()->selectedRows())
    {
        QModelIndex index = m_sortFilterItemModel->mapToSource(indexF);
        m_listDl << reinterpret_cast<CBookmarkItem *>(index.internalPointer());
    }

    download_next();
}

void CCompositWidget::bookmarkView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionBookmarkAdd);
    menu.addAction(m_actionBookmarkEdit);
    menu.addAction(m_actionBookmarkRemove);
    menu.addAction(m_actionBookmarkScreenshot);
    menu.addAction(m_actionBookmarkDownload);

    if (m_bookmarkView->selectionModel()->selectedRows().count() == 1)
    {
        menu.addSeparator();
        QMenu *downloadMenu = new QMenu(tr("Downloaded web pages"), &menu);
        menu.addAction(menu.addMenu(downloadMenu));

        CBookmarkItem *item = reinterpret_cast<CBookmarkItem *>(
                    m_sortFilterItemModel->mapToSource(m_bookmarkView->selectionModel()->selectedRows().first()).internalPointer());

        QString subdir = sha1(item->data().url().toString()) + "_" + md5(item->data().url().toString());
        QString path = GPrj()->downloadsPath();
        if (QDir(path + "/" + subdir).exists())
        {
            foreach (const QString &dirName, QDir(path + "/" + subdir).entryList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name|QDir::Reversed))
            {
                QAction *action = new QAction(dirName, &menu);
                connect(action, SIGNAL(triggered()), this, SLOT(download_openUrl()));
                downloadMenu->addAction(action);
                action->setData(path + "/" + subdir + "/" + dirName + "/" + "index.html");
            }
        }
    }

    if (m_bookmarkView->selectionModel()->selectedRows().count() == 1)
    {
        QMenu *downloadMenu = new QMenu(tr("Screenshots"), &menu);
        menu.addAction(menu.addMenu(downloadMenu));

        CBookmarkItem *item = reinterpret_cast<CBookmarkItem *>(
                    m_sortFilterItemModel->mapToSource(m_bookmarkView->selectionModel()->selectedRows().first()).internalPointer());

        QString subdir = sha1(item->data().url().toString()) + "_" + md5(item->data().url().toString());
        QString path = GPrj()->screenshotPath();
        if (QDir(path + "/" + subdir).exists())
        {
            foreach (const QString &fileName, QDir(path + "/" + subdir).entryList(QDir::Files|QDir::NoDotAndDotDot, QDir::Name|QDir::Reversed))
            {
                if (!fileName.endsWith(".png"))
                    continue;

                QAction *action = new QAction(fileName, &menu);
                connect(action, SIGNAL(triggered()), this, SLOT(download_openUrl()));
                downloadMenu->addAction(action);
                action->setData(path + "/" + subdir + "/" + fileName);
            }
        }
    }

    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
}

void CCompositWidget::bookmarkView_doubleClicked(const QModelIndex &indexF)
{
    QModelIndex index = m_sortFilterItemModel->mapToSource(indexF);
    CBookmarkItem *item  = reinterpret_cast<CBookmarkItem *>(index.internalPointer());
    if (!item)
        return;

    QDesktopServices::openUrl(item->data().url());
}

void CCompositWidget::navigationView_showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_actionTagAdd);
    menu.addAction(m_actionTagEdit);
    menu.addAction(m_actionTagRemove);
    menu.exec(m_navigationView->viewport()->mapToGlobal(pos));
}

void CCompositWidget::updateActions()
{
    int selectedCount = m_bookmarkView->selectionModel()->selectedRows().count();
    m_actionBookmarkEdit->setEnabled((selectedCount == 1));
    m_actionBookmarkRemove->setEnabled(selectedCount);
    m_actionBookmarkScreenshot->setEnabled(selectedCount);
}

void CCompositWidget::screenshot_next()
{
    if (m_list.isEmpty())
        return;
    CBookmarkItem *item = m_list.takeFirst();
    qDebug() << item->data().url() << " left " << m_list.count();

    CWebPageScreenshot *screenshot = new CWebPageScreenshot(m_network, this);
    connect(screenshot, SIGNAL(finished()), screenshot, SLOT(deleteLater()));
    connect(screenshot, SIGNAL(finished()), this, SLOT(screenshot_finished()));
    connect(screenshot, SIGNAL(finished()), this, SLOT(screenshot_next()));

    screenshot->setUrl(item->data().url());
    screenshot->setScreenshotSize(QSize(1280, 1024));
    screenshot->start();
}

void CCompositWidget::screenshot_finished()
{
    CWebPageScreenshot *screenshot = qobject_cast<CWebPageScreenshot *>(sender());
    if (screenshot->error() != CWebPageScreenshot::NoError)
        return;

    QImage image = screenshot->screenshot();

    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz.png");
    QString subdir = sha1(screenshot->url().toString()) + "_" + md5(screenshot->url().toString());
    QString path = GPrj()->screenshotPath();
    QString res = path + "/" + subdir + "/" + fileName;
    if (!QDir(path + "/" + subdir).exists())
        QDir().mkpath(path + "/" + subdir);

    QFile file(path + "/" + subdir + "/url.txt");
    file.open(QIODevice::WriteOnly);
    file.write(screenshot->url().toString().toUtf8());
    file.close();


    image.save(res);
}

void CCompositWidget::download_openUrl()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QDesktopServices::openUrl("file://" + action->data().toString());
}

void CCompositWidget::download_next()
{
    if (m_listDl.isEmpty())
        return;
    CBookmarkItem *item = m_listDl.takeFirst();
    qDebug() << item->data().url() << " left " << m_listDl.count();


    CWebPageDownloader *download = new CWebPageDownloader(m_network, this);
    connect(download, SIGNAL(finished()), download, SLOT(deleteLater()));
    connect(download, SIGNAL(finished()), this, SLOT(download_finishied()));
    connect(download, SIGNAL(finished()), this, SLOT(download_next()));

    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz");
    QString subdir = sha1(item->data().url().toString()) + "_" + md5(item->data().url().toString());
    QString path = GPrj()->downloadsPath();
    QString res = path + "/" + subdir + "/" + fileName;
    if (!QDir(res).exists())
        QDir().mkpath(res);

    QFile file(path + "/" + subdir + "/url.txt");
    file.open(QIODevice::WriteOnly);
    file.write(item->data().url().toString().toUtf8());
    file.close();

    download->setUrl(item->data().url());
    download->setFileName(res + "/index.html");
    download->start();
}

void CCompositWidget::download_finishied()
{
}

