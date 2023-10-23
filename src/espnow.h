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
    String R1Name;
    String R2Name;
    String R3Name;
    String R4Name;
    String R5Name;
    String R6Name;
    String R7Name;
    String R8Name;
    int R1_state;
    int R2_state;
    int R3_state;
    int R4_state;
    int R5_state;
    int R6_state;
    int R7_state;
    int R8_state;
};

extern struct rValues relayValues;
#endif