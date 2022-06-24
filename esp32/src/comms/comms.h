#ifndef MOTOR_H
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "global.h"
#include "../driver/motor.h"
#include "../control/control.h"
#include "../driver/optical_flow.h"
#include "../control/obstacle_avoidance.h"
extern AsyncWebServer server;
extern AsyncWebSocket ws;
extern AsyncWebSocketClient * MonitorClient;
extern AsyncWebSocketClient * GamepadClient;
void InitComms();
void InitWebSocket();
void MonitorSend(String msg);

#endif