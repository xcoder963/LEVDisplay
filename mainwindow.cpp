#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    //just some background stuff will remove this later
    QString backColor = "background-color: rgb(255, 255, 255);";
    ui->main->setStyleSheet(backColor);
    ui->bluetooth_page->setStyleSheet(backColor);
    ui->wifi_page->setStyleSheet(backColor);
    ui->map_page->setStyleSheet(backColor);
    ui->menu->setStyleSheet(backColor);

    auto UI_ARR_BUTTONS_1 = { ui->S_button, ui->R_button, ui->P_button, ui->E_button,
                        ui->reverse_button, ui->leftIndicator_button,
                        ui->rightIndicator_button, ui->leftIndicator_button_mp,
                        ui->rightIndicator_button_mp };

    auto UI_ARR_FRAMES = { ui->notification_frame, ui->menu_frame,
                            ui->bluetooth_page_frame, ui->wifi_page_frame, ui->status_frame };

    auto UI_ARR_BUTTONS_2 = { ui->map_mini, ui->menuButton, ui->bluetooth_button, ui->wifi_button };

    MainWindow::applyShadow(UI_ARR_BUTTONS_1, 10);
    MainWindow::applyShadow(UI_ARR_BUTTONS_2, 15);
    MainWindow::applyShadow(UI_ARR_FRAMES, 15);
    //MainWindow::applyShadow(UI_WEB, 15);

    MainWindow::applyIcons();

    QString someExtraShit[] = {"border: 1.5px solid black; border-radius: 8px;\nbackground-color: rgb(255, 255, 255);\ncolor: rgb(57, 57, 57);",
                               "QProgressBar {border: 1px solid black;background-color: rgb(255, 255, 255);border-radius: 5px;color: white;}\nQProgressBar::chunk {background-color: green;border-radius: 5px;}"};

    ui->br_in->setStyleSheet(someExtraShit[0]);
    ui->br_de->setStyleSheet(someExtraShit[0]);
    ui->battery_progressBar->setStyleSheet(someExtraShit[1]);
    ui->battery_another->setStyleSheet(someExtraShit[1]);

    //ui->map_frame->setUrl(QUrl("/home/xcoder963/Projects/perm/LEV/build-LEVScreen-Desktop-Debug/mapResources/index.html"));
}

template <class T>
void MainWindow::applyShadow(T Arr, qint16 radius) {
    for(auto UI_OBJ:  Arr) {
        //I will memory manage this shit later
        auto *effect = new QGraphicsDropShadowEffect;
        effect->setBlurRadius(radius);
        effect->setOffset(0, 0);
        UI_OBJ->setGraphicsEffect(effect);
    }
}

void MainWindow::applyIcons() {
    //This is ugly as well
    ui->bluetooth_button->setIcon(QIcon("images/bluetooth.png"));
    ui->bluetooth_button->setIconSize(QSize(30, 30));
    ui->wifi_button->setIcon(QIcon("images/wifi.png"));
    ui->wifi_button->setIconSize(QSize(30, 30));
    ui->menuButton->setIcon(QIcon("images/menu.png"));
    ui->menuButton->setIconSize(QSize(40, 40));
    ui->mode_Btn->setIcon(QIcon("images/mode.png"));
    ui->mode_Btn->setIconSize(QSize(50, 50));
    ui->mShutDownBtn->setIcon(QIcon("images/shutdown.png"));
    ui->mShutDownBtn->setIconSize(QSize(50, 50));
    ui->map_mini->setIcon(QIcon("images/map_mini.jpg"));
    ui->map_mini->setIconSize(QSize(240, 130));
    ui->indicator_img_button->setIcon(QIcon("images/indicator.png"));
    ui->indicator_img_button->setIconSize(QSize(60, 60));
    ui->indicator_img_button_mp->setIcon(QIcon("images/indicator.png"));
    ui->indicator_img_button_mp->setIconSize(QSize(60, 60));
    ui->motorOn->setIcon(QIcon("images/motor_on.png"));
    ui->motorOn->setIconSize(QSize(30, 30));
}

MainWindow::~MainWindow() {
    delete ui;
}

//useless redirection
void MainWindow::on_bluetooth_button_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_wifi_button_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_map_mini_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_menuButton_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_mode_Btn_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_mShutDownBtn_clicked() { QCoreApplication::quit(); }


void MainWindow::on_bt_refresh_btn_clicked() {
    ui->bluetooth_list->clear();
    //ui->bluetooth_list->repaint();
    lBte.startDeviceDiscovery();
    QList<QString> scannedDeviecs = lBte.getAllScannedDevices();
    for (int i = 0; i < scannedDeviecs.size(); i++) {
        ui->bluetooth_list->addItem(scannedDeviecs.at(i));
    }
    //ui->bluetooth_list->repaint();
}


void MainWindow::on_wifi_refresh_btn_clicked() {
    ui->wifi_list->clear();
    QList<QString> scannedDevices = lWte.getAllWifiDevices();
    for (int i = 0; i < scannedDevices.size(); i++) {
        ui->wifi_list->addItem(scannedDevices.at(i));
    }
}


void MainWindow::on_bt_connect_btn_clicked() {
    QString deviceAddr = ui->bluetooth_list->currentItem()->text();
    QStringList deviceAddrArr = deviceAddr.split(" ");
    deviceAddr = deviceAddrArr.at(deviceAddrArr.size() - 1);
    qDebug() << deviceAddr;
    bteMan.connectToDeviceTemp(deviceAddr);
}


void MainWindow::on_wifi_connect_btn_clicked() {
    QString deviceAddr = ui->wifi_list->currentItem()->text();
    QString passOption = "";
    QString password = ui->wifi_password->text();
    if (password == "") {
        qDebug() << "[NOTICE]: No Password.";
        passOption = "None";
    }
    lWte.connectToWifiDevice(deviceAddr, password, passOption);
}

