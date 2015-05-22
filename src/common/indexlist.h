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
#ifndef INDEXLIST_H
#define INDEXLIST_H

#include <QList>
#include <QHash>


template<typename T>
class IndexList
{
public:
    IndexList();

    inline int count() const;
    inline T *at(int index) const;
    inline T *value(int index) const;
    inline int indexOf(const T *value) const;
    void push_back(T *value);
    void push_front(T *value);
    void removeAt(int index);
    void removeFirst();
    void removeLast();
    void clear();
private:
    QList<T *> m_items;
    QHash<const T *, int> m_indexes;
};


template<typename T>
IndexList<T>::IndexList()
{
}

template<typename T>
int IndexList<T>::count() const
{
    return m_items.count();
}

template<typename T>
T *IndexList<T>::at(int index) const
{
    return m_items.at(index);
}

template<typename T>
T *IndexList<T>::value(int index) const
{
    return m_items.value(index);
}

template<typename T>
int IndexList<T>::indexOf(const T *value) const
{
    return m_indexes.value(value, -1);
}

template<typename T>
void IndexList<T>::push_back(T *value)
{
    m_indexes.insert(value, m_items.count());
    m_items.push_back(value);
}

template<typename T>
void IndexList<T>::push_front(T *value)
{
    typename QHash<const T *, int>::iterator i;
    for (i = m_indexes.begin(); i != m_indexes.end(); ++i)
        i.value() += 1;

    m_items.push_front(value);
    m_indexes.insert(value, 0);
}

template<typename T>
void IndexList<T>::removeAt(int index)
{
    m_indexes.remove(m_items.at(index));
    m_items.removeAt(index);
    for (int i = index; i < m_items.count(); ++i)
        m_indexes[m_items.at(i)] -= 1;
}

template<typename T>
void IndexList<T>::removeFirst()
{
    m_indexes.remove(m_items.first());
    m_items.removeFirst();

    typename QHash<const T *, int>::iterator i;
    for (i = m_indexes.begin(); i != m_indexes.end(); ++i)
        i.value() -= 1;
}

template<typename T>
void IndexList<T>::removeLast()
{
    m_indexes.remove(m_items.last());
    m_items.removeLast();
}

template<typename T>
void IndexList<T>::clear()
{
    m_items.clear();
    m_indexes.clear();
}


#endif // INDEXLIST_H
