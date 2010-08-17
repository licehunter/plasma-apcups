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

#ifndef APCUPSMON_H
#define APCUPSMON_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMap>
#include <QStringList>

// Class that talks to an apcupsd daemon.
// It can poll it at set intervals or on demand and it can request
// the UPS status or events.
// It also implements a D-BUS interface.
class ApcUpsMon : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "eu.navlost.apcupsmon")

public:

    ApcUpsMon(QString host = QString("localhost"), quint16 port = 3551, int updint = 5, QObject *parent = NULL);
    
    bool hasError() const;

public Q_SLOTS:
    
    // Connect to apcupsd daemon
    void connectToHost(QString host, unsigned int port = 3551);
    // Set update interval in msec, set to 0 for no updates
    void setInterval(int updateInterval);
    // Request status info, can be called manually or
    // automatically by using setInterval()
    void requestStatus();
    // Request events info. Manual calling only.
    void requestEvents();
    // Returns the error string, or QString() if no error
    QString errorString() const;
    // Stop polling and disconnect from apcupsd daemon
    void stopUpdates();
    // Return the data keys received from apcupsd
    QStringList getUpsKeys() const;
    // Return one data value corresponding to specified key
    QString getUpsData(QString key) const;
    // Return all keys/data value pairs
    QMap<QString, QString> getUpsData() const;
    
signals:
    // A status message was received
    Q_SCRIPTABLE void statusReceived();
    // An events message was received
    Q_SCRIPTABLE void eventsReceived();
    // An error has occurred
    Q_SCRIPTABLE void haveError(const QString&);
    
private Q_SLOTS:
    
    // Read data from socket
    void socketRead();
    // Called by QTcpSocket in case of error
    void socketError(QAbstractSocket::SocketError);
    // Called when setInterval() used
    void timeout();
    // Process a response once it has been completely
    // received
    void processResponse();
    
private:

    QString host;
    unsigned int port;
    QTcpSocket socket;
    // QTimer used by setInterval()
    QTimer timer;
    // Hold the response text, possibly incomplete as it
    // may come in more than one socketRead() call
    QByteArray response;
    // How many bytes left to read, if not all received
    // on previous socketRead() call
    qint64 bytesLeft;
    // Hold status key / value pairs
    QMap<QString,QString> upsData;
    // Hold the current error message, or QString() if no error
    QString m_errorString;
};

#endif /* APCUPSMON_H */
