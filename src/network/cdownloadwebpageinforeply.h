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
#ifndef CDOWNLOADWEBPAGEINFOREPLY_H
#define CDOWNLOADWEBPAGEINFOREPLY_H

#include <QIcon>
#include <libxml2/libxml/tree.h>
#include "cabstractdownloadreply.h"
#include "cdownloadwebpageinforequest.h"


class CDownloadWebPageInfoReply : public CAbstractDownloadReply
{
    Q_OBJECT
public:
    CDownloadWebPageInfoReply(const CDownloadWebPageInfoRequest &request,
                              QNetworkAccessManager *network,
                              QObject *parent = 0);

    inline const CDownloadWebPageInfoRequest &request() const;
    inline const QString &title() const;
    inline const QString &keywords() const;
    inline const QString &description() const;
protected:
    virtual void processEnd();
private:
    void parseHtml(xmlNode *root, const xmlChar *prev);
private:
    CDownloadWebPageInfoRequest m_request;
    QString m_title;
    QString m_keywords;
    QString m_description;
};

const CDownloadWebPageInfoRequest &CDownloadWebPageInfoReply::request() const
{
    return m_request;
}

const QString &CDownloadWebPageInfoReply::title() const
{
    return m_title;
}

const QString &CDownloadWebPageInfoReply::keywords() const
{
    return m_keywords;
}

const QString &CDownloadWebPageInfoReply::description() const
{
    return m_description;
}


#endif // CDOWNLOADWEBPAGEINFOREPLY_H
