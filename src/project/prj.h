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
#ifndef PRJ_H
#define PRJ_H

#include <QObject>
#include <QAction>
class IconMgr;
class Manager;


class Prj : public QObject
{
    Q_OBJECT
public:
    explicit Prj(QObject *parent = 0);
    virtual ~Prj();

    inline Manager *manager() const;
    inline IconMgr *iconMgr() const;

    inline bool isOpen() const;
    inline bool hasChanges() const;

    bool create(const QString &path, QString *reason = 0);
    bool open(const QString &path, QString *reason = 0);
    bool save(QString *reason = 0);
    void close();

    inline QString path() const;
    inline QString xmlPath() const;
    inline QString iconPath() const;
    inline QString screenshotPath() const;
    inline QString downloadsPath() const;

    inline QAction *createAction() const;
    inline QAction *openAction() const;
    inline QAction *saveAction() const;
    inline QAction *closeAction() const;
public:
    static QString xmlPath(const QString &path);
    static QString iconPath(const QString &path);
    static QString screenshotPath(const QString &path);
    static QString downloadsPath(const QString &path);
signals:
    void opened();
    void closed();
private slots:
    void somethingChanged();
private:
    void updateActions();
private:
    Manager *m_manager;
    IconMgr *m_iconMgr;
    QAction *m_createAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_closeAction;
    QString m_path;
    bool m_hasChanges;
};

Manager *Prj::manager() const
{
    return m_manager;
}

IconMgr *Prj::iconMgr() const
{
    return m_iconMgr;
}

bool Prj::isOpen() const
{
    return (!m_path.isEmpty());
}

bool Prj::hasChanges() const
{
    return m_hasChanges;
}

QString Prj::path() const
{
    return m_path;
}

QString Prj::xmlPath() const
{
    return xmlPath(m_path);
}

QString Prj::iconPath() const
{
    return iconPath(m_path);
}

QString Prj::screenshotPath() const
{
    return screenshotPath(m_path);
}

QString Prj::downloadsPath() const
{
    return downloadsPath(m_path);
}

QAction *Prj::createAction() const
{
    return m_createAction;
}

QAction *Prj::openAction() const
{
    return m_openAction;
}

QAction *Prj::saveAction() const
{
    return m_saveAction;
}

QAction *Prj::closeAction() const
{
    return m_closeAction;
}


#endif // PRJ_H
