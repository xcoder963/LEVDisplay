#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QDebug>
#include <QBluetoothServer>

class BluetoothManager: public  QObject {
public:
    BluetoothManager();
    ~BluetoothManager();
    void connectToADevice(QBluetoothLocalDevice &lDevice, const QBluetoothAddress &address);
    void connectToDeviceTemp(QString &address);
private:
    QBluetoothAddress *connectedDevice;
};

class BluetoothServer: public QObject {

    Q_OBJECT

public:
    explicit BluetoothServer(QObject *parent = nullptr);
    ~BluetoothServer();
    void startBTServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());
    void stopBTServer();

public slots:
    void sendData(const QString &data);

signals:
    void dataRecieved(const QString &type, const QString &sender, const QString &data);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

private:
    QBluetoothServer *rfcommServer = nullptr;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
};

class LocalBluetoothDevice: public QObject {
    Q_OBJECT
public:
    LocalBluetoothDevice();
    //~LocalBluetoothDevice();
    QString getLocalBTDeviceName();
    QList<QBluetoothAddress> getConnectedDevices();
    void startDeviceDiscovery();
    QList<QString> getAllScannedDevices();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

private:
    QBluetoothLocalDevice localDevice;
    QString localDeviceName;
    QList<QBluetoothAddress> remoteDevices;
    QList<QString> envDevices;
};

#endif // BLUETOOTHMANAGER_H
