#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include "ctagitem.h"
#include "cbookmarkitem.h"
#include "imanageraction.h"
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IManagerAction
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Quit_triggered();
    void on_action_Save_triggered();

    void slot_tagsNeedMoving(const QList<QStringList> &tags,
                             const QStringList &parentTag);
    void slot_bookmarksNeedTagging(const QList<QUrl> &bookmarks,
                                   const QStringList& tag);
protected:
//    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void tagsCopyOrMove(const QList<QStringList> &tags,
                          const QStringList &parentTag);
    virtual void bookmarksAssignTag(const QList<QUrl> &bookmarks,
                                    const QStringList& tag);
    virtual void bookmarksMarkFavorite(const QList<QUrl> &bookmarks);
    virtual void bookmarksMarkReadLater(const QList<QUrl> &bookmarks);
    virtual void bookmarksMarkTrash(const QList<QUrl> &bookmarks);

private:
    Ui::MainWindow *ui;
    CBookmarkFilterDataModel *dataModel;
    CBookmarkFilter *filter;
    CManager *mgr;
};

#endif // MAINWINDOW_H
