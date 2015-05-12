#ifndef CNAVTAGVIEW_H
#define CNAVTAGVIEW_H

#include <QTreeView>


class CNavTagView : public QTreeView
{
public:
    CNavTagView(QWidget *parent = 0);
protected:
    void dropEvent(QDropEvent *event);
};


#endif // CNAVTAGVIEW_H
