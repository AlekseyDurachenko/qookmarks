#include "cnavtagview.h"
#include <QDragMoveEvent>
#include <QDebug>


CNavTagView::CNavTagView(QWidget *parent) : QTreeView(parent)
{
}

void CNavTagView::dropEvent(QDropEvent *event)
{
    if (event->keyboardModifiers() & Qt::ControlModifier)
        event->setDropAction(Qt::CopyAction);
    else
        event->setDropAction(Qt::MoveAction);

    QTreeView::dropEvent(event);
}
