#include "global.h"
#include "driver/motor.h"
#include "driver/optical_flow.h"
#include "driver/mpu6050.h"
#include "comms/comms.h"
#include "control/control.h"
#include "utils/utils.h"
#include "iostream"
#include "control/obstacle_avoidance.h"
#include "control/obstacle_detect.h"
#include <math.h>
xTaskHandle ControlHandler, FeedbackHandler, SensorHandler, DriveHandler;
long MotorControlAmount = 0;
void DriveTask(void *parameter)
{
  while (1)
  {
    MotorControlAmount += MotorOneControl - MotorTwoControl;
    DriveMotors();
    
    vTaskDelay(50);
  }
  vTaskDelete(NULL);
}

void DebugTask(void *parameter)
{
  vTaskDelete(NULL);
  while (1)
  {
    // if (MotorOneSpeed + MotorTwoSpeed == 0)
    // {
    //   digitalWrite(PIN_DEBUG_LED, HIGH);
    //   vTaskDelay(200);
    //   digitalWrite(PIN_DEBUG_LED, LOW);
    //   vTaskDelay(200);
    // }
    // else
    // {
    //   digitalWrite(PIN_DEBUG_LED, HIGH);
    //   vTaskDelay(100);
    // }
  }
  vTaskDelete(NULL);
}
#define OBS_SERIAL Serial
int RevBuffer[100];
int RevPointer = 0;
int MessageLength = 0;
bool StartRev = false;
bool MessageReceived = false;
void Serial2Task(void *parameter)
{
  while (1)
  {
    while (Serial2.available())
    {
      int inByte = Serial2.read();
      Serial.write(inByte);
    }
    while (Serial.available())
    {
      int inByte = Serial.read();
      if (!StartRev)
      {
        if ((char)inByte == '$')
        {
          StartRev = true;
        }
      }
      else
      {
        if (inByte == '#')
        {
          MessageLength = RevPointer;
          RevPointer = 0;
          StartRev = false;
          MessageReceived = true;
        }
        else if (RevPointer < 100)
        {
          RevBuffer[RevPointer] = inByte;
          RevPointer += 1;
        }
        else
        {
          RevPointer = 0;
          MessageLength = 0;
          MessageReceived = false;
          StartRev = false;
        }
      }
      if (MessageReceived)
      {
        MessageReceived = false;
        String msg = "$cam:";
        int word_state = 0;
        long word = 0;
        double distance_from_raw;
        for (int i = 0; i < MessageLength; i++)
        {
          word += RevBuffer[i] << (24 - 8 * word_state);
          msg += String(RevBuffer[i], HEX) + " ";
          word_state++;
          if (word_state == 4)
          {
            //Serial2.printf("%08x\n", word);

            unsigned int color = (word >> 24) & 0xFF;
            unsigned int message_type = (word >> 23) & 0x1;
            unsigned int distance_raw = word & 0xFFFFF;

            int center_x = (word >> 11) & 0x03FF;
            int center_y = word & 0x03FF;
            center_x -= 320;

            if (message_type == 0)
            {
              distance_from_raw = 3.909e6 / (distance_raw + 2.436e4);

              if (color == 51)
              {
                if (color == 0)
                {
                  OBS_SERIAL.printf("black:");
                }
                else if (color == 17)
                {
                  OBS_SERIAL.printf("white:");
                }
                else if (color == 34)
                {
                  OBS_SERIAL.printf("red:");
                }
                else if (color == 51)
                {
                  OBS_SERIAL.printf("blue:");
                }
                else if (color == 68)
                {
                  OBS_SERIAL.printf("yellow:");
                }
                else if (color == 85)
                {
                  OBS_SERIAL.printf("purple:");
                }
                else
                {
                  OBS_SERIAL.printf("unknown:");
                }
                OBS_SERIAL.printf("c:%u dis:%.2f ", color, distance_from_raw);
              }
            }

            else if (message_type == 1)
            {
              double distance_from_y = (-9.292 * ((double)center_y) + 6355.0) / ((double)center_y - 189.6);
              double distance_x = -5 * distance_from_raw * center_x / (1455.15 * 3);
              if (color == 51)
              {
                OBS_SERIAL.printf(" center:%d %d dis:%.2f %.2f\n", center_x, center_y, distance_from_y, distance_x);
                OBS_SERIAL.println();
                ObstacleDetect(center_x, center_y, distance_from_raw, distance_x, color);
              }
            }
            word_state = 0;
            word = 0;
            // OBS_SERIAL.println();
          }
        }
        // MonitorSend(msg);
      }
    }
    vTaskDelay(10);
    // OBS_SERIAL.write(inByte);
  }
}
int IsControlling = 0;
bool BeepControl = false;

void FeedBackTask(void *parameter)
{
  int data = 0;

  while (1)
  {
    while (MonitorClient == nullptr)
    {
      vTaskDelay(10);
      BeepControl = true;
      // wait for monitor
    }
    if (BeepControl)
    {
      for (int i = 0; i < 2; i++)
      {
        digitalWrite(PIN_BEEP, HIGH);
        vTaskDelay(100);
        digitalWrite(PIN_BEEP, LOW);
        vTaskDelay(50);
      }
      BeepControl = false;
    }

    data += 1;
    String msg = "$opt:";
    msg += String(OpticalFlowPosX);
    msg += " ";
    msg += String(OpticalFlowPosY);
    msg += ",real:";
    msg += String(RealPosX);
    msg += " ";
    msg += String(RealPosY);
    msg += ",yaw:";
    msg += String(YawAngle);
    msg += ",ctrl:";
    msg += String(abs(MotorTwoControl)+abs(MotorOneControl));
    msg += ",qual:";
    msg += String(OpticalFlowQuality);
    msg += ",mpu:";
    msg += String(MPUYawAngle);
    msg += ",motor:";
    msg += String(MotorControlAmount);
    msg += ",obs_cnt:";
    msg += String(obs_cnt);
    msg += ",x_off:";
    msg += String(OpticalFlowXOffset);
    MonitorSend(msg);
    // Serial.println(msg);
    vTaskDelay(150);
  }
  vTaskDelete(NULL);
}

float K2 = 0.3;
float K1 = 0;
float currentYawRadian = 0;

void SensorTask(void *parameter)
{
  while (1)
  {
    UpdateOpticalFlow();
    CurrentYaw = K1 * CurrentYaw + (1 - K1) * OpticalFlowYawAngle;
    currentYawRadian = CurrentYaw * MATH_PI / 180;
    if(OpticalFlowSpeedY!=0){
    RealVelocityX = sin(currentYawRadian) * OpticalFlowSpeedY / 157;
    RealVelocityY = cos(currentYawRadian) * OpticalFlowSpeedY / 157;
    RealPosX += RealVelocityX;
    RealPosY += RealVelocityY;
    }

    vTaskDelay(5);
  }
  vTaskDelete(NULL);
}

void MPUTask(void *parameter)
{
  while (1)
  {
    UpdateMPU();
    SERIAL_USED.print("#");
    CurrentYaw = K2 * CurrentYaw + (1 - K2) * (CurrentYaw + MPUYawAngleIncrement);
    MPUYawAngleIncrement = 0;
    vTaskDelay(15);
  }
  vTaskDelete(NULL);
}

#define MAX_STATE 16
float YawTurningThreshold = 30;
float YawFIR = 0.5;
double coe = 0;
float TargetDistance;
float RawYawDiff = 0;
float DOAYawDiff = 0;

void ControlTask(void *parameter)
{
  int state = 0;
  while (1)
  {
    state += 1;
    // 1 cycle

    // SERIAL_USED.print(" x:");
    // SERIAL_USED.print(RealPosX);
    // SERIAL_USED.print(" y:");
    // SERIAL_USED.print(RealPosY);

    // position control
    TargetDistance = GetDistance(TargetPosX, TargetPosY, RealPosX, RealPosY);
    if (true)
    {
      CurrentPosX = RealPosX;
      CurrentPosY = RealPosY;
      PosXController.Compute();
      PosYController.Compute();
    }

    // velocity control
    if (true)
    {
      float _TargetYaw = 0;
      float XDiff = PosYControl - RealVelocityX;
      float YDiff = PosXControl - RealVelocityY;
      TargetVelocity = XDiff * XDiff + YDiff * YDiff;
      if (TargetVelocity != 0)
        TargetVelocity = sqrt(TargetVelocity);
      VelocityController.Compute();

      float atanVal;
      if (abs(XDiff) <= 0.000001)
      {
        XDiff = MathSign(XDiff) * 0.000001;
      }
      if (abs(YDiff) <= 0.000001)
      {
        YDiff = MathSign(YDiff) * 0.000001;
      }
      if (XDiff == 0 && YDiff == 0)
        atanVal = 0;
      else
        atanVal = atan(YDiff / XDiff) * 180 / MATH_PI;
      if (XDiff < 0 & YDiff < 0)
        _TargetYaw = atanVal - 180;
      else if (XDiff<0 & YDiff> 0)
        _TargetYaw = atanVal + 180;
      else
        _TargetYaw = atanVal;
      // if (XDiff >= 0)
      //   distanceSign = 1;
      // else
      //   distanceSign = -1;

      // if (XDiff > 0)
      //   _TargetYaw = atanVal;
      // else
      //   _TargetYaw = atanVal + 180 * MathSign(YDiff);
      if (TargetVelocity >= 6 && TargetDistance > 4)
      {
        TargetYaw = (double)_TargetYaw;
      }
      RawYawDiff = GetAngleDiff(TargetYaw, CurrentYaw);
    }
    if (true)
      DOAYawDiff = ObstacleAvoiding(RealPosY, RealPosX, CurrentYaw);
    if (TargetVelocity >= 6 && TargetDistance > 4)
    {
      RawYawDiff += DOAYawDiff;
    }

    YawDiff = YawFIR * YawDiff + (1 - YawFIR) * RawYawDiff;
    // SERIAL_USED.print(" yaw_diff:");
    // SERIAL_USED.print(YawDiff);
    // SERIAL_USED.print(" ryaw_diff:");
    // SERIAL_USED.println(RawYawDiff);
    double abs_YawDiff = abs(YawDiff);
    coe = 1;
    if (abs_YawDiff > YawTurningThreshold)
      coe = 0;
    else
      coe = -1 * pow((YawDiff / YawTurningThreshold), 2) + 1;
    // yaw control
    YawController.Compute();

    if (ControlMode < MANUAL_CONTROLLER)
    {
      int distanceControlEnable = ControlMode == AUTO_CONTROLLER ? 1 : 0;
      if (GamepadClient == nullptr)
      {
        if (TargetVelocity >= 2)
        {
          MotorOneControl = (int)YawControl + distanceControlEnable * (coe * VelocityControl);
          MotorTwoControl = -(int)YawControl + distanceControlEnable * (coe * VelocityControl);
        }
        else
        {
          MotorOneControl = (int)YawControl;
          MotorTwoControl = -(int)YawControl;
        }
      }
      else
      {
        MotorOneControl = 0;
        MotorTwoControl = 0;
      }
    }
    else
    {
      MotorOneControl = 0;
      MotorTwoControl = 0;
    }
    if (MotorTwoControl == 0 && MotorOneControl == 0)
    {
      IsControlling = 0;
    }
    else
    {
      IsControlling = 1;
    }
    if (state >= MAX_STATE)
    {
      state = 0;
    }
    vTaskDelay(50);
  }
  vTaskDelete(NULL);
}

void InitSystem()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  // pinMode(PIN_DEBUG_LED, OUTPUT);
  pinMode(PIN_BEEP, OUTPUT);

  //   ledcSetup(MOTORA_PWM_CHANNEL, 5000, 13);
  //   ledcAttachPin(PIN_MOTOR_PWMA, MOTORA_PWM_CHANNEL);
  //   ledcSetup(MOTORB_PWM_CHANNEL, 5000, 13);
  //   ledcAttachPin(PIN_MOTOR_PWMB, MOTORB_PWM_CHANNEL);

  InitOpticalFlow();
  // InitMPU6050();
  InitComms();
  InitController();
  // MPUTest();

  SERIAL_USED.println("System Init success");
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(PIN_BEEP, HIGH);
    vTaskDelay(100);
    digitalWrite(PIN_BEEP, LOW);
    vTaskDelay(100);
  }
  //  TestOpticalFlow();
}
void _ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255)
{
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // 向 LEDC 写入任务
  ledcWrite(channel, duty);
}
void setup()
{
  InitSystem();

  xTaskCreate(Serial2Task, "Serial2Task", 8192, NULL, 2, NULL);

  xTaskCreate(FeedBackTask, "FeedBackTask", 4096, NULL, 0, &FeedbackHandler);

  xTaskCreate(SensorTask, "SensorTask", 4096, NULL, 4, &SensorHandler);
  // xTaskCreate(MPUTask, "SensorTask", 4096, NULL, 4, NULL);
  xTaskCreate(ControlTask, "ControlTask", 4096, NULL, 2, &ControlHandler);
  xTaskCreate(
      DriveTask,      /*任务函数*/
      "DriveTask",    /*带任务名称的字符串*/
      2048,           /*堆栈大小，单位为字节*/
      NULL,           /*作为任务输入传递的参数*/
      0,              /*任务的优先级*/
      &DriveHandler); /*任务句柄*/
}

void loop()
{
  ws.cleanupClients();
  delay(10);
  //   analogWrite(PIN_MOTOR_PWMB,150);
  // analogWrite(PIN_MOTOR_PWMA,150);
  // ledcAnalogWrite(0, 150);
  // ledcAnalogWrite(1, 150);
}

// #include "Arduino.h"

// void setup()
// {
//     Serial.begin(9600);
//     Serial2.begin(9600);
// }
// void loop()
// {
//     while (Serial.available())
//     {
//         int inByte = Serial.read();
//         Serial2.write(inByte);
//     }

//     while (Serial2.available())
//     {
//         int inByte = Serial2.read();
//         Serial.write(inByte);
//     }
// }

