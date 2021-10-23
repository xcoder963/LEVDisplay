#include "BluetoothManager.h"

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

LocalBluetoothDevice::~LocalBluetoothDevice() {

}
