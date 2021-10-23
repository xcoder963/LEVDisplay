#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include "BluetoothManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    template<class T>
    void applyShadow(T Arr, qint16 radius);
    void applyIcons();

    ~MainWindow();

private slots:
    void on_bluetooth_button_clicked();
    void on_wifi_button_clicked();
    void on_map_mini_clicked();
    void on_menuButton_clicked();
    void on_mode_Btn_clicked();
    void on_mShutDownBtn_clicked();
    void on_bt_refresh_btn_clicked();

private:
    Ui::MainWindow *ui;
    LocalBluetoothDevice lBte;
};
#endif // MAINWINDOW_H
