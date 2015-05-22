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
#ifndef DOWNLOADWEBPAGEINFOREPLY_H
#define DOWNLOADWEBPAGEINFOREPLY_H

#include "abstractdownloadreply.h"
#include "downloadwebpageinforequest.h"
#include <libxml2/libxml/tree.h>
#include <QIcon>


class DownloadWebPageInfoReply : public AbstractDownloadReply
{
    Q_OBJECT
public:
    DownloadWebPageInfoReply(const DownloadWebPageInfoRequest &request,
                              QNetworkAccessManager *network,
                              QObject *parent = 0);

    inline const DownloadWebPageInfoRequest &request() const;
    inline const QString &title() const;
    inline const QString &keywords() const;
    inline const QString &description() const;
protected:
    virtual void processEnd();
private:
    void parseHtml(xmlNode *root, const xmlChar *prev);
private:
    DownloadWebPageInfoRequest m_request;
    QString m_title;
    QString m_keywords;
    QString m_description;
};

const DownloadWebPageInfoRequest &DownloadWebPageInfoReply::request() const
{
    return m_request;
}

const QString &DownloadWebPageInfoReply::title() const
{
    return m_title;
}

const QString &DownloadWebPageInfoReply::keywords() const
{
    return m_keywords;
}

const QString &DownloadWebPageInfoReply::description() const
{
    return m_description;
}


#endif // DOWNLOADWEBPAGEINFOREPLY_H
