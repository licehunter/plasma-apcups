/**************************************************************************
*   Copyright (C) 2010 by Diego Berge <devel@navlost.eu>                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, see <http://www.gnu.org/licenses/>   *
***************************************************************************/

#include <stdio.h>
#include <QtEndian>
#include "apcupscif.h"
#include "apcupsc.h"

ApcUpsMon::ApcUpsMon(QString host, quint16 port, int updint, QObject *parent)
: QObject(parent)
{
    bytesLeft = 0;
    lastResponseTimestamp = 0;
    connect(&socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
    connect(&socket, SIGNAL(connected()), this, SLOT(timeout()));
    connect(&socket, SIGNAL(connected()), &timer, SLOT(start()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(timeout()));
    connect(&socket, SIGNAL(disconnected()), &timer, SLOT(stop()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
            connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
            
            connectToHost(host, port);
            setInterval(updint);
            
            // Launch D-BUS
            new ApcUpsMonAdaptor(this);
            QDBusConnection dbus = QDBusConnection::sessionBus();
            dbus.registerObject("/", this);
            dbus.registerService("eu.navlost.apcupsmon");
}

bool ApcUpsMon::hasError() const
{
    return !m_errorString.isEmpty();
}

QString ApcUpsMon::errorString() const
{
    return m_errorString;
}

void ApcUpsMon::connectToHost(const QString &h, unsigned int p)
{
    if (p == 0)
        p = 3551;
    socket.disconnectFromHost();
    host = h;
    port = p;
    socket.connectToHost(host, port);
}

void ApcUpsMon::connectToHost()
{
    socket.connectToHost(host, port);
}

void ApcUpsMon::stopUpdates()
{
    socket.disconnectFromHost();
}

void ApcUpsMon::setInterval(int updint)
{
    timer.setInterval(updint*1000);
    expiryLimit = updint * 10;
}

void ApcUpsMon::socketRead()
{
    // Each line is of the form <bytes><text>, where
    // <bytes> is a 16 bit integer in network order,
    // and <text> are <bytes> characters of the form
    // KEY : VALUE.
    // The response ends with a line of the form
    // \0x00\0x00 (i.e., a line with zero data length)
    // The complete response may take more than one
    // socketRead() call, so we keep track of how
    // many bytes we have left to read from a previous
    // call, and only consider a response complete
    // upon receiving the '\0x00\0x00' line, at which
    // point we call processResponse() for the high-level
    // processing.
    char chunkSize[2];
    qint16 size;
    qint64 bytesRead;
    char line[1024];
    // Read any bytes left over from a previous socketRead()
    // call
    while (socket.bytesAvailable() && bytesLeft > 0) {
        bytesRead = socket.read(line, bytesLeft);
        bytesLeft -= bytesRead;
        if (bytesRead > 0) {
            line[bytesRead] = '\0';
            response.append(line);
        }
    }
    // if any bytes left
    while (socket.bytesAvailable()) {
        // Get this line's length
        if (socket.read(chunkSize, 2) == 2) {
            //             size = qFromBigEndian((qint16)(*chunkSize));
            // FIXME - This will bomb on big-endian machines,
            // I need to get qFromBigEndian() working!!!
            *(&size) = chunkSize[1];
            *(&size+1) = chunkSize[0];
            // Check if we are at the end of the response
            if (size > 0) {
                if (size >= 1024) {
                    // FIXME - Ideally would need to resize the
                    // 'line' variable, however the current apcupsd
                    // protocol does not send responses anywhere near
                    // 1 kB so at the moment that's an over optimisation
                    //                     qDebug() << "Bad news, hit a huge line, which I'll ignore";
                    QByteArray sink = socket.read(size);
                    return;
                }
                bytesRead = socket.read(line, size);
                if (bytesRead < size) {
                    // We have an incomplete line, set bytesLeft
                    // so we take care of it on the next call
                    bytesLeft = size - bytesRead;
                }
                if (bytesRead > 0) {
                    // Append this line to the rest of the response
                    line[bytesRead] = '\0';
                    response.append(line);
                }
            } else { // FIXME - this assumes that size == 0
                // We have a complete response, now process it
                processResponse();
            }
        } else {
            m_errorString = "Unable to read from socket";
        }
    }
}

// Have a socket error, set m_errorString and propagate
// signal
void ApcUpsMon::socketError(QAbstractSocket::SocketError)
{
    m_errorString = socket.errorString();
    if (socket.state() != QAbstractSocket::ConnectedState) {
        // Attempt to reconnect in two timer intervals or,
        // failing that, ten seconds
        QTimer::singleShot((timer.interval() ? timer.interval() : 10000),
                           this, SLOT(connectToHost()));
    }
    
    emit haveError(m_errorString);
}

void ApcUpsMon::timeout()
{
    if (lastResponseTimestamp != 0 && (time(NULL) - lastResponseTimestamp) > expiryLimit) {
        // If we haven't received a response in a while, close the socket.
        // If there is a problem, this should generate diagnostics when
        // attempting to reconnect.
        lastResponseTimestamp = 0; // Otherwise we go in an infinite loop
        socket.abort();
    }
    requestStatus();
}

void ApcUpsMon::requestStatus()
{
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.putChar('\0');
        socket.putChar('\x06');
        socket.write("status");
    } else {
        // We're not connected, try and reconnect?
        connectToHost();
    }
}

void ApcUpsMon::requestEvents()
{
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.putChar('\0');
        socket.putChar('\x06');
        socket.write("events");
    } else {
        // We're not connected, try and reconnect?
        connectToHost();
    }
}

QStringList ApcUpsMon::getUpsKeys() const
{
    return upsData.keys();
}

QString ApcUpsMon::getUpsData(const QString &key) const
{
    return upsData.value(key, QString(""));
}

QMap<QString, QString> ApcUpsMon::getUpsData() const
{
    return upsData;
}

void ApcUpsMon::processResponse()
{
    // Process a response. It could be
    // either a status or an events reponse. We
    // use a regular expression to determine if
    // the response starts with APC [...] and ends
    // with END APC [...], in which case we assume
    // it to be a status response. If not, it must
    // be an events response. Crude but works.
    QTextStream in(response);
    
    lastResponseTimestamp = time(NULL); // Update response timestamp
    m_errorString.clear();  // No errors
    upsData.clear();        // Clear status data
    QRegExp rx("^APC *:.*\nEND APC *:[^\n]+\n$");
    if (rx.exactMatch(response)) {
        // If it has matched, it must be status data
        while (!in.atEnd()) {
            // Split the data in key/value pairs,
            // separated by the first colon found
            // in the line
            QString line = in.readLine();
            int pos = line.indexOf(':');
            if (pos>0) {
                QString key = line.left(pos).trimmed();
                QString value = line.mid(pos+1).trimmed();
                upsData.insert(key, value);
            }
        }
        emit statusReceived();
    } else {
        emit eventsReceived();
    }
    response.clear();
}
