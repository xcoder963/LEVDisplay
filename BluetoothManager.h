#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QDebug>

class BluetoothManager {


};

class LocalBluetoothDevice: public QObject {
    Q_OBJECT
public:
    LocalBluetoothDevice();

    QString getLocalBTDeviceName();
    QList<QBluetoothAddress> getConnectedDevices();
    void startDeviceDiscovery();
    QList<QString> getAllScannedDevices();

    ~LocalBluetoothDevice();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

private:
    QBluetoothLocalDevice localDevice;
    QString localDeviceName;
    QList<QBluetoothAddress> remoteDevices;
    QList<QString> envDevices;
};

#endif // BLUETOOTHMANAGER_H
