#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QtGui>
#include "cmanager.h"
#include "ctagmgr.h"
#include "ctagitem.h"
#include "cbookmarkmgr.h"
#include "cbookmarkitem.h"
#include "bookmarkimportchromium.h"
#include "cbookmarkfilter.h"
#include "cbookmarkfilterdatamodel.h"
#include "cbookmarkfilter.h"
#include "cbookmarkfilteritemmodel.h"
#include "cnavigationitemmodel.h"
#include <QMessageBox>
#include <QPushButton>

//class testInterface : public INavigationActions
//{
//public:
//    virtual void tagsCopyOrMove(const QList<QStringList> &tags,
//                          const QStringList &parentTag) {}
//    virtual void bookmarksAssignTag(const QList<QUrl> &bookmarks,
//                                    const QStringList& tag) {}
//    virtual void bookmarksMarkFavorite(const QList<QUrl> &bookmarks) {}
//    virtual void bookmarksMarkReadLater(const QList<QUrl> &bookmarks) {}
//    virtual void bookmarksMarkTrash(const QList<QUrl> &bookmarks) {}
//};


//void printTagItem(const QString &path, CTagItem *item)
//{
//    foreach (CTagItem *tmp, item->children())
//    {
//        qDebug() << path + tmp->data().name() << "(" << tmp->bookmarks().count() << ")";
//        printTagItem(path + tmp->data().name() + " >> ", tmp);
//    }
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QSet<QString> set;
//    set << "January" << "February" << "December";

//    foreach (QString item, set)
//    {
//        qDebug() << set;
//        qDebug() << item;
//        set.remove(item);
//    }
//    qDebug() << set;


//    QSet<QString> set;
//    set << "January" << "February" << "December";

//    QSet<QString>::iterator i;
//    for (i = set.begin(); i != set.end(); i = set.erase(i))
//    {
//        qDebug() << set;
//        qDebug() << *i;
//    }
//    qDebug() << set;

//    QSet<QString>::iterator i = set.begin();
//    while (i != set.end()) {
//        qDebug() << *i;
//        i = set.erase(i);
//    }

//    qDebug() << set;
//    exit(0);

//    CBookmarkMgr *bookmarkMgr = new CBookmarkMgr(this);
//    CBookmark _b1;
//    _b1.setUrl(QUrl("ru.ru"));
//    CBookmarkItem *b1 = bookmarkMgr->bookmarkAdd(_b1);

//    CTag _t1;
//    _t1.setName("unnamed");
//    CTagItem *t1 = bookmarkMgr->tagRootItem()->addChild(_t1);

//    t1->bookmarkAdd(b1);
//    t1->bookmarkAdd(b1);
    //bookmarkImportChromium(bookmarkMgr, QDir::homePath() + "/.config/chromium/Default/Bookmarks");


//    foreach (CBookmarkItem *item, bookmarkMgr->bookmarks())
//    {
//        qDebug() << item->data().url();
//        foreach (CTagItem *tag, item->tags())
//            qDebug() << "TAG: " << tag->path();
//    }

//    printTagItem("", bookmarkMgr->tagRootItem());

    //CTagItemModel *model = new CTagItemModel(bookmarkMgr->tagRootItem(), this);
    //ui->tag_treeView->setModel(model);

//    mgr = new CManager(this);
//    dataModel = new CBookmarkFilterDataModel(mgr, this);
//    filter = new CBookmarkFilter(mgr, this);
//    dataModel->setFilter(filter);
//    CBookmarkFilteredItemModel *model = new CBookmarkFilteredItemModel(dataModel, this);
//    ui->treeView_bookmarks->setModel(model);


    //bookmarkImportChromium(mgr, QDir::homePath() + "/.config/chromium/Default/Bookmarks");

//    CBookmark data1 = mgr->bookmarkMgr()->at(1)->data();
//    data1.setTrash(true);
//    mgr->bookmarkMgr()->at(1)->setData(data1);

//    foreach (CBookmarkItem *item, mgr->bookmarkMgr()->bookmarks())
//    {
//        qDebug() << item->data().url();
//        foreach (CTagItem *tag, item->tags())
//            qDebug() << "TAG: " << tag->path();
//    }

//    printTagItem("", mgr->tagMgr()->rootItem());

//    CBookmark data = mgr->bookmarkMgr()->at(0)->data();
//    data.setTrash(true);
//    mgr->bookmarkMgr()->at(0)->setData(data);

//    CNavigationItemModel *navItemModel = new CNavigationItemModel(mgr, this);
//    navItemModel->setNavigationActions(this);
//    ui->treeView_tags->setModel(navItemModel);

    m_project = new CPrj(this);

    m_compositeWidget = new CCompositWidget(m_project->manager(), this);
    setCentralWidget(m_compositeWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_action_Quit_triggered()
//{
//    //filter->setInclusiveOption(Bookmark::FilterOptions(Bookmark::Trash));
//    //filter->update();

//    CTag data = mgr->tagMgr()->rootItem()->at(0)->data();
//    data.setName("test");
//    mgr->tagMgr()->rootItem()->at(0)->setData(data);
//    mgr->tagMgr()->rootItem()->at(9)->at(0)->setData(data);




////    CTag tag;
////    tag.setName("xzxzxz");
////    mgr->tagMgr()->rootItem()->add(tag);
////    mgr->tagMgr()->rootItem()->at(0)->add(tag);

//    //mgr->tagMgr()->rootItem()->removeAt(0);

//    //mgr->tagMgr()->rootItem()->at(9)->at(1)->moveTo(mgr->tagMgr()->rootItem());
//    //mgr->tagMgr()->rootItem()->at(6)->moveTo(mgr->tagMgr()->rootItem()->at(0));
//    //mgr->tagMgr()->rootItem()->at(9)->at(0)->moveTo(mgr->tagMgr()->rootItem()->at(0));
//}

//void MainWindow::on_action_Save_triggered()
//{
//    CBookmark data = mgr->bookmarkMgr()->at(14)->data();
//    data.setTrash(true);
//    mgr->bookmarkMgr()->at(14)->setData(data);

//    mgr->bookmarkMgr()->removeAt(1);
//}

//void MainWindow::slot_tagsNeedMoving(const QList<QStringList> &tags,
//        const QStringList &parentTag)
//{
//}

//void MainWindow::slot_bookmarksNeedTagging(const QList<QUrl> &bookmarks,
//        const QStringList &tag)
//{


//    //    QList<CBookmarkItem *> bookmarkItems;
//    //    foreach (const QUrl &url, bookmarkUrls)
//    //    {
//    //        CBookmarkItem *bookmarkItem = m_manager->bookmarkMgr()->find(url);
//    //        if (!bookmarkItem)
//    //            continue;

//    //        bookmarkItems.push_back(bookmarkItem);
//    //    }

//    //    if (bookmarkItems.isEmpty())
//    //        return false;

//    //    emit bookmarksNeedTagging(bookmarkItems, tagParentItem);

//    //    return true;
//}

//void MainWindow::tagsCopyOrMove(const QList<QStringList> &tags, const QStringList &parentTag)
//{
//    CTagItem *parentItem = mgr->tagMgr()->findByPath(parentTag);
//    if (!parentItem)
//        return;

//    QList<CTagItem *> tagItems;
//    foreach (const QStringList &tagPath, tags)
//    {
//        CTagItem *tagItem = mgr->tagMgr()->findByPath(tagPath);
//        if (!tagItem)
//            continue;

//        if (tagItem->parent() == parentItem)
//            continue;

//        if (tagItem->aboveOf(parentItem))
//        {
//            QMessageBox::warning(this, tr("Warning"), tr("Can't move parent to child"));
//            return;
//        }

//        tagItems.push_back(tagItem);
//    }

//    foreach (CTagItem *tagItem, tagItems)
//        tagItem->moveTo(parentItem);
//}

//void MainWindow::bookmarksAssignTag(const QList<QUrl> &bookmarks, const QStringList &tag)
//{
//    CTagItem *tagItem = mgr->tagMgr()->findByPath(tag);
//    if (!tagItem)
//        return;

//    QMessageBox msgBox(QMessageBox::Question, "Action", "What do you want?");
//    QPushButton *moveButton = msgBox.addButton("Move", QMessageBox::ActionRole);
//    QPushButton *copyButton = msgBox.addButton("Copy", QMessageBox::ActionRole);
//    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);

//    if (msgBox.exec() == QMessageBox::Cancel)
//        return;

//    foreach (const QUrl &url, bookmarks)
//    {
//        CBookmarkItem *bookmarkItem = mgr->bookmarkMgr()->find(url);
//        if (bookmarkItem)
//        {
//            if (msgBox.clickedButton() == moveButton)
//                foreach (CTagItem *currentTagItem, bookmarkItem->tags())
//                    if (tagItem != currentTagItem)
//                        currentTagItem->bookmarkRemove(bookmarkItem);

//            tagItem->bookmarkAdd(bookmarkItem);
//        }
//    }
//}

//void MainWindow::bookmarksMarkFavorite(const QList<QUrl> &bookmarks)
//{

//}

//void MainWindow::bookmarksMarkReadLater(const QList<QUrl> &bookmarks)
//{

//}

//void MainWindow::bookmarksMarkTrash(const QList<QUrl> &bookmarks)
//{

//}

////void MainWindow::dragEnterEvent(QDragEnterEvent *event)
////{
////    qDebug() << "ok" << event->format();
////}

void MainWindow::on_action_create_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Create bookmarks"), "",
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    QString reason;
    if (!m_project->create(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);
}

void MainWindow::on_action_open_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
            tr("Open bookmarks"), "",
            QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

    if (dirName.isEmpty())
        return;

    QString reason;
    if (!m_project->open(dirName, &reason))
        QMessageBox::warning(this, tr("Warning"), reason);
}

void MainWindow::on_action_save_triggered()
{
    QString reason;
    qDebug() << m_project->save(&reason);
    qDebug() << reason;
}

void MainWindow::on_action_close_triggered()
{
    m_project->close();
}
