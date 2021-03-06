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
#include "downloadtofilereply.h"
#include <QDebug>


DownloadToFileReply::DownloadToFileReply(
        const DownloadToFileRequest &request, QNetworkAccessManager *network,
        QObject *parent) : AbstractDownloadReply(network, parent)
{
    m_request = request;
    m_isFileOpened = false;
    fetchUrl(request.url(),
             request.maxRetryCount(),
             request.maxRedirectCount(),
             true);
}

void DownloadToFileReply::processStart()
{
    m_file.setFileName(m_request.fileName());
    if (!m_file.open(QIODevice::WriteOnly))
    {
        setError(FileError, m_file.errorString());
        abort();
    }
    else
    {
        m_isFileOpened = true;
    }
}

void DownloadToFileReply::processData()
{
    QByteArray ba = readAll();
    if (m_file.write(ba) != ba.size())
    {
        setError(FileError, m_file.errorString());
        abort();
    }
}

void DownloadToFileReply::processEnd()
{
    if (!m_isFileOpened)
        return;

     m_file.close();

     if (error() == NoError)
         return;

     if (QFile::remove(m_request.fileName()))
        qWarning() << tr("can't remove the file")
                   << m_request.fileName();
}
