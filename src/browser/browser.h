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
#ifndef BROWSER_H
#define BROWSER_H

#include <QUrl>


class Browser
{
public:
    enum WindowType {
        CurrentWindow       = 0x0000,
        NewWindow           = 0x0001,
        NewPrivateWindow    = 0x0002
    };

    static void init();

    static const QList<QByteArray> &browsers();
    static QString browserName(const QByteArray &browser);

    static bool openUrl(const QUrl &url);
    static bool openUrl(const QByteArray &browser, WindowType windowType,
                        const QList<QUrl> &urls, QString *reason = 0);

    static bool canOpenMultipleUrls(const QByteArray &browser);
    static bool canOpenCurrentWindow(const QByteArray &browser);
    static bool canOpenNewWindow(const QByteArray &browser);
    static bool canOpenNewPrivateWindow(const QByteArray &browser);
private:
    static QList<QByteArray> m_systemBrowsers;
    static QHash<QByteArray, QByteArray> m_systemBrowserExec;
};


#endif // BROWSER_H
