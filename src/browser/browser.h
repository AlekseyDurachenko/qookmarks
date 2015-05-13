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
    enum Options {
        NoOptions               = 0x0000,
        SupportMultipleUrl      = 0x0001,
        SupportMultipleWindow   = 0x0002,
        SupportPrivateWindow    = 0x0004,
        SupportAll      = SupportMultipleUrl
                        | SupportMultipleWindow
                        | SupportPrivateWindow
    };

    enum Flags {
        NoFlags = 0x0000,
        Window  = 0x0001,
        Private = 0x0002
    };

    static void init();
    static bool openUrl(const QUrl &url);
    static bool openUrl(const QByteArray &browser, const QList<QUrl> &urls,
                        int flags = NoFlags);
    static const QList<QByteArray> &systemBrowsers();
    static const QString &systemBrowserName(const QByteArray &browser);
    static int systemBrowserOptions(const QByteArray &browser);
private:
    static void registerSystemBrowser(const QByteArray &browser,
                                      const QString &name,
                                      int options = NoOptions);
    static void registerRegularArgs(const QByteArray &browser,
                                    const QByteArray &args);
    static void registerWindowArgs(const QByteArray &browser,
                                   const QByteArray &args);
    static void registerPrivateArgs(const QByteArray &browser,
                                    const QByteArray &args);
private:
    static QList<QByteArray> m_systemBrowsers;
    static QHash<QByteArray, QString> m_systemBrowserNames;
    static QHash<QByteArray, int> m_systemBrowserOptions;
    static QHash<QByteArray, QByteArray> m_systemBrowserExecutable;
    static QHash<QByteArray, QByteArray> m_systemBrowserRegularArgs;
    static QHash<QByteArray, QByteArray> m_systemBrowserWindowArgs;
    static QHash<QByteArray, QByteArray> m_systemBrowserPrivateArgs;
};


#endif // BROWSER_H
