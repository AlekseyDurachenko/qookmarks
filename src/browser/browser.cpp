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


static bool findBrowser(const QByteArray &command, QByteArray *exePath = 0)
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
    if (findBrowser("chromium-browser", &execPath))
    {
        m_systemBrowsers.push_back("chromium-browser");
        m_systemBrowserExec.insert("chromium-browser", execPath);
    }

    if (findBrowser("firefox", &execPath))
    {
        m_systemBrowsers.push_back("firefox");
        m_systemBrowserExec.insert("firefox", execPath);
    }
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

bool Browser::openUrl(const QUrl &url)
{
    return QDesktopServices::openUrl(url);
}

bool Browser::openUrl(const QByteArray &browser, WindowType windowType,
        const QList<QUrl> &urls, QString *reason)
{
    if (!canOpenUrl(browser, windowType, urls.count(), reason))
        return false;

    if (browser == "chromium-browser")
    {
        QString command = m_systemBrowserExec["chromium-browser"];
        QStringList args;
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

        return QProcess::startDetached(command, args);
    }
    else if (browser == "firefox")
    {
        QString command = m_systemBrowserExec["firefox"];
        QStringList args;
        switch (windowType)
        {
        case CurrentWindow:
            foreach (const QUrl &url, urls)
            {
                args.push_back("-new-tab");
                args.push_back(url.toString());
            }
            break;
        case NewWindow:
            if (urls.count() == 1)
            {
                args.push_back("--new-window");
            }
            foreach (const QUrl &url, urls)
            {
                args.push_back(url.toString());
            }
            break;
        case NewPrivateWindow:
            foreach (const QUrl &url, urls)
            {
                args.clear();
                args.push_back("--private-window");
                args.push_back(url.toString());
            }
            break;
        }

        return QProcess::startDetached(command, args);
    }

    return false;
}

bool Browser::canOpenUrl(const QByteArray &browser,
        Browser::WindowType windowType, int urlCount, QString *reason)
{
    try
    {
        if (urlCount < 1)
            throw QObject::tr("Url list is empty");

        if (browser == "chromium-browser")
        {
            switch (windowType)
            {
            case CurrentWindow:
            case NewWindow:
            case NewPrivateWindow:
                return true;
            }
        }

        if (browser == "firefox")
        {
            switch (windowType)
            {
            case CurrentWindow:
            case NewWindow:
                return true;
            case NewPrivateWindow:
                if (urlCount > 1)
                    throw QObject::tr("Firefox can't open more than one url");
                return true;
            }
        }
    }
    catch (const QString &error)
    {
        if (reason)
            *reason = error;
    }

    return false;
}

bool Browser::hasWindowType(const QByteArray &browser,
        Browser::WindowType windowType)
{
    if (browser == "chromium-browser")
    {
        switch (windowType)
        {
        case CurrentWindow:
        case NewWindow:
        case NewPrivateWindow:
            return true;
        }
    }

    if (browser == "firefox")
    {
        switch (windowType)
        {
        case CurrentWindow:
        case NewWindow:
        case NewPrivateWindow:
            return true;
        }
    }

    return false;
}
