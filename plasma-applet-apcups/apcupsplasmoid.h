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

#ifndef APCUPSPLASMOID_H
#define APCUPSPLASMOID_H

#include <Plasma/PopupApplet>
#include <Plasma/DataEngine>
#include <Plasma/ToolTipContent>
#include <KDE/KConfigDialog>
#include "apcupsplasmoidwidget.h"
#include "ui_ApcUpsPlasmoidConfiguration.h"

class ApcUpsPlasmoid : public Plasma::PopupApplet
{
    Q_OBJECT
    
    public:
        
        ApcUpsPlasmoid(QObject *parent, const QVariantList &args);
        ~ApcUpsPlasmoid();
        
        void init();
        
        void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect);
        
    signals:
        
        void configurationChanged(const QString &hostname, quint16 port);
        
    protected slots:
        
        // Called when a new source is added to the data engine
        void sourceAdded(const QString &name);
        
        // Called when the data engine receives new data
        void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);
        
        void createConfigurationInterface(KConfigDialog* parent);
        // Called when load values are changed in the configuration
        // interface, in order to keep the spinboxes within valid ranges
        void loadPctRangeCheck(int);
        // Called when the configuration dialogue is Applied or Ok'ed
        void configurationAccepted();
        // Called by configurationAccepted(), requests a new data source
        void readConfiguration(const QString &hostname, quint16 port);
        
    private:
        
        // UPS states, based on load and battery left
        enum UpsState { NormalState, WarningState, CriticalState };
        
        // Main widget
        ApcUpsPlasmoidWidget *container;
        // Tooltip for plasmoid
        Plasma::ToolTipContent tooltip;
        // Configuration form
        Ui::ConfigForm generalConfig;

        QString sourceName; // Name of the dataengine source
        QString hostname;   // Host name of the apcupsd daemon
        quint16 port;       // Port number of the apcupsd daemon
        UpsState state;     // Normal, warning, or critical
        QString status;     // STATUS value from data engine
        double loadPct;
        double loadPctWarning, loadPctCritical;
        double battCharge;
        double battChargeCritical;
        // maxTimeLeft is used to set the scale of the
        // timeLeftBar meter
        double timeLeft, maxTimeLeft;
        QStringList upsEvents; // List of events received
        
        // Utility function that creates a data engine source name
        // based on the given hostname and port values
        QString getSourceName(const QString &h, quint16 p) const;
        // Utility function to extract a double value from a string
        bool getDouble(const char *key, double *dest);
        // Utility function to send notifications to desktop user.
        void notify(QString eventId, QString text);
};

#endif /* APCUPSPLASMOID_H */
