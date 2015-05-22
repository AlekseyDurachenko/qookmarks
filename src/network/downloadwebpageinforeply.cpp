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
#include "downloadwebpageinforeply.h"
#include <QTextCodec>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/HTMLparser.h>
#include <QDebug>


DownloadWebPageInfoReply::DownloadWebPageInfoReply(
        const DownloadWebPageInfoRequest &request,
        QNetworkAccessManager *network, QObject *parent) :
    AbstractDownloadReply(network, parent)
{
    m_request = request;
    fetchUrl(request.url(),
             request.maxRetryCount(),
             request.maxRedirectCount(),
             true);
}

void DownloadWebPageInfoReply::processEnd()
{
    QByteArray srcData = readAll();

    xmlDoc* doc = htmlReadDoc((const xmlChar *)srcData.constData(), NULL, charset().data(),
                    HTML_PARSE_RECOVER|HTML_PARSE_NOERROR|HTML_PARSE_NOWARNING);

    xmlNode *root = xmlDocGetRootElement(doc);
    parseHtml(root, 0);

    xmlFreeDoc(doc);
}

void DownloadWebPageInfoReply::parseHtml(xmlNode *root, const xmlChar *prev)
{
    for (xmlNode *cur_node = root; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if (!xmlStrcmp(cur_node->name, (const xmlChar *)"html")
                    && prev == 0)
                parseHtml(cur_node->children, cur_node->name);
            else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"head")
                    && !xmlStrcmp(prev, (const xmlChar *)"html"))
                parseHtml(cur_node->children, cur_node->name);
            else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"meta")
                    && !xmlStrcmp(prev, (const xmlChar *)"head"))
            {
                xmlChar *propName = xmlGetProp(cur_node, (const xmlChar *)"name");
                if (propName)
                {
                    xmlChar *propContent = xmlGetProp(cur_node, (const xmlChar *)"content");
                    if (propContent)
                    {
                        if (QString((char*)propName).toLower() == "description")
                            m_description = QString::fromUtf8((const char*)propContent).trimmed();
                        if (QString((char*)propName).toLower() == "keywords")
                            m_keywords = QString::fromUtf8((const char*)propContent).trimmed();
                    }
                }
            }
            else if (!xmlStrcmp(cur_node->name, (const xmlChar *)"title")
                    && !xmlStrcmp(prev, (const xmlChar *)"head"))
            {
                m_title = QString::fromUtf8((const char*)xmlNodeGetContent(cur_node)).trimmed();
            }
        }
    }
}
