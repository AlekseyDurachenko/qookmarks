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
#ifndef CTAGITEM_H
#define CTAGITEM_H

#include <QObject>

class CTagItem : public QObject
{
    Q_OBJECT
public:
    enum Type {Normal = 0, Unlabeled }
    explicit CTagItem(Type type, int row, QObject *parent = 0);

    inline Type type() const;
    inline int row() const;

    inline int columnCount() const;
    inline int childCount() const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(int column, int role) const;
    CTagItem *child(int row);
    void add(CTagItem *item);

    inline const QString &tagName() const;
    void setTagName(const QString &tagName);
signals:
    void inserted(CTagItem *parent, int first, int last);
    void removed(CTagItem *parent, int first, int last);
    void changed(CTagItem *parent, int first, int last);
private:
    Type m_type;
    int m_row;
    QList<CTagItem *> m_childList;
    QString m_tagName;
};

CTagItem::Type CTagItem::type() const
{
    return m_type;
}

int CTagItem::row() const
{
    return m_row;
}

int CTagItem::columnCount() const
{
    return 1;
}

int CTagItem::childCount() const
{
    return m_childList.count();
}

const QString &CTagItem::tagName() const
{
    return m_tagName;
}

#endif // CTAGITEM_H
