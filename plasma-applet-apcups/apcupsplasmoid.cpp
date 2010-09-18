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

#include <QGraphicsScene>
#include <QPainter>
#include <Plasma/ToolTipManager>
#include <KConfigGroup>
#include <KNotification>
#include "apcupsplasmoid.h"

ApcUpsPlasmoid::ApcUpsPlasmoid(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
{
    setBackgroundHints(DefaultBackground);
    resize(160, 160);
    hostname = QString();
    loadPct = 0;
    battCharge = 0;
    timeLeft = 0;
    maxTimeLeft = 0;
    status = QString("N/A");
    upsEvents = QStringList();
}

ApcUpsPlasmoid::~ApcUpsPlasmoid()
{
    dataEngine("apcups")->disconnectSource(hostname, this);
}

void ApcUpsPlasmoid::init()
{
    container = new ApcUpsPlasmoidWidget(this);
    container->setPreferredSize(160,160);
    setHasConfigurationInterface(true);
    if (!dataEngine("apcups") || !dataEngine("apcups")->isValid()) {
        setFailedToLaunch(true, i18n("The apcups data engine could not be initialised. Try restarting KDE."));
    } else {
        KConfigGroup cfg = config();
        hostname = cfg.readEntry("hostname", "localhost");
        port = cfg.readEntry("port", 3551);
        battChargeCritical = cfg.readEntry("battChargeCritical", 20);
        loadPctWarning = cfg.readEntry("loadPctWarning", 50);
        loadPctCritical = cfg.readEntry("loadPctCritical", 80);
        sourceName = getSourceName(hostname, port);
        
        setGraphicsWidget(container);
        setPopupIcon("apcups");
        tooltip.setMainText(i18n("APC UPS Monitor"));
        tooltip.setSubText(sourceName);
        tooltip.setImage(KIcon("apcups").pixmap(IconSize(KIconLoader::Desktop)));
        Plasma::ToolTipManager::self()->registerWidget(this);
        
        connect(this, SIGNAL(configurationChanged(QString, quint16)),
                this, SLOT(readConfiguration(QString, quint16)));
        connect(dataEngine("apcups"), SIGNAL(sourceAdded(QString)),
                this, SLOT(sourceAdded(QString)));
                
        dataEngine("apcups")->connectSource(sourceName, this, 5000);
    }
}

void ApcUpsPlasmoid::createConfigurationInterface(KConfigDialog* parent)
{
    
    KConfigGroup cfg = config();
    cfg.readEntry("hostname", hostname);
    cfg.readEntry("port", (unsigned int)port);
    cfg.readEntry("battChargeCritical", battChargeCritical);
    cfg.readEntry("loadPctWarning", loadPctWarning);
    cfg.readEntry("loadPctCritical", loadPctCritical);
    
    QWidget *configUi = new QWidget();
    generalConfig.setupUi(configUi);
    generalConfig.hostname->setText(hostname);
    generalConfig.port->setValue(port);
    generalConfig.battChargeCritical->setValue(battChargeCritical);
    generalConfig.loadPctWarning->setValue(loadPctWarning);
    generalConfig.loadPctCritical->setValue(loadPctCritical);
    parent->addPage(configUi, i18n("Settings"), "apcups");
    
    connect(generalConfig.loadPctWarning, SIGNAL(valueChanged(int)),
            this, SLOT(loadPctRangeCheck(int)));
            connect(generalConfig.loadPctCritical, SIGNAL(valueChanged(int)),
                    this, SLOT(loadPctRangeCheck(int)));
                    connect(parent, SIGNAL(applyClicked()), this, SLOT(configurationAccepted()));
                    connect(parent, SIGNAL(okClicked()), this, SLOT(configurationAccepted()));
}

void ApcUpsPlasmoid::loadPctRangeCheck(int)
{
    if (sender() == generalConfig.loadPctWarning) {
        generalConfig.loadPctCritical->setMinimum(qMin(100,generalConfig.loadPctWarning->value()+1));
    } else if (sender() == generalConfig.loadPctCritical) {
        generalConfig.loadPctWarning->setMaximum(qMax(0,generalConfig.loadPctCritical->value()-1));
    }
}

void ApcUpsPlasmoid::configurationAccepted()
{
    QString h = generalConfig.hostname->text();
    quint16 p = generalConfig.port->value();
    battChargeCritical = generalConfig.battChargeCritical->value();
    loadPctWarning = generalConfig.loadPctWarning->value();
    loadPctCritical = generalConfig.loadPctCritical->value();
    KConfigGroup cfg = config();
    cfg.writeEntry("hostname", h);
    cfg.writeEntry("port", (unsigned int)p);
    cfg.writeEntry("battChargeCritical", battChargeCritical);
    cfg.writeEntry("loadPctWarning", loadPctWarning);
    cfg.writeEntry("loadPctCritical", loadPctCritical);
    
    emit configurationChanged(h, p);
}

void ApcUpsPlasmoid::readConfiguration(const QString &h, quint16 p)
{
    if (h != hostname || p != port) {
        dataEngine("apcups")->disconnectSource(sourceName, this);
        hostname = h;
        port = p;
        sourceName = getSourceName(hostname, port);
        connect(dataEngine("apcups"), SIGNAL(sourceAdded(QString)),
                this, SLOT(sourceAdded(QString)));
                dataEngine("apcups")->connectSource(sourceName, this, 5000);
    }
}

void ApcUpsPlasmoid::sourceAdded(const QString &name)
{
    if (name == sourceName) {
        // A new source was added to the dataengine. Connect to it
        // if the source name is the same as the hostname we are
        // monitoring.
        dataEngine("apcups")->connectSource(name, this, 5000);
        disconnect(dataEngine("apcups"), SIGNAL(sourceAdded(QString)),
                   this, SLOT(sourceAdded(QString)));
    }
}

// Called whenever the dataengine gets new data.
void ApcUpsPlasmoid::dataUpdated(const QString &name, const Plasma::DataEngine::Data &data)
{
    if (name != sourceName)
        return;
    if (data.keys().count() == 0)
        return;
    
    if (!data.contains("Error")) { // If no error
        // get the values we are interested on
        status = data.value("STATUS").toString();
        getDouble(data.value("LOADPCT").toString().toAscii(), &loadPct);
        getDouble(data.value("BCHARGE").toString().toAscii(), &battCharge);
        getDouble(data.value("TIMELEFT").toString().toAscii(), &timeLeft);
        maxTimeLeft = qMax(timeLeft, maxTimeLeft);
        
        // Initialise state variables before use.
        // TODO - A better option would be to add a third state,
        // UnknownState to cater for the case where we cannot
        // determine the UPS state e.g., due to comms errors, etc.
        UpsState loadState = NormalState;
        UpsState battState = NormalState;
        
        // Work out the UPS state as regards battery charge
        if (status == "ONLINE") {
            battState = NormalState;
        } else {
            if (battCharge > battChargeCritical)
                battState = WarningState;
            else
                battState = CriticalState;
        }
        // Work out the UPS state as regards connected load
        if (loadPct < loadPctWarning)
            loadState = NormalState;
        else if (loadPct > loadPctCritical)
            loadState = CriticalState;
        else
            loadState = WarningState;
        
        // Take the worst of the two states
        state = qMax(battState, loadState);
        
        // Change the popup icon according to UPS state
        switch (state) {
            case NormalState:
                setPopupIcon("apcups_normalstate");
                break;
            case WarningState:
                setPopupIcon("apcups_warningstate");
                break;
            case CriticalState:
                setPopupIcon("apcups_criticalstate");
                break;
            default:
                setPopupIcon("apcups");
        }
        
        // Check for events
        if (data.contains("events")) {
            if (upsEvents.length() == 0) {
                // When we first start our run, we just collect any
                // events that come through the first time, and we'll
                // assume they are in the past. We then deal we any new
                // events that arrive subsequently.
                upsEvents = data.value("events").toString().split("\n",  QString::SkipEmptyParts);
            } else {
                QString evt;
                QStringList newEvents = data.value("events").toString().split("\n",  QString::SkipEmptyParts);
                
                // Go through the list we just received
                int i = 0;
                while (i < newEvents.length()) {
                    // If this event has already been seen
                    if (upsEvents.contains(newEvents.at(i))) {
                        // Remove it from the new events list
                        newEvents.removeAt(i);
                    } else {
                        QString evt = newEvents.at(i);
                        // ...if not, append it to the seen events list
                        upsEvents.append(evt);
                        // ...and do a bit of reformatting
                        newEvents.replace(i++, QString("<i>%1</i><br/>%2").arg(evt.left(28)).arg(evt.mid(28)));
                    }
                }
                
                if (newEvents.length()) {
                  // In the end, issue a notification with any new, unseen events we have received.
                  // FIXME - We should be using our own eventid ("upsEvent"), but I can't get it
                  // to work for some unfathomable reason :(  In the meanwhile, a standard event
                  // will have to do.
                  // NOTE - Default event changed to custom event, still need to see if it works.
                  KNotification::event(QString::fromLatin1("upsEvent"),
                    QString(i18n("APC UPS Monitor - %1")).arg(sourceName),
                    QString(i18n("%1").arg(newEvents.join("\n"))));
                }
            }
        }
    } else {
        // There was an error, update the status
        // widget to show the error message.
        // FIXME - The target widget would need resizing
        // as often the errors are fairly long.
        status = "ERROR: ";
        status.append(data.value("Error").toString());
        
        // Set the popup icon to the "no-state" version
        setPopupIcon("apcups");
    }
    
    update();
}

void ApcUpsPlasmoid::paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(contentsRect)
    
    container->setHostname(sourceName);
    container->setStatus(popupIcon());
    container->setState(status);
    container->setLoad(loadPct);
    container->setCharge(battCharge);
    container->setTimeLeft(timeLeft, maxTimeLeft);
    
    tooltip.setSubText(QString("%1 (%2)").arg(sourceName).arg(status));
    Plasma::ToolTipManager::self()->setContent(this, tooltip);    
}

QString ApcUpsPlasmoid::getSourceName(const QString &h, quint16 p) const
{
    return (port == 3551 && h.indexOf(":") == -1 ? h : QString("%1:%2").arg(h).arg(p));
}

// Utility function, extracts a double out of a string, which
// may contain trailing non-numeric characters.
// Returns true on success, false otherwise.
bool ApcUpsPlasmoid::getDouble(const char *str, double *dest)
{
    if ((sscanf(str, "%lf", dest) == 1)) {
        return true;
    }
    return false;
}

K_EXPORT_PLASMA_APPLET(apcups, ApcUpsPlasmoid);

#include "apcupsplasmoid.moc"
