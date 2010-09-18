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

#include "apcupsplasmoidwidget.h"

ApcUpsPlasmoidWidget::ApcUpsPlasmoidWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
    verticalLayout = new QGraphicsLinearLayout(Qt::Vertical);
    
    horizontalLayout = new QGraphicsLinearLayout(verticalLayout);
    verticalLayout->addItem(horizontalLayout);
    hostLabel = new Plasma::Label();
    horizontalLayout->addItem(hostLabel);
    statusLed = new Plasma::IconWidget();
    statusLed->setIcon("apcups");
    horizontalLayout->addItem(statusLed);
    
    horizontalLayout_2 = new QGraphicsLinearLayout(verticalLayout);
    verticalLayout->addItem(horizontalLayout_2);
    stateLabel = new Plasma::Label();
    timeLeftBar = new Plasma::Meter();
    timeLeftBar->setMeterType(Plasma::Meter::BarMeterHorizontal);
    timeLeftBar->setMaximum(100);
    timeLeftBar->setMinimum(0);
    timeLeftBar->setLabel(0, i18nc("Label showing how much time is left running on batteries.", "Left"));
    timeLeftBar->setLabelAlignment(0, Qt::AlignLeft|Qt::AlignVCenter);
    horizontalLayout_2->addItem(stateLabel);
    horizontalLayout_2->addItem(timeLeftBar);
    
    loadBar = new Plasma::Meter();
    loadBar->setMeterType(Plasma::Meter::BarMeterHorizontal);
    loadBar->setMaximum(100);
    loadBar->setMinimum(0);
    loadBar->setLabel(0, i18nc("Label showing UPS load, in percentage (i.e., how much stuff is connected to it).", "Load"));
    loadBar->setLabelAlignment(0, Qt::AlignLeft|Qt::AlignVCenter);
    chargeBar = new Plasma::Meter();
    chargeBar->setMeterType(Plasma::Meter::BarMeterHorizontal);
    chargeBar->setMaximum(100);
    chargeBar->setMinimum(0);
    chargeBar->setLabel(0, i18nc("Label showing how much charge the battery currently has.", "Charge"));
    chargeBar->setLabelAlignment(0, Qt::AlignLeft|Qt::AlignVCenter);
    verticalLayout->addItem(loadBar);
    verticalLayout->addItem(chargeBar);
    
    setMinimumSize(QSizeF(128, 100));
    setLayout(verticalLayout);
    show();
    hostLabel->setText(i18nc("Default text of label showing the hostname. Not normally seen.", "Hostname"));
    stateLabel->setText(i18nc("Default text of label showing the status. Not normally seen.", "STATUS"));
    timeLeftBar->setValue(100);
}

void ApcUpsPlasmoidWidget::setHostname(const QString &hostname)
{
    hostLabel->setText(hostname);
}

// FIXME - There was a naming mixup, this should be
// setStatus()
void ApcUpsPlasmoidWidget::setState(const QString &state)
{
    stateLabel->setText(state);
    if (state.indexOf("ERROR") == 0) {
        // If we get an error status, do not show
        // any of the meters.
        timeLeftBar->hide();
        loadBar->hide();
        chargeBar->hide();
    } else {
        // Only show the timeLeftBar if we're
        // running on batteries
        if (state == "ONLINE")
            timeLeftBar->hide();
        else
            timeLeftBar->show();
        loadBar->show();
        chargeBar->show();
    }
}

// FIXME - There was a naming mixup, this should be
// setState()
void ApcUpsPlasmoidWidget::setStatus(const QIcon& icon)
{
    statusLed->setIcon(icon);
}

void ApcUpsPlasmoidWidget::setTimeLeft(double act, double max)
{
    timeLeftBar->setMaximum(max);
    timeLeftBar->setValue(act);
    timeLeftBar->setLabel(1, QString("%1 m").arg(act));
}

void ApcUpsPlasmoidWidget::setLoad(double v)
{
    loadBar->setValue(v);
    loadBar->setLabel(1, QString("%1%").arg(v));
}

void ApcUpsPlasmoidWidget::setCharge(double v)
{
    chargeBar->setValue(v);
    chargeBar->setLabel(1, QString("%1%").arg(v));
}
