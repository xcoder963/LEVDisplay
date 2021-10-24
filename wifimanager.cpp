#include "wifimanager.h"

WifiManager::WifiManager() {

}

QList<QString> WifiManager::getAllWifiDevices() {
    QNetworkConfigurationManager ncm;
    netcfgList = ncm.allConfigurations();
    wifiList.clear();

    for (auto &x: netcfgList) {
        if (x.bearerType() == QNetworkConfiguration::BearerWLAN) {
            if (x.name() == "") {
                wifiList << "Unknown(Other Network)";
            } else {
                wifiList << x.name();
            }
            //qDebug() << x.type();
        }
    }
    return wifiList;
}

void WifiManager::connectToWifiDevice(QString &ssid, QString &password, QString &passOption) {
    std::string passString = "nmcli d wifi connect \"" + ssid.toStdString() + "\"";
    if (passOption != "None") {
        passString += " password " + password.toStdString();
    }
    qDebug() << QString::fromStdString(passString);
    system(passString.c_str());
}
