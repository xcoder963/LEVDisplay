#include "webinteractions.h"

webInteractions::webInteractions() {

}

static size_t WriteCallBack(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t WriteCallBackFF(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

QString webInteractions::getImageData() {
    std::string readBuffer;
    QString retData;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        retData = QString::fromStdString(readBuffer);
        qDebug() << retData;
    }

    return retData;
}

bool webInteractions::downloadFile(QString fileName, QString fileUrl) {
    curl = curl_easy_init();
    if (curl) {
        filePtr = fopen(fileName.toStdString().c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, fileUrl.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileUrl.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, filePtr);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(filePtr);
    } else {
        return false;
    }

    return true;
}

QString webInteractions::loginCheck(QString email, QString password) {
    QString retVal = NULL;
    std::string readBuffer;
    QString url = "";
    QString postFields = "?uname="+email+"&password="+password;;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_HSTSWRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        retVal = QString::fromStdString(readBuffer);
        qDebug() << retVal;
    }

    return retVal;
}

