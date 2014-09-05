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
#include "cbookmarkmgr.h"

CBookmarkMgr::CBookmarkMgr(QObject *parent) :
    QObject(parent)
{
    m_bookmarkRoot = new CBookmarkItem(-1, "", this, 0);
    m_tagRoot = new CTagItem(-1, CTagItem::RootItem, "", 0, 0);

    m_tagRoot->add(new CTagItem(100, CTagItem::Tag, "tag100", 0, m_tagRoot));
    m_tagRoot->add(new CTagItem(200, CTagItem::Tag, "tag200", 0, m_tagRoot));
    m_tagRoot->add(new CTagItem(300, CTagItem::Tag, "tag300", 0, m_tagRoot));

    m_bookmarkRoot->add(new CBookmarkItem(400, "bookmark400", this, m_bookmarkRoot));
    m_bookmarkRoot->add(new CBookmarkItem(500, "bookmark500", this, m_bookmarkRoot));

    m_bookmarkRoot->childAt(1)->tagAdd(m_tagRoot->childAt(0));
    m_bookmarkRoot->childAt(1)->tagAdd(m_tagRoot->childAt(1));
    m_bookmarkRoot->childAt(1)->tagAdd(m_tagRoot->childAt(2));

    //m_bookmarkRoot->childAt(1)->tagRemove(m_tagRoot->child(0));
    //m_tagRoot->removeAt(1);
    delete m_tagRoot;
}

CBookmarkMgr::~CBookmarkMgr()
{
    delete m_bookmarkRoot;
}
