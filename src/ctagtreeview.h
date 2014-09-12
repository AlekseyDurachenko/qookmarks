// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef CTAGTREEVIEW_H
#define CTAGTREEVIEW_H

#include "ctagitemmodel.h"
#include "cbookmarkmgr.h"
#include <QTreeView>

class CTagTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CTagTreeView(QWidget *parent = 0);

    inline CBookmarkMgr *mgr() const;
    void setBookmarkMgr(CBookmarkMgr *mgr);
signals:
    void currentTagChanged(CTagItem *tag);
    void currentTagChanged(const QSet<CTagItem *> &tags);
public slots:
private slots:
    void onMgrDestroyed();
protected:
    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous);
private:
    CBookmarkMgr *m_mgr;
    CTagItemModel *m_model;
};

CBookmarkMgr *CTagTreeView::mgr() const
{
    return m_mgr;
}


#endif // CTAGTREEVIEW_H
