#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    //ui->mainStackPane->setCurrentIndex(1);

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
    ui->btServerOn->setStyleSheet(someExtraShit[0]);
    ui->btServerOf->setStyleSheet(someExtraShit[0]);
    ui->battery_progressBar->setStyleSheet(someExtraShit[1]);
    ui->battery_another->setStyleSheet(someExtraShit[1]);
    ui->loginUserName->setStyleSheet("border: 1.5px solid black; border-radius: 8px; color: rgb(0, 0, 0);");
    ui->loginUserPassword->setStyleSheet("border: 1.5px solid black; border-radius: 8px; color: rgb(0, 0, 0);");

    // This is temprory
    ui->btServerOn->setDisabled(true);

    //ui->map_frame->setUrl(QUrl("/home/xcoder963/Projects/perm/LEV/build-LEVScreen-Desktop-Debug/mapResources/index.html"));

    //std::thread serverThread(&MainWindow::startServer, this);
    //std::future<void> fut = std::async(&MainWindow::startServer, this);
    MainWindow::startServer();
}

void MainWindow::startServer() {
    btServer = new BluetoothServer(this);

    connect(btServer, QOverload<const QString &>::of(&BluetoothServer::clientConnected),
            this, &MainWindow::clientConnected);
    connect(btServer, QOverload<const QString &>::of(&BluetoothServer::clientDisconnected),
            this, &MainWindow::clientDisconnected);
    connect(btServer, &BluetoothServer::dataRecieved, this,
            &MainWindow::showRecivedData);

    btServer->startBTServer();
}

void MainWindow::clientConnected(const QString &devName) {
    qDebug() << "[NOTICE]: Got a connection from " + devName + ".";
}

void MainWindow::clientDisconnected(const QString &devName) {
    qDebug() << "[NOTICE]: Got a disconnection from " + devName + ".";
}

void MainWindow::showRecivedData(const QString &type, const QString &senderName, const QString &data) {
    qDebug() << "[NOTICE]: Got data := " + data + " from " + senderName + ".";
    if (type == "[ICP]" || type == "[ISP]") {
        ui->notification_list->addItem(data);
        ui->notification_list->repaint();
    } else if (type == "[IGP]"){
        ui->stackedWidget->setCurrentIndex(4);
    }
}

template <class T>
void MainWindow::applyShadow(T &Arr, qint16 radius) {
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
    ui->loginMainIcon->setIcon(QIcon("images/login.png"));
    ui->loginMainIcon->setIconSize(QSize(150, 100));
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
void MainWindow::on_loginBtn_clicked() { ui->mainStackPane->setCurrentIndex(1); }
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

// Boi is this ugly
void MainWindow::on_E_button_clicked() {
    ui->E_button->setGeometry(270, 420, 140, 60);
    ui->E_button->setText("Economy");
    ui->P_button->setGeometry(420, 420, 60, 60);
    ui->P_button->setText("P");
    ui->S_button->setGeometry(490, 420, 60, 60);
    ui->S_button->setText("S");
    ui->R_button->setGeometry(560, 420, 60, 60);
    ui->R_button->setText("R");
}


void MainWindow::on_P_button_clicked() {
    ui->E_button->setGeometry(270, 420, 60, 60);
    ui->E_button->setText("E");
    ui->P_button->setGeometry(340, 420, 140, 60);
    ui->P_button->setText("Power");
    ui->S_button->setGeometry(490, 420, 60, 60);
    ui->S_button->setText("S");
    ui->R_button->setGeometry(560, 420, 60, 60);
    ui->R_button->setText("R");
}


void MainWindow::on_S_button_clicked() {
    ui->E_button->setGeometry(270, 420, 60, 60);
    ui->E_button->setText("E");
    ui->P_button->setGeometry(340, 420, 60, 60);
    ui->P_button->setText("P");
    ui->S_button->setGeometry(410, 420, 140, 60);
    ui->S_button->setText("Sports");
    ui->R_button->setGeometry(560, 420, 60, 60);
    ui->R_button->setText("R");
}


void MainWindow::on_R_button_clicked() {
    ui->E_button->setGeometry(270, 420, 60, 60);
    ui->E_button->setText("E");
    ui->P_button->setGeometry(340, 420, 60, 60);
    ui->P_button->setText("P");
    ui->S_button->setGeometry(410, 420, 60, 60);
    ui->S_button->setText("S");
    ui->R_button->setGeometry(480, 420, 140, 60);
    ui->R_button->setText("Race");
}


void MainWindow::on_btServerOn_clicked() {
    ui->btServerOn->setDisabled(true);
    MainWindow::startServer();
    ui->btServerOf->setDisabled(false);
}


void MainWindow::on_btServerOf_clicked() {
    ui->btServerOf->setDisabled(true);
    delete btServer;
    ui->btServerOn->setDisabled(false);
}


void MainWindow::on_mDocumentBtn_clicked() {  }
void MainWindow::on_mErrorsBtn_clicked() { }
