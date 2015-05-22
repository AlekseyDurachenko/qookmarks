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
#ifndef DOWNLOADTOBYTEARRAYREPLY_H
#define DOWNLOADTOBYTEARRAYREPLY_H

#include "abstractdownloadreply.h"
#include "downloadtobytearrayrequest.h"


class DownloadToByteArrayReply : public AbstractDownloadReply
{
    Q_OBJECT
public:
    DownloadToByteArrayReply(const DownloadToByteArrayRequest &request,
                             QNetworkAccessManager *network,
                             QObject *parent = 0);

    inline const DownloadToByteArrayRequest &request() const;
    inline const QByteArray &data() const;
protected:
    virtual void processEnd();
private:
    DownloadToByteArrayRequest m_request;
    QByteArray m_data;
};

const DownloadToByteArrayRequest &DownloadToByteArrayReply::request() const
{
    return m_request;
}

const QByteArray &DownloadToByteArrayReply::data() const
{
    return m_data;
}


#endif // DOWNLOADTOBYTEARRAYREPLY_H
