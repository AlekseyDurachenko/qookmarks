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
#include <QProcess>


QList<QByteArray> Browser::m_systemBrowsers;
QHash<QByteArray, QByteArray> Browser::m_systemBrowserExec;


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
    QByteArray execPath;
    if (testBrowser("chromium-browser", &execPath))
    {
        m_systemBrowsers.push_back("chromium-browser");
        m_systemBrowserExec.insert("chromium-browser", execPath);
    }

    if (testBrowser("firefox", &execPath))
    {
        m_systemBrowsers.push_back("firefox");
        m_systemBrowserExec.insert("firefox", execPath);
    }
}

bool Browser::openUrl(const QUrl &url)
{
    return QDesktopServices::openUrl(url);
}

bool Browser::openUrl(const QByteArray &browser, const QList<QUrl> &urls,
        WindowType windowType, QString *reason)
{
    try
    {
        if (urls.count() > 1
                && !canOpenMultipleUrls(browser))
            throw QObject::tr("%1 can open only one url")
                .arg(browserName(browser));

        if (windowType == CurrentWindow
                && !canOpenCurrentWindow(browser))
            throw QObject::tr("%1 cannot open url in current window")
                .arg(browserName(browser));

        if (windowType == NewWindow
                && !canOpenNewWindow(browser))
            throw QObject::tr("%1 cannot open url in new window")
                .arg(browserName(browser));

        if (windowType == NewPrivateWindow
                && !canOpenNewPrivateWindow(browser))
            throw QObject::tr("%1 cannot open url in new private window")
                .arg(browserName(browser));

        QString command;
        QStringList args;
        if (browser == "chromium-browser")
        {
            command = m_systemBrowserExec["chromium-browser"];
            switch (windowType)
            {
            case CurrentWindow:
                break;
            case NewWindow:
                args.push_back("--new-window");
                break;
            case NewPrivateWindow:
                args.push_back("--new-window");
                args.push_back("--incognito");
                break;
            }

            foreach (const QUrl &url, urls)
                args.push_back(url.toString());
        }
        else if (browser == "firefox")
        {
            command = m_systemBrowserExec["firefox"];
            switch (windowType)
            {
            case CurrentWindow:
                break;
            case NewWindow:
                args.push_back("--new-window");
                break;
            case NewPrivateWindow:
                args.push_back("--private-window");
                break;
            }

            foreach (const QUrl &url, urls)
                args.push_back(url.toString());
        }

        return QProcess::startDetached(command, args);
    }
    catch (const QString &error)
    {
        if (reason)
            *reason = error;
    }

    return false;
}

const QList<QByteArray> &Browser::browsers()
{
    return m_systemBrowsers;
}

QString Browser::browserName(const QByteArray &browser)
{
    if (browser == "chromium-browser")
        return QObject::tr("Chromium");
    if (browser == "firefox")
        return QObject::tr("Firefox");

    return QObject::tr("Unknow");
}

bool Browser::canOpenMultipleUrls(const QByteArray &browser)
{
    if (browser == "chromium-browser")
        return true;
    if (browser == "firefox")
        return false;

    return false;
}

bool Browser::canOpenCurrentWindow(const QByteArray &browser)
{
    if (browser == "chromium-browser")
        return true;
    if (browser == "firefox")
        return true;

    return false;
}

bool Browser::canOpenNewWindow(const QByteArray &browser)
{
    if (browser == "chromium-browser")
        return true;
    if (browser == "firefox")
        return true;

    return false;
}

bool Browser::canOpenNewPrivateWindow(const QByteArray &browser)
{
    if (browser == "chromium-browser")
        return true;
    if (browser == "firefox")
        return true;

    return false;
}
