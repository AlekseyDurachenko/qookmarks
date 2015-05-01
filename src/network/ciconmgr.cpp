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
#include "ciconmgr.h"


CIconMgr::CIconMgr(QObject *parent) : QObject(parent)
{
}

CIconMgr::~CIconMgr()
{
}

void CIconMgr::init(const QString &iconPath, const QList<QUrl> &urls)
{

}

void CIconMgr::deinit()
{

}

const QIcon &CIconMgr::icon(const QUrl &url)
{

}

void CIconMgr::downloadIcon(const QUrl &url)
{

}

