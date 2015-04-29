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
#ifndef CDOWNLOADTOFILEREPLY_H
#define CDOWNLOADTOFILEREPLY_H

#include <QFile>
#include "cabstractdownloadreply.h"
#include "cdownloadtofilerequest.h"


class CDownloadToFileReply : public CAbstractDownloadReply
{
public:
    CDownloadToFileReply(const CDownloadToFileRequest &request,
                         QNetworkAccessManager *network,
                         QObject *parent = 0);

    inline const CDownloadToFileRequest &request() const;
protected:
    virtual void processStart();
    virtual void processData();
    virtual void processEnd();
private:
    CDownloadToFileRequest m_request;
    QFile m_file;
    bool m_isFileOpened;
};


#endif // CDOWNLOADTOFILEREPLY_H
