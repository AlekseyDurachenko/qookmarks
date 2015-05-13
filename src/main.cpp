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
#include <QApplication>
#include "cmainwindow.h"
#include "singleton.h"
#include "cprj.h"
#include "browser.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Browser::init();
    qDebug() << Browser::browsers();

    // create singletons
    singleton<CNetworkMgr>();
    singleton<CPrj>();

    CMainWindow w;
    w.show();
    int ret = a.exec();

    // destroy singletons
    //delete singleton<CPrj>();
    //delete singleton<CNetworkMgr>();

    return ret;
}
