/* qgvdial Telepathy is GPL.
 * Derived from vicar-telepathy. Its licence follows.
 */

/*
@version: 0.5
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License

Based on Telepathy-SNOM with copyright notice below.
*/

/*
 * Telepathy SNOM VoIP phone connection manager
 * Copyright (C) 2006 by basyskom GmbH
 *  @author Tobias Hunger <info@basyskom.de>
 *
 * This library is free software; you can redisQObject::tribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is disQObject::tributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin SQObject::treet, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "connection.h"
#include "connectionadaptor.h"
#include "connectioninterfacerequestsadaptor.h"
#include "connectioninterfacerequeststypes.h"
#include "connectioninterfacecapabilitiesadaptor.h"
#include "connectioninterfacecapabilitiestypes.h"
#include "names.h"
#include "vicarcallrouterproxy.h"

#include <QtCore>
#include <QtDBus>

namespace
{
static const QString protocol_qgvtp("qgv");

static const QString connection_service_name_prefix("org.freedesktop.Telepathy.Connection." TP_NAME "." + protocol_qgvtp + '.');
static const QString connection_object_path_prefix("/org/freedesktop/Telepathy/Connection/" TP_NAME "/" + protocol_qgvtp + '/');
static const QString requests_interface("org.freedesktop.Telepathy.Connection.Interface.Requests");
}

using namespace org::maemo;


class ConnectionPrivate
{
public:
    ConnectionPrivate(Connection * p,
                      const QString & acc)
    : account(acc)
    , connection_status(Connection::Disconnected)
    , adaptor(new ConnectionAdaptor(p))
    , connIfaceReqsAdaptor(new ConnectionInterfaceRequestsAdaptor(p))
    , parent(p)
    {
        Q_ASSERT(0 != adaptor);
    }

    ~ConnectionPrivate()
    {
        qDebug() << "QgvTp: Connection Destructing";
    }

    const QString account;

    Connection::Status connection_status;
    ConnectionAdaptor * adaptor;
    ConnectionInterfaceRequestsAdaptor * connIfaceReqsAdaptor;
    Connection * const parent;
};

// ---------------------------------------------------------------------------

Connection::Connection(const QString & account, QObject * parent)
: QObject(parent)
, d(new ConnectionPrivate(this, account))
{
    Q_ASSERT(0 != d);
    Q_ASSERT(!account.isEmpty());

    /*  -- Set the Dynamic property "Interfaces" ---

        Apparently it is not sufficient to implement an additional interface like Conn.I.Requests.
        We have to assign the list of additional interfaces to the DBus Property Interfaces.

        The actual DBus property "Interfaces" is declared in ConnectionAdaptor class,
         which is our Connection Interface implementation.
     */

    QStringList interfaces = QStringList(requests_interface);
    this->setProperty("Interfaces",interfaces);


    //Set the property RequestableChannelClasses
    org::freedesktop::Telepathy::RequestableChannelClassList requestableChannelClasses;

    uint targetHandleType(1);

    org::freedesktop::Telepathy::RequestableChannelClass requestableChannelClass1;
    requestableChannelClass1.fixedProperties.insert("org.freedesktop.Telepathy.Channel.TargetHandleType",targetHandleType);
    requestableChannelClass1.fixedProperties.insert("org.freedesktop.Telepathy.Channel.ChannelType","org.freedesktop.Telepathy.Channel.Type.StreamedMedia");

    requestableChannelClass1.allowedProperties.append("org.freedesktop.Telepathy.Channel.TargetHandle");
    requestableChannelClass1.allowedProperties.append("org.freedesktop.Telepathy.Channel.Type.StreamedMedia.InitialAudio");

    requestableChannelClasses.append(requestableChannelClass1);

    org::freedesktop::Telepathy::RequestableChannelClass requestableChannelClass2;
    requestableChannelClass2.fixedProperties.insert("org.freedesktop.Telepathy.Channel.TargetHandleType",targetHandleType);
    requestableChannelClass2.fixedProperties.insert("org.freedesktop.Telepathy.Channel.ChannelType","org.freedesktop.Telepathy.Channel.Type.StreamedMedia");

    requestableChannelClass2.allowedProperties.append("com.nokia.Telepathy.Channel.Interface.Conference.InitialMembers");
    requestableChannelClass2.allowedProperties.append("org.freedesktop.Telepathy.Channel.TargetHandleType");
    requestableChannelClass2.allowedProperties.append("org.freedesktop.Telepathy.Channel.Type.StreamedMedia.InitialAudio");

    requestableChannelClasses.append(requestableChannelClass2);


    this->setProperty("RequestableChannelClasses",QVariant::fromValue(requestableChannelClasses));



    qDebug() << "qgvtp: Connection set up.";
}

Connection::~Connection()
{
    qDebug() << "qgvtp: Connection closed.";
    delete(d);
}

bool Connection::registerObject()
{
    if (!QDBusConnection::sessionBus().registerService(serviceName()))
    {
        qDebug() << "qgvtp: Problem registering connection service:" << serviceName();
        return false;
    }

    if (!QDBusConnection::sessionBus().registerObject(objectPath().path(),
                                                      this))
    {
        qDebug() << "qgvtp: Problem registering object path:" << objectPath().path();
        return false;
    }
    return true;
}

void Connection::unregisterObject()
{
    qDebug() << "qgvtp: Unregistering Connection object from DBus";
    QDBusConnection::sessionBus().unregisterObject(objectPath().path());
    QDBusConnection::sessionBus().unregisterService(serviceName());
}

QString Connection::name() const
{    
    return QString("qgvtp");
}


QString Connection::serviceName() const
{ return connection_service_name_prefix + name(); }

QDBusObjectPath Connection::objectPath() const
{ return QDBusObjectPath(connection_object_path_prefix + name()); }


//org.freedesktop.Telepathy.Connection
void Connection::Connect()
{
    /*
       Since this is not a "real" Telepathy Connection to a SIP, Chat server,
       I am not connecting to anything.
     */
    qDebug() << "qgvtp: Changing status to Connected...";
    d->connection_status = Connection::Connected;

    //Let all the Telepathy clients know that connection status has changed
    qDebug() << "qgvtp: Emitting StatusChanged.";
    emit StatusChanged(d->connection_status, ReasonRequested);

}

void Connection::Disconnect()
{
    qDebug() << "qgvtp: Changing status to Disconnected...";
    //We don't have any Handles to release here. So just change the status to Disconnected
    d->connection_status = Connection::Disconnected;

    qDebug() << "qgvtp: Emitting StatusChanged";
    emit StatusChanged(d->connection_status, ReasonRequested);

    //As per Telepathy specfication, on disconnect we need to unregister from Dbus and destroy the object.
    unregisterObject();
    deleteLater();
}

QStringList Connection::GetInterfaces()
{
    QStringList result;
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to get Interfaces List. The connection is no longer available.");
        return result;
    }    
    result <<requests_interface;
    return result;
}

QString Connection::GetProtocol()
{ return protocol_qgvtp; }

uint Connection::GetStatus()
{ return static_cast<uint>(d->connection_status); }

uint Connection::GetSelfHandle()
{
    qDebug() << "qgvtp: GetSelfHandle";
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to get Self Handle. The connection is no longer available.");
        qDebug() << "qgvtp: NOT CONNECTED when requesting selfhandle!";
        return 0;
    }

    //WARNING: Incomplete implemenation
    uint handle = 0;
    qDebug() << "qgvtp: Returning Handle" << handle << "as self handle.";
    return handle;
}

QList<uint> Connection::RequestHandles(uint handle_type,
                                       const QStringList & names)
{
    Q_UNUSED(names);
    QList<uint> result;

    // check input:
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to process handle request. The connection is no longer available.");
        return result;
    }
    if (handle_type != HandleContact)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Supports handles of type Contact only.");
        return result;
    }

    //WARNING: Incomplete implementation. Create a handle and return the value here.
    return result;
}

void Connection::HoldHandles(const uint handle_type, const QList<uint> &handles)
{
    Q_UNUSED(handles);
    qDebug() << "qgvtp: HoldHandles.";
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to process handle request. The connection is no longer available.");
        return;
    }
    if (handle_type != HandleContact)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Supports handles of type Contact only.");
        return;
    }

    //WARNING: Incomplete implementation
}

QStringList Connection::InspectHandles(const uint handle_type,
                                       const QList<uint> &handles)
{
    Q_UNUSED(handles);
    QStringList result;

        // check input:
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to process handle request. The connection is no longer available.");
        return result;
    }
    if (handle_type != HandleContact)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Supports handles of type Contact only.");
        return result;
    }

    //WARNING: Incomplete implementation
    return result;
}

void Connection::ReleaseHandles(const uint handle_type, const QList<uint> &handles)
{
    Q_UNUSED(handles);
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to release handle. The connection is no longer available.");
        qDebug() << "qgvtp: Releasing Handle while connection is no longer connected.";
        return;
    }
    if (handle_type != HandleContact)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Supports handles of type Contact only.");
        qDebug() << "qgvtp: Trying to release a Handle that is not a contact.";
        return;
    }

    //WARNING: Incomplete implementation
}

org::freedesktop::Telepathy::ChannelInfoList Connection::ListChannels()
{
    org::freedesktop::Telepathy::ChannelInfoList result;
    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp - Unable to list channels. The connection is no longer available.");
        return result;
    }

    //WARNING: Incomplete implementation
    //Btw - We never have any channels :)

    return result;
}

QDBusObjectPath Connection::RequestChannel(const QString &type,
                                           uint handle_type, uint handle,
                                           bool suppress_handler)
{
    Q_UNUSED(handle);
    Q_UNUSED(suppress_handler);
    //This method is deprecated and no longer used as per latest Telepathy spec

    if (type != QString("org.freedesktop.Telepathy.Channel.Type.StreamedMedia"))
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.NotImplemented",
                       "qgvtp: Failed to create channel: Channel type not implemented.");
        return QDBusObjectPath();
    }

    if (handle_type != HandleContact )
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidHandle",
                       "qgvtp: Failed to create channel: Handle type not supported.");
        return QDBusObjectPath();
    }

    if (d->connection_status != Connected)
    {
        sendErrorReply("org.freedesktop.Telepathy.Error.Disconnected",
                       "qgvtp: Failed to create channel: Connection is Disconnected.");
        return QDBusObjectPath();
    }

    //TODO qgvtp Specific code here

    //WARNING: Incomplete implementation, we are not creating any channels here at all.
    QDBusObjectPath channel_path;
    return channel_path;
}

//org.freedesktop.Telepathy.Connection.Interface.Requests
QDBusObjectPath Connection::CreateChannel(const QVariantMap &request,
                                                           QVariantMap &channel_properties)
{
    Q_UNUSED(channel_properties);
    Q_ASSERT(!request.isEmpty());
    qDebug() << "qgvtp: CreateChannel";
    qDebug() << " Request details are: "<< request;

     //Ideally we need to emit NewChannels signal here, but since we are not creating any channels we ignore it

    //WARNING: qgvtp - Specific implementation
    return processChannel(request);

}

bool Connection::EnsureChannel(const QVariantMap &request,
                                                QDBusObjectPath &channel_object,
                                                QVariantMap &channel_properties)
{
    Q_UNUSED(channel_object);
    Q_UNUSED(channel_properties);
    Q_ASSERT(!request.isEmpty());
    qDebug() << "qgvtp: EnsureChannel";
    qDebug() << " Request details are: "<< request;

    //WARNING: Incomplete implementation
    processChannel(request);

    return true;
}

QDBusObjectPath Connection::processChannel(const QVariantMap &request){

    QDBusObjectPath channel_path;

    if (!request.contains("org.freedesktop.Telepathy.Channel.TargetID")){
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Invalid request. TargetID (Phone Number) not included.");
        return channel_path;
    }

    QVariant vNumber = request.value("org.freedesktop.Telepathy.Channel.TargetID");
    if (!vNumber.isValid()){
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Invalid request. Phone Number is not valid.");
        return channel_path;
    }
    QString strNumber = vNumber.toString();
    if (strNumber.isEmpty()){
        sendErrorReply("org.freedesktop.Telepathy.Error.InvalidArgument",
                       "qgvtp - Invalid request. Phone Number is empty.");
        return channel_path;
    }

    /*
        Send an error reply to Tp Client (Mission Control) to force it to close the active channel.
        Once it recieves the reply, the client does not bother what we return.

     */

    sendErrorReply("org.freedesktop.Telepathy.Error.NotAvailable",
                   "qgvtp - Creating a new channel to "+strNumber+" via qgvdial.");


    // This is where we call QGVDIAL.
    QDBusInterface iface("org.QGVDial.CallServer",
                         "/org/QGVDial/CallServer",
                         "", 
                         QDBusConnection::sessionBus());
    if (!iface.isValid()) {
        qDebug ("QGVDial interface is not ready");
        return channel_path;
    }
        
    iface.call("Call", strNumber);

/*
    //Initiate a new call to CC/Google Out/Skype-out number by requesting a new channel with Ring CM.
    VicarCallRouterProxy *callRouter = new VicarCallRouterProxy(APPLICATION_DBUS_SERVICE,APPLICATION_DBUS_PATH,QDBusConnection::sessionBus(),this);

    callRouter->callInternationalNumber(strNumber);

    qDebug() << "qgvtp: Call is processed.";
*/

    return channel_path;
}


//org.freedesktop.Telepathy.Connection.Interface.Capabilities
org::freedesktop::Telepathy::ContactCapabilitiesList Connection::GetCapabilities(const QList<uint> &Handles){
    Q_UNUSED(Handles);
    org::freedesktop::Telepathy::ContactCapabilitiesList capabilities;
    return capabilities;

}


org::freedesktop::Telepathy::CapabilityPairList Connection::AdvertiseCapabilities(org::freedesktop::Telepathy::CapabilityPairList Add, const QStringList &Remove){
    Q_UNUSED(Add);
    Q_UNUSED(Remove);
    org::freedesktop::Telepathy::CapabilityPairList capabilities;
    return capabilities;
}
