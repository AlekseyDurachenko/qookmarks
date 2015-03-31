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
#ifndef CPRJ_H
#define CPRJ_H

#include <QObject>
class CManager;


class CPrj : public QObject
{
    Q_OBJECT
public:
    explicit CPrj(QObject *parent = 0);
    virtual ~CPrj();

    inline CManager *manager() const;
    inline bool isOpen() const;

    bool create(const QString &path, QString *reason = 0);
    bool open(const QString &path, QString *reason = 0);
    bool save(QString *reason = 0);
    void close();

    inline QString xmlPath() const;
    inline QString iconPath() const;
    inline QString screenshotPath() const;
    inline QString downloadsPath() const;
signals:
public slots:
private:
    static QString xmlPath(const QString &path);
    static QString iconPath(const QString &path);
    static QString screenshotPath(const QString &path);
    static QString downloadsPath(const QString &path);
private:
    CManager *m_manager;
    QString m_path;
};

CManager *CPrj::manager() const
{
    return m_manager;
}

bool CPrj::isOpen() const
{
    return (!m_path.isEmpty());
}

QString CPrj::xmlPath() const
{
    return xmlPath(m_path);
}

QString CPrj::iconPath() const
{
    return iconPath(m_path);
}

QString CPrj::screenshotPath() const
{
    return screenshotPath(m_path);
}

QString CPrj::downloadsPath() const
{
    return downloadsPath(m_path);
}


#endif // CPRJ_H
