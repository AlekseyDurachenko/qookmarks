#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include "ctagitem.h"
#include "cbookmarkitem.h"
class CBookmarkFilterDataModel;
class CBookmarkFilter;
class CManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

private:
    Ui::MainWindow *ui;
    CBookmarkFilterDataModel *dataModel;
    CBookmarkFilter *filter;
    CManager *mgr;
};

#endif // MAINWINDOW_H
