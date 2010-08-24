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

#include "apcupsengine.h"

#include <Plasma/DataContainer>

ApcUpsEngine::ApcUpsEngine(QObject* parent, const QVariantList& args)
: Plasma::DataEngine(parent, args)
{
    Q_UNUSED(args)
    
    // This prevents applets from setting an unnecessarily high
    // update interval and using too much CPU.
    // Although we set this to 1 sec, the underlying polling of
    // the UPSes is controlled by the ApcUpsMon object (see below)
    // at whichever update rate we use there.
    setMinimumPollingInterval(1000);
}

bool ApcUpsEngine::sourceRequestEvent(const QString &name)
{
    return addHost(name);
}

void ApcUpsEngine::removeSource(const QString &name)
{
    if (hosts.contains(name)) {
        ApcUpsMon *host = hosts.take(name);
        if(host)
            delete(host);
    }
    Plasma::DataEngine::removeSource(name);
}

bool ApcUpsEngine::updateSourceEvent(const QString &name)
{
    Q_UNUSED(name);
    // We do asynchronous updates, so always return false
    return false;
}

bool ApcUpsEngine::addHost(const QString &name)
{
    QString hostname;
    quint16 port;
    
    if (hosts.contains(name)) {
        // We're already tracking this source
        return false;
    }
    
    // Check to see if we have a source name
    // of the form hostname:port, and assign
    // hostname and port values accordingly.
    if (name.contains(':')) {
        hostname = name.left(name.lastIndexOf(':'));
        port = name.section(':', -1).toUInt();
        if (port == 0)
            port = 3551;
    } else {
        hostname = name;
        port = 3551;
    }
    
    // Here we hard code the update rate to five seconds.
    // This should be responsive enough while avoiding
    // excessive network traffic
    ApcUpsMon *host = new ApcUpsMon(hostname, port, 5);
    connect(host, SIGNAL(statusReceived()), SLOT(statusReceived()));
    connect(host, SIGNAL(haveError(const QString&)), SLOT(statusReceived()));
    hosts.insert(name, host);
    updateSourceEvent(name);
    return true;
}

void ApcUpsEngine::statusReceived()
{
    // We have received an update from apcupsd.
    // Feed all key / value pairs to the data engine
    QObject *host = sender();
    if (!host)
        return;
    QMap<QString, ApcUpsMon*>::const_iterator j;
    for (j = hosts.constBegin(); j != hosts.constEnd(); ++j) {
        QMap<QString, QString>::const_iterator i;
        QString name;
        if (j.value() != host)
            continue;
        name = j.key();
        removeAllData(name);    // Do not keep stale data
        if (hosts.value(name)->hasError()) {
            // If there was an error, we set a key called "Error"
            // with the error message as its value
            setData(name, "Error", hosts.value(name)->errorString());
        } else {
            for (i = hosts.value(name)->getUpsData().constBegin(); i != hosts.value(name)->getUpsData().constEnd(); ++i)
                setData(name, i.key(), i.value());
        }
    }
}


// This does the magic that allows Plasma to load
// this plugin.  The first argument must match
// the X-Plasma-EngineName in the .desktop file.
K_EXPORT_PLASMA_DATAENGINE(apcups, ApcUpsEngine)

#include "apcupsengine.moc"
