#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include "BluetoothManager.h"
#include "wifimanager.h"
#include <thread>
#include <future>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    template<class T>
    void applyShadow(T &Arr, qint16 radius);
    void applyIcons();
    void startServer();

    ~MainWindow();

public slots:
    void clientConnected(const QString &devName);
    void clientDisconnected(const QString &devName);
    //void clientDisconnected();
    //void connected(const QString &devName);
    void showRecivedData(const QString &type, const QString &senderName, const QString &data);
    //void reactOnSocketError(const QString &error);

private slots:
    void on_bluetooth_button_clicked();
    void on_wifi_button_clicked();
    void on_map_mini_clicked();
    void on_menuButton_clicked();
    void on_mode_Btn_clicked();
    void on_mShutDownBtn_clicked();
    void on_bt_refresh_btn_clicked();
    void on_wifi_refresh_btn_clicked();
    void on_bt_connect_btn_clicked();
    void on_wifi_connect_btn_clicked();
    void on_E_button_clicked();
    void on_P_button_clicked();
    void on_S_button_clicked();
    void on_R_button_clicked();
    void on_btServerOn_clicked();
    void on_btServerOf_clicked();
    void on_mDocumentBtn_clicked();
    void on_mErrorsBtn_clicked();
    void on_loginBtn_clicked();

private:
    Ui::MainWindow *ui;
    LocalBluetoothDevice lBte;
    BluetoothManager bteMan;
    WifiManager lWte;
    BluetoothServer *btServer;
};
#endif // MAINWINDOW_H
