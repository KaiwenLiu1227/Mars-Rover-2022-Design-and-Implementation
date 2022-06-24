
#include "comms.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/control");
AsyncWebSocketClient *MonitorClient = nullptr;
AsyncWebSocketClient *GamepadClient = nullptr;
void notifyOK()
{
  ws.textAll("OK");
}

void MonitorSend(String msg)
{
  if (MonitorClient == nullptr)
  {
    // SERIAL_USED.println("Monitor not available!");
    return;
  }
  ws.text(MonitorClient->id(), msg);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    Serial.print("rev:");
    for(int i=0;i<len;i++){
      Serial.print(" ");
      Serial.print(data[i]);
    }
     Serial.println();
    data[len] = 0;
    if (data[0] != '$')
    {
      ws.text(client->id(), "Header fault!");
      return;
    }

    // handler logic
    if (data[1] == 0)
    { // register device
      if (data[2] == 0)
      {
        MonitorClient = client;
        // SERIAL_USED.print("Monitor client: #");
        // SERIAL_USED.println(client->id());
        ws.text(client->id(), "OK");
      }
      else if (data[2] == 1)
      {
        GamepadClient = client;
        // SERIAL_USED.print("Gamepad client: #");
        // SERIAL_USED.println(client->id());
        ws.text(client->id(), "OK");
      }
      else if (data[2] == 2)
      {
        GamepadClient = nullptr;
        ws.text(client->id(), "OK");
      }
    }
    else if (data[1] == 1)
    { // motor value write
      if ((data[2] & 1) == 1)
      {
        MotorOneForwarding = true;
      }
      else
      {
        MotorOneForwarding = false;
      }
      if ((data[2] & 2) == 2)
      {
        MotorTwoForwarding = true;
      }
      else
      {
        MotorTwoForwarding = false;
      }
      MotorOneSpeed = data[3];
      MotorTwoSpeed = data[4];
      ws.text(client->id(), "OK");
    }
    else if (data[1] == 2)
    {
      // controller yaw
      if (data[2] == 0)
      {
        if (data[3] == 1)
        {
          TargetYaw = (double)data[4];
        }
        else if (data[3] == 0)
        {
          TargetYaw = -(double)data[4];
        }
        else if (data[3] == 2)
        {
          TargetYaw += (double)data[4];
        }
        else if (data[3] == 3)
        {
          TargetYaw -= (double)data[4];
        }
          else if (data[3] == 4)
        {
          OpticalFlowXOffset += (double)data[4]*0.5;
        }
        else if (data[3] == 5)
        {
          OpticalFlowXOffset -= (double)data[4]*0.5;
        }
        ws.text(client->id(), "OK");
      }
      // controller pos
      else if (data[2] == 1)
      {
        if (data[3] == 1)
        {
          TargetPosX = (double)data[4];
        }
        else
        {
          TargetPosX = -(double)data[4];
        }
        if (data[5] == 1)
        {
          TargetPosY = (double)data[6];
        }
        else
        {
          TargetPosY = -(double)data[6];
        }
        ws.text(client->id(), "OK");
      }
      // clear current state
      else if (data[2] == 2)
      {
        TargetPosX = 0;
        TargetPosY = 0;
        MotorOneSpeed = 0;
        MotorTwoSpeed = 0;
        total_x1 = 0;
        total_y1 = 0;
        OpticalFlowPosX = 0;
        OpticalFlowPosY = 0;
        RealPosX = 0;
        RealPosY = 0;
        RealVelocityX = 0;
        RealVelocityY = 0;
        YawAngle = 0;
        OpticalFlowYawAngle = 0;
        OpticalFlowSpeedX = 0;
        OpticalFlowSpeedY = 0;
        TargetYaw, CurrentYaw, YawControl = 0;
        TargetVelocity, CurrentVelocity, VelocityControl = 0;
        CurrentDistance, DistanceControl = 0;
        CurrentPosX, CurrentPosY, PosXControl, PosYControl = 0;
        TargetPosX, TargetPosY = 0;
        TargetVelocityX, TargetVelocityY;
        YawDiff = 0;
      }
      // set control mode
      else if (data[2] == 3)
      {
        if (data[3] <= 2)
        {
          ControlMode = data[3];
          ws.text(client->id(), "OK");
        }
        else
        {
          ws.text(client->id(), "Unknown Mode");
        }
      }
      // add obs
      else if (data[2] == 4)
      {
        float obs_x,obs_y;
        if (data[3] == 1)
        {
          obs_x = (float)data[4];
        }
        else if (data[3] == 0)
        {
          obs_x = -(float)data[4];
        }
          if (data[5] == 1)
        {
          obs_y = (float)data[6];
        }
        else if (data[5] == 0)
        {
          obs_y = -(float)data[6];
        }
        if(data[7]==1)
          ForceAddObstacle(obs_x,obs_y);
        else
          AddObstacle(obs_x,obs_y);
      }
    }
    else
    {
      ws.text(client->id(), "Unknown command");
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    // SERIAL_USED.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    // SERIAL_USED.printf("WebSocket client #%u disconnected\n", client->id());
    if (MonitorClient != nullptr && MonitorClient->id() == client->id())
    {
      MonitorClient = nullptr;
    }
    if (GamepadClient != nullptr && GamepadClient->id() == client->id())
    {
      GamepadClient = nullptr;
    }
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len, client);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void InitWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String &var)
{
  return "html";
}

void InitComms()
{
  // Connect to Wi-Fi
  WiFi.softAP("ESP_Mars");

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   SERIAL_USED.println("Connecting to WiFi..");
  // }

  // Print ESP Local IP Address
  // SERIAL_USED.print("Wifi local ip:");
  // SERIAL_USED.println(WiFi.localIP());

  InitWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", "hello esp32!", processor);
  });

  // Start server
  server.begin();
}

void CleanUpClients()
{
  ws.cleanupClients();
}