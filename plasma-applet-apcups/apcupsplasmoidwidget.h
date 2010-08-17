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

#ifndef APCUPSPLASMOID_WIDGET_H
#define APCUPSPLASMOID_WIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <Plasma/Label>
#include <Plasma/Meter>
#include <Plasma/IconWidget>

// Class to define the layout of the plasmoid widget
// and update the displayed values
class ApcUpsPlasmoidWidget : public QGraphicsWidget
{
    Q_OBJECT

    public:
        ApcUpsPlasmoidWidget(QGraphicsItem *parent = 0);
        
    public slots:
        
        void setHostname(const QString &hostname);
        void setStatus(const QIcon &icon);
        void setState(const QString &state);
        void setTimeLeft(double act, double max);
        void setLoad(double v);
        void setCharge(double v);
        
    private:
        
        QGraphicsLinearLayout *verticalLayout;
        QGraphicsLinearLayout *horizontalLayout;
        Plasma::IconWidget *statusLed;
        Plasma::Label *hostLabel;
        QGraphicsLinearLayout *horizontalLayout_2;
        Plasma::Label *stateLabel;
        Plasma::Meter *timeLeftBar;
        Plasma::Meter *loadBar;
        Plasma::Meter *chargeBar;
        
};

#endif /* APCUPSPLASMOID_WIDGET_H */
