#ifndef ESPNOW_H
#define ESPNOW_H
#include <esp_now.h>

class espNow
{
public:
    espNow(){};
    esp_now_peer_info_t slaveInfo;
    void espNowInit();
    void sendData();
};

struct rValues
{
    int R1;
    int R2;
    int R3;
    int R4;
};

extern struct rValues relayValues;
#endif