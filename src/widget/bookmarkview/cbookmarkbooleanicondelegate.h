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
#ifndef CBOOKMARKBOOLEANICONDELEGATE_H
#define CBOOKMARKBOOLEANICONDELEGATE_H

#include <QItemDelegate>


class CBookmarkBooleanIconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CBookmarkBooleanIconDelegate(QObject *parent = 0);

    inline const QIcon &iconOn() const;
    void setIconOn(const QIcon &icon);

    inline const QIcon &iconOff() const;
    void setIconOff(const QIcon &icon);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
private:
    QIcon m_iconOn;
    QIcon m_iconOff;
};

const QIcon &CBookmarkBooleanIconDelegate::iconOn() const
{
    return m_iconOn;
}

const QIcon &CBookmarkBooleanIconDelegate::iconOff() const
{
    return m_iconOff;
}


#endif // CBOOKMARKBOOLEANICONDELEGATE_H
