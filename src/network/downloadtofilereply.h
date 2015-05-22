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
#ifndef DOWNLOADTOFILEREPLY_H
#define DOWNLOADTOFILEREPLY_H

#include "abstractdownloadreply.h"
#include "downloadtofilerequest.h"
#include <QFile>


class DownloadToFileReply : public AbstractDownloadReply
{
public:
    DownloadToFileReply(const DownloadToFileRequest &request,
                        QNetworkAccessManager *network,
                        QObject *parent = 0);

    inline const DownloadToFileRequest &request() const;
protected:
    virtual void processStart();
    virtual void processData();
    virtual void processEnd();
private:
    DownloadToFileRequest m_request;
    QFile m_file;
    bool m_isFileOpened;
};


#endif // DOWNLOADTOFILEREPLY_H
