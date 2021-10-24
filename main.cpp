#include "mainwindow.h"

#include <QApplication>
//#include "BluetoothManager.h"
#include <QDebug>
#include "wifimanager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(900, 600);
    //LocalBluetoothDevice btDev;
    //qDebug() << btDev.getLocalBTDeviceName();
    //btDev.startDeviceDiscovery();
    //WifiManager wtDev;
    //qDebug() << wtDev.getAllWifiDevices();
    w.show();
    return a.exec();
}
