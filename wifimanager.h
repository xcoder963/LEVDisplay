#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QList>

class WifiManager
{
public:
    WifiManager();
    QList<QString> getAllWifiDevices();
    void connectToWifiDevice(QString &ssid, QString &password, QString &passOption);
private:
    int foundDevicesCOunt;
    QNetworkConfiguration netcfg;
    QList<QString> wifiList;
    QList<QNetworkConfiguration> netcfgList;
    QList<QString> wifiDevices;
};

#endif // WIFIMANAGER_H
