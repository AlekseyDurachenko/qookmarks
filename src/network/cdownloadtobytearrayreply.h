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
#ifndef CDOWNLOADTOBYTEARRAYREPLY_H
#define CDOWNLOADTOBYTEARRAYREPLY_H

#include "cabstractdownloadreply.h"
#include "cdownloadtobytearrayrequest.h"


class CDownloadToByteArrayReply : public CAbstractDownloadReply
{
    Q_OBJECT
public:
    CDownloadToByteArrayReply(const CDownloadToByteArrayRequest &request,
                              QNetworkAccessManager *network,
                              QObject *parent = 0);

    inline const CDownloadToByteArrayRequest &request() const;
    inline const QByteArray &data() const;
protected:
    virtual void processEnd();
private:
    CDownloadToByteArrayRequest m_request;
    QByteArray m_data;
};

const CDownloadToByteArrayRequest &CDownloadToByteArrayReply::request() const
{
    return m_request;
}

const QByteArray &CDownloadToByteArrayReply::data() const
{
    return m_data;
}


#endif // CDOWNLOADTOBYTEARRAYREPLY_H
