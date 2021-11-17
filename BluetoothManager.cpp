#include "BluetoothManager.h"
#include <iostream>
#include <cstdlib>

BluetoothManager::BluetoothManager() {

}

void BluetoothManager::connectToADevice(QBluetoothLocalDevice &lDevice, const QBluetoothAddress &address) {
    lDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
}

void BluetoothManager::connectToDeviceTemp(QString &address) {
    std::string passString = "bluetoothctl connect " + address.toStdString();
    system(passString.c_str());
}

BluetoothManager::~BluetoothManager() {

}

LocalBluetoothDevice::LocalBluetoothDevice() {
    //Check if BT is in your system
    if (localDevice.isValid()) {
        //turn BT ON
        localDevice.powerOn();
        localDeviceName = localDevice.name();
        //make it visible to others
        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }
}

QString LocalBluetoothDevice::getLocalBTDeviceName() {
    return localDeviceName;
}

QList<QBluetoothAddress> LocalBluetoothDevice::getConnectedDevices() {
    remoteDevices = localDevice.connectedDevices();
    //QList<QString> = QString(remoteDevices);
    return remoteDevices;
}

void LocalBluetoothDevice::startDeviceDiscovery() {
    //well this is embarssing
    envDevices.clear();
    //signal shit boi
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                     this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));
    discoveryAgent->start();
    discoveryAgent->stop();
    delete discoveryAgent;
}

void LocalBluetoothDevice::deviceDiscovered(const QBluetoothDeviceInfo &device) {
    qDebug() << "Found new device: " << device.name() << '(' << device.address().toString() << ')';
    envDevices.append(QString(device.name() + " " + device.address().toString()));
}

QList<QString> LocalBluetoothDevice::getAllScannedDevices() {
    qDebug() << envDevices;
    return envDevices;
}

BluetoothServer::BluetoothServer(QObject *parent)
    : QObject(parent) {

}

// Uuid string boi
static const QLatin1String serviceUuid("04c6093b-0000-1000-8000-00805f9b34fb");

void BluetoothServer::startBTServer(const QBluetoothAddress &localAdapter) {
    if (rfcommServer) {
        return;
    }

    // Create the server
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&BluetoothServer::clientConnected));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "[WARNING]: Cannot bind server to " + localAdapter.toString() + ".";
        return;
    } else {
        qDebug() << "[NOTICE]: Bluetooth server started.";
    }
    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;

    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(qint16(0x100));

    profileSequence.append(QVariant::fromValue(classId));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    // Service name, description and provider here
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("BT_Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Just My Shit Bluetooth Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("www.fuckof.com"));

    // Service Uuid
    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    // Service discoverabilty
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

    // Protocol descriptor
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    // Register Service
    serviceInfo.registerService(localAdapter);
}

void BluetoothServer::stopBTServer() {
    qDebug() << "[NOTICE]: Bluetooth server stoped.";
    // Unregister service
    serviceInfo.unregisterService();
    // Close sockets
    qDeleteAll(clientSockets);
    // Close server
    delete rfcommServer;
    rfcommServer = nullptr;
}

void BluetoothServer::clientConnected() {
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket) {
        return;
    }

    connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothServer::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this,
            QOverload<>::of(&BluetoothServer::clientDisconnected));
    clientSockets.append(socket);
    emit clientConnected(socket->peerName());
}

void BluetoothServer::clientDisconnected() {
    //qDebug() << "Got here again.";
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket) {
        return;
    }
    emit clientDisconnected(socket->peerName());
    clientSockets.removeOne(socket);
    socket->deleteLater();
}

void BluetoothServer::sendData(const QString &data) {
    QByteArray dataTS = data.toUtf8() + '\n';
    for (QBluetoothSocket *socket: qAsConst(clientSockets)) {
        socket->write(dataTS);
    }
}

void BluetoothServer::readSocket() {
    //qDebug() << "Got here.";
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket) {
        return;
    }

    //qDebug() << socket->readLine();

    /*while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug() << line;
        emit dataRecieved(socket->peerName(), QString::fromUtf8(line.constData(), line.length()));
    }*/

    QByteArray data =  socket->readLine();
    //qDebug() << data;
    std::string fData = data.toStdString();
    QString processedData = QString::fromStdString(fData);
    QString protocol = processedData.mid(2, 5);

    /*
        Protocols:-
        [BRK] := Break Connection Protocol.
        [ICP] := Incomming Call Protocol.
        [ISP] := Incomming Sms Protocol.
        [IGP] := Incomming Geo Location Protocol.
    */

    if (protocol == "[BRK]") {
        // Signal shit boi
        emit clientDisconnected();
        //emit dataRecieved(socket->peerName(), processedData.mid(9, processedData.length()));
    } else if (protocol == "[ICP]") {
        emit dataRecieved(protocol, socket->peerName(), processedData.mid(9, processedData.length()));
    } else if (protocol == "[ISP]") {
        // will update for SMS here
        emit dataRecieved(protocol, socket->peerName(), processedData.mid(9, processedData.length()));
    } else if (protocol == "[IGP]") {
        emit dataRecieved(protocol, socket->peerName(), processedData.mid(9, processedData.length()));
    }
}

BluetoothServer::~BluetoothServer() {
    stopBTServer();
}
