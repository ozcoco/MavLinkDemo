//
// Created by Administrator on 2020/1/17.
//

#include "event_utils.h"
#include <jni.h>
#include <cstring>
#include "utils/log.h"
#include <string>

extern "C"
{
#include <curl/curl.h>
#include <curl/easy.h>
}


static size_t writer(char *data, size_t size, size_t nmemb, void *writer_data) {

    const size_t &sizes = size * nmemb;

    if (nullptr == writer_data) {
        return 0;
    }

    auto *buf = static_cast<std::string *>(writer_data);

    buf->append(data, sizes);

    return sizes;
}

static size_t writer2(char *data, size_t size, size_t nmemb, void *writer_data) {

    const size_t &sizes = size * nmemb;

    LOGI("get data ----> %s", data);

    if (nullptr == writer_data) {
        return 0;
    }

    auto *buf = static_cast<std::string *>(writer_data);

    buf->append(data, sizes);

    return sizes;
}

static void init_curl(CURL *curl, std::string *pWriteDataBuf) {

    CURLcode code;

    code = curl_global_init(CURL_GLOBAL_ALL);

    if (code != CURLE_OK) {

        LOGE("%s", "Failed to set redirect option");

        return;
    }

    curl = curl_easy_init();

    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    if (code != CURLE_OK) {

        LOGE("%s", "Failed to set FOLLOW LOCATION");

        return;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

    if (code != CURLE_OK) {

        LOGE("%s", "Failed to set FOLLOW LOCATION");

        return;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, pWriteDataBuf);


    curl_easy_setopt(curl, CURLOPT_HEADER, 1);

    if (code != CURLE_OK) {

        LOGE("%s", "Failed to set FOLLOW LOCATION");

        return;
    }


}


static void Get(const char *url, std::string *dataBuf) {

    CURL *curl;

    init_curl(curl, dataBuf);

    if (!curl)
        goto exit;

    CURLcode code;

    code = curl_easy_setopt(curl, CURLOPT_URL, url);

    if (code == CURLE_OK) {
        LOGE("%s", "Failed to set URL");
        goto exit;
    }

    code = curl_easy_perform(curl);

    if (code == CURLE_OK) {
        LOGE("%s", "Failed to perform");
        goto exit;
    }

    exit:
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}


std::string meteData{};

void test() {

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, "http://www.cnblogs.com/hibraincol/");
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com/");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer2);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &meteData);

        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        res = curl_easy_perform(curl);
        if (0 != res) {
            printf("curl error: %d\n", res);
            LOGE("curl error: %d -----> %s", res, "Failed to perform");
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


void Get2(const char *url, std::string &buf) {

    CURL *curl;

    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
//        curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com/");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer2);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);

        curl_easy_setopt(curl, CURLOPT_HEADER, 1);

        res = curl_easy_perform(curl);
        if (0 != res) {
            printf("curl error: %d\n", res);
            LOGE("curl error: %d -----> %s", res, "Failed to perform");
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


extern "C"
JNIEXPORT jstring JNICALL
Java_me_oz_demo_mavlink_net_NativeHttpClient_get(JNIEnv *env, jclass type, jstring _url) {

    jboolean isCopy;

    const char *url = env->GetStringUTFChars(_url, &isCopy);

    std::string dataBuf{};

    Get2(url, dataBuf);

//    test();

    env->ReleaseStringUTFChars(_url, url);

    return env->NewStringUTF(dataBuf.c_str());
}


