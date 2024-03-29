/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c ApcUpsMonAdaptor -a apcupscif eu.navlost.apcupsmon.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

// NOTE - I can't seem to get the relevant CMake macro to work in order to
// autogenerate the whole thing via qdbuscpp2xml and qdbusxml2cpp, so I just
// do it manually and include the generated files in the source distribution.
// FIXES are welcome.

#ifndef APCUPSCIF_H
#define APCUPSCIF_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface eu.navlost.apcupsmon
 */
class ApcUpsMonAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "eu.navlost.apcupsmon")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"eu.navlost.apcupsmon\">\n"
"    <signal name=\"statusReceived\"/>\n"
"    <signal name=\"eventsReceived\"/>\n"
"    <signal name=\"haveError\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </signal>\n"
"    <method name=\"connectToHost\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"host\"/>\n"
"      <arg direction=\"in\" type=\"u\" name=\"port\"/>\n"
"    </method>\n"
"    <method name=\"connectToHost\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"host\"/>\n"
"    </method>\n"
"    <method name=\"setInterval\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"updateInterval\"/>\n"
"    </method>\n"
"    <method name=\"requestStatus\"/>\n"
"    <method name=\"requestEvents\"/>\n"
"    <method name=\"errorString\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"stopUpdates\"/>\n"
"    <method name=\"getUpsKeys\">\n"
"      <arg direction=\"out\" type=\"as\"/>\n"
"    </method>\n"
"    <method name=\"getUpsData\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"key\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    ApcUpsMonAdaptor(QObject *parent);
    virtual ~ApcUpsMonAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void connectToHost(const QString &host);
    void connectToHost(const QString &host, uint port);
    QString errorString();
    QString getUpsData(const QString &key);
    QStringList getUpsKeys();
    void requestEvents();
    void requestStatus();
    void setInterval(int updateInterval);
    void stopUpdates();
Q_SIGNALS: // SIGNALS
    void eventsReceived();
    void haveError(const QString &in0);
    void statusReceived();
};

#endif
