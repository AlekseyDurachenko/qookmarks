#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include "ctagitem.h"
#include "cbookmarkitem.h"
#include "inavigationactions.h"
#include "ccompositwidget.h"
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow //, public INavigationActions
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
private slots:
//    void on_action_Quit_triggered();
//    void on_action_Save_triggered();

//    void slot_tagsNeedMoving(const QList<QStringList> &tags,
//                             const QStringList &parentTag);
//    void slot_bookmarksNeedTagging(const QList<QUrl> &bookmarks,
//                                   const QStringList& tag);
protected:
//    virtual void dragEnterEvent(QDragEnterEvent *event);
//    virtual void tagsCopyOrMove(const QList<QStringList> &tags,
//                          const QStringList &parentTag);
//    virtual void bookmarksAssignTag(const QList<QUrl> &bookmarks,
//                                    const QStringList& tag);
//    virtual void bookmarksMarkFavorite(const QList<QUrl> &bookmarks);
//    virtual void bookmarksMarkReadLater(const QList<QUrl> &bookmarks);
//    virtual void bookmarksMarkTrash(const QList<QUrl> &bookmarks);
private:
    Ui::MainWindow *ui;
//    CBookmarkFilterDataModel *dataModel;
//    CBookmarkFilter *filter;
    CCompositWidget *m_compositeWidget;
    CManager *mgr;
};

#endif // MAINWINDOW_H
