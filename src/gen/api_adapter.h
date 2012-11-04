/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -a api_adapter -v api_server.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef API_ADAPTER_H_1352017339
#define API_ADAPTER_H_1352017339

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface org.QGVDial.CallServer
 */
class CallServerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.QGVDial.CallServer")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.QGVDial.CallServer\">\n"
"    <method name=\"Call\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"strNumber\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    CallServerAdaptor(QObject *parent);
    virtual ~CallServerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Call(const QString &strNumber);
Q_SIGNALS: // SIGNALS
};

/*
 * Adaptor class for interface org.QGVDial.SettingsServer
 */
class SettingsServerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.QGVDial.SettingsServer")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.QGVDial.SettingsServer\">\n"
"    <method name=\"GetPhoneNames\">\n"
"      <arg direction=\"out\" type=\"as\" name=\"arrPhoneNames\"/>\n"
"    </method>\n"
"    <method name=\"GetCurrentPhone\">\n"
"      <arg direction=\"out\" type=\"i\" name=\"index\"/>\n"
"    </method>\n"
"    <method name=\"SetCurrentPhone\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"index\"/>\n"
"      <arg direction=\"out\" type=\"b\" name=\"success\"/>\n"
"    </method>\n"
"    <signal name=\"CallbacksChanged\"/>\n"
"  </interface>\n"
        "")
public:
    SettingsServerAdaptor(QObject *parent);
    virtual ~SettingsServerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    int GetCurrentPhone();
    QStringList GetPhoneNames();
    bool SetCurrentPhone(int index);
Q_SIGNALS: // SIGNALS
    void CallbacksChanged();
};

/*
 * Adaptor class for interface org.QGVDial.TextServer
 */
class TextServerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.QGVDial.TextServer")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.QGVDial.TextServer\">\n"
"    <method name=\"Text\">\n"
"      <arg direction=\"in\" type=\"as\" name=\"arrNumbers\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"strData\"/>\n"
"    </method>\n"
"    <method name=\"TextWithoutData\">\n"
"      <arg direction=\"in\" type=\"as\" name=\"arrNumbers\"/>\n"
"    </method>\n"
"    <method name=\"getTextsByDate\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"strStart\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"strEnd\"/>\n"
"      <arg direction=\"out\" type=\"as\" name=\"arrTexts\"/>\n"
"    </method>\n"
"    <method name=\"getTextsByContact\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"strContact\"/>\n"
"      <arg direction=\"out\" type=\"as\" name=\"arrTexts\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    TextServerAdaptor(QObject *parent);
    virtual ~TextServerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Text(const QStringList &arrNumbers, const QString &strData);
    void TextWithoutData(const QStringList &arrNumbers);
    QStringList getTextsByContact(const QString &strContact);
    QStringList getTextsByDate(const QString &strStart, const QString &strEnd);
Q_SIGNALS: // SIGNALS
};

#endif
