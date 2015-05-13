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
#include "browser.h"
#include <QDesktopServices>
#include <QStringList>
#include <QProcess>
#include <QDebug>


QList<QByteArray> Browser::m_systemBrowsers;
QHash<QByteArray, QString> Browser::m_systemBrowserNames;
QHash<QByteArray, int> Browser::m_systemBrowserOptions;
QHash<QByteArray, QByteArray> Browser::m_systemBrowserExecutable;
QHash<QByteArray, QByteArray> Browser::m_systemBrowserRegularArgs;
QHash<QByteArray, QByteArray> Browser::m_systemBrowserWindowArgs;
QHash<QByteArray, QByteArray> Browser::m_systemBrowserPrivateArgs;


static bool testBrowser(const QByteArray &command, QByteArray *exePath = 0)
{
    QProcess process;
    process.start("which", (QStringList() << command), QIODevice::ReadOnly);

    if (!process.waitForStarted(-1))
        return false;

    QByteArray output;
    while(process.waitForReadyRead(-1))
        output += process.readAllStandardOutput();

    if (exePath)
        *exePath = output.trimmed();

    return (process.exitCode() == 0);
}

void Browser::init()
{
    registerSystemBrowser("chromium-browser", QObject::tr("Chromium"), SupportAll);
    registerRegularArgs("chromium-browser", "");
    registerWindowArgs("chromium-browser", "--new-window");
    registerPrivateArgs("chromium-browser", "--incognito");
    registerSystemBrowser("firefox", QObject::tr("Firefox"), SupportMultipleWindow|SupportPrivateWindow);
    registerRegularArgs("firefox", "");
    registerWindowArgs("firefox", "--new-window");
    registerPrivateArgs("firefox", "--private-window");
}

bool Browser::openUrl(const QUrl &url)
{
    return QDesktopServices::openUrl(url);
}

bool Browser::openUrl(const QByteArray &browser, const QList<QUrl> &urls,
        int flags)
{
    if (!m_systemBrowsers.contains(browser)
            || (((m_systemBrowserOptions[browser] & SupportMultipleUrl) == false)
                && urls.count() > 1)
            || (((m_systemBrowserOptions[browser] & SupportMultipleWindow) == false)
                && flags & Window)
            || (((m_systemBrowserOptions[browser] & SupportPrivateWindow) == false)
                && flags & Private))
        return false;

    QStringList args;
    if (flags & Window)
        args.push_back(m_systemBrowserWindowArgs[browser]);
    if (flags & Private)
        args.push_back(m_systemBrowserPrivateArgs[browser]);
    foreach (const QUrl &url, urls)
        args.push_back(url.toString());

    return QProcess::startDetached(m_systemBrowserExecutable[browser], args);
}

const QList<QByteArray> &Browser::systemBrowsers()
{
    return m_systemBrowsers;
}

const QString &Browser::systemBrowserName(const QByteArray &browser)
{
    return m_systemBrowserNames[browser];
}

int Browser::systemBrowserOptions(const QByteArray &browser)
{
    return m_systemBrowserOptions[browser];
}

void Browser::registerSystemBrowser(const QByteArray &browser,
        const QString &name, int options)
{
    QByteArray path;
    if (!testBrowser(browser, &path))
        return;

    m_systemBrowsers.push_back(browser);
    m_systemBrowserNames.insert(browser, name);
    m_systemBrowserOptions.insert(browser, options);
    m_systemBrowserExecutable.insert(browser, path);
}

void Browser::registerRegularArgs(const QByteArray &browser,
                                        const QByteArray &args)
{
    m_systemBrowserRegularArgs[browser] = args;
}

void Browser::registerWindowArgs(const QByteArray &browser,
                                 const QByteArray &args)
{
    m_systemBrowserWindowArgs[browser] = args;
}

void Browser::registerPrivateArgs(const QByteArray &browser,
                                  const QByteArray &args)
{
    m_systemBrowserPrivateArgs[browser] = args;
}
