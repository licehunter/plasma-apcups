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

#ifndef APCUPSENGINE_H
#define APCUPSENGINE_H

#include <QMap>
#include <Plasma/DataEngine>

#include "apcupsc.h"
 
/**
 * This engine provides status and event information from
 * APC Uninterruptible Power Supplies, via apcupsd.
 *
 * The apcupsd daemon needs to be running on the hosts to
 * which the target UPSes are connected.
 */
class ApcUpsEngine : public Plasma::DataEngine
{
    Q_OBJECT
 
    public:
        ApcUpsEngine(QObject* parent, const QVariantList& args);
 
    protected:
        // this virtual function is called when a new source is requested
        bool sourceRequestEvent(const QString& name);
        
        void removeSource(const QString& name);
        
        // this virtual function is called when an automatic update
        // is triggered for an existing source (ie: when a valid update
        // interval is set when requesting a source)
        bool updateSourceEvent(const QString& source);
        
    protected slots:
        void statusReceived();
        
    private:
        
        QMap<QString,ApcUpsMon*> hosts;
        
        // Add a host to the list of monitored UPSes
        bool addHost(const QString &name);
};
 
#endif /* APCUPSENGINE_H */
