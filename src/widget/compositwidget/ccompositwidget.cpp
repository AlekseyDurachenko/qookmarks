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
#include <QMessageBox>
#include "cnavigationview.h"
#include "cbookmarkview.h"
#include "cnavigationitemmodel.h"
#include "cbookmarkfilter.h"
#include "cbookmarkfilteritemmodel.h"
#include "cbookmarkfilterdatamodel.h"
#include "cmanager.h"
#include "ctagmgr.h"
#include "cbookmarkitem.h"
#include "cbookmarkmgr.h"
#include "cwebpagescreenshot.h"
#include "cwebpagedownloader.h"
#include "cprj.h"
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>


CCompositWidget::CCompositWidget(CPrj *project, QWidget *parent) :
    QWidget(parent)
{
    m_network = new QNetworkAccessManager(this);

    m_project = project;

    m_filter = new CBookmarkFilter(m_project->manager(), this);
    m_dataModel = new CBookmarkFilterDataModel(m_project->manager(), this);
    m_dataModel->setFilter(m_filter);
    m_bookmarkItemModel = new CBookmarkFilteredItemModel(m_dataModel, this);
    m_bookmarkView = new CBookmarkView(this);
    m_bookmarkView->setModel(m_bookmarkItemModel);
    connect(m_bookmarkView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(bookmarkView_showContextMenu(QPoint)));
    connect(m_bookmarkView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateActions()));


    m_navigationItemModel = new CNavigationItemModel(m_project->manager(), this);
    m_navigationView = new CNavigationView(m_project->manager(), this);
    m_navigationView->setModel(m_navigationItemModel);
    m_navigationItemModel->setNavigationActions(m_navigationView);
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
                    != m_project->manager()->tagMgr()->rootItem()))
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
                tags.insert(m_project->manager()->tagMgr()->rootItem());
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

void CCompositWidget::actionBookmarkAdd_triggered()
{

}

void CCompositWidget::actionBookmarkEdit_triggered()
{

}

void CCompositWidget::actionBookmarkRemove_triggered()
{
    if (QMessageBox::question(this, tr("Question"), tr("Remove bookmark(s)?"),
            QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
        return;

    foreach (const QModelIndex &index,
             m_bookmarkView->selectionModel()->selectedRows())
    {
        CBookmarkItem *item = reinterpret_cast<CBookmarkItem *>(
                    index.internalPointer());
        CBookmark data = item->data();
        data.setTrash(true);
        item->setData(data);
    }
}

void CCompositWidget::actionBookmarkScreenshot_triggered()
{
    foreach (const QModelIndex &index,
             m_bookmarkView->selectionModel()->selectedRows())
        m_list << reinterpret_cast<CBookmarkItem *>(index.internalPointer());

    screenshot_next();
}

void CCompositWidget::actionBookmarkDownload_triggered()
{
    foreach (const QModelIndex &index,
             m_bookmarkView->selectionModel()->selectedRows())
        m_listDl << reinterpret_cast<CBookmarkItem *>(index.internalPointer());

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
    menu.exec(m_bookmarkView->viewport()->mapToGlobal(pos));
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

void CCompositWidget::screenshot_finished()
{
    CWebPageScreenshot *screenshot = qobject_cast<CWebPageScreenshot *>(sender());
    if (screenshot->error() != CWebPageScreenshot::NoError)
        return;

    QImage image = screenshot->screenshot();

    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz.png");
    QString subdir = sha1(screenshot->url().toString()) + "_" + md5(screenshot->url().toString());
    QString path = m_project->screenshotPath();
    QString res = path + "/" + subdir + "/" + fileName;
    if (!QDir(path + "/" + subdir).exists())
        QDir().mkpath(path + "/" + subdir);

    QFile file(path + "/" + subdir + "/url.txt");
    file.open(QIODevice::WriteOnly);
    file.write(screenshot->url().toString().toUtf8());
    file.close();


    image.save(res);
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
    QString path = m_project->downloadsPath();
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

