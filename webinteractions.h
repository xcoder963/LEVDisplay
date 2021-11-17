#ifndef WEBINTERACTIONS_H
#define WEBINTERACTIONS_H

#include <curl/curl.h>
#include <iostream>
#include <QString>
#include <QDebug>

class webInteractions {
public:
    webInteractions();
    QString getImageData();
    bool downloadFile(QString fileName, QString fileUrl);
    QString loginCheck(QString email, QString password);
private:
    CURL *curl;
    CURLcode res;
    FILE *filePtr;
};

#endif // WEBINTERACTIONS_H
