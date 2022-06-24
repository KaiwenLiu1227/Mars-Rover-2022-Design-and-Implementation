// #include "global.h"
// #include "driver/motor.h"
// #include "driver/optical_flow.h"
// #include "driver/mpu6050.h"
// #include "comms/comms.h"
// #include "control/control.h"
// #include "utils/utils.h"
// #include "iostream"
// #include <math.h>
// xTaskHandle ControlHandler, FeedbackHandler, SensorHandler, DriveHandler;
// void DriveTask(void *parameter)
// {
//   while (1)
//   {
//     DriveMotors();
//     // vTaskDelay(50);
//     delay(50);
//   }
//   vTaskDelete(NULL);
// }

// void DebugTask(void *parameter)
// {
//   vTaskDelete(NULL);
//   while (1)
//   {
//     // if (MotorOneSpeed + MotorTwoSpeed == 0)
//     // {
//     //   digitalWrite(PIN_DEBUG_LED, HIGH);
//     //   vTaskDelay(200);
//     //   digitalWrite(PIN_DEBUG_LED, LOW);
//     //   vTaskDelay(200);
//     // }
//     // else
//     // {
//     //   digitalWrite(PIN_DEBUG_LED, HIGH);
//     //   vTaskDelay(100);
//     // }
//   }
//   vTaskDelete(NULL);
// }

// void BeepTask(void *parameter)
// {
//   while (1)
//   {
//   }
//   vTaskDelete(NULL);
// }

// int RevBuffer[100];
// int RevPointer = 0;
// int MessageLength = 0;
// bool StartRev = false;
// bool MessageReceived = false;
// void Serial2Task(void *parameter)
// {
//   while (1)
//   {
//     if (Serial2.available())
//     {
//       int inByte = Serial2.read();
//       Serial.write(inByte);
//     }
//     if (Serial.available())
//     {
//       int inByte = Serial.read();
//       if (!StartRev)
//       {
//         if ((char)inByte == '$')
//         {
//           StartRev = true;
//         }
//       }
//       else
//       {
//         if (inByte == '#')
//         {
//           MessageLength = RevPointer;
//           RevPointer = 0;
//           StartRev = false;
//           MessageReceived = true;
//         }
//         else
//         {
//           RevBuffer[RevPointer] = inByte;
//           RevPointer += 1;
//         }
//       }
//       if (MessageReceived)
//       {
//         MessageReceived = 0;
//         String msg = "$cam:";
//         for (int i = 0; i < MessageLength; i++)
//         {
//           msg += String(RevBuffer[i], HEX) + " ";
//         }
//         int color = RevBuffer[0];
//         int visionPXCnt = (RevBuffer[1] << 16 + RevBuffer[2] << 8 + RevBuffer[3]) >> 5;
//         int dataSecondSeg = RevBuffer[4] << 24 + RevBuffer[5] << 16 + RevBuffer[6] << 8 + RevBuffer[7];
//         int centerX = dataSecondSeg >> 22;
//         int centerY = (dataSecondSeg << 10) >> 22;
//         MonitorSend(msg);
//       }

//       Serial2.write(inByte);
//     }
//   }
// }

// void FeedBackTask(void *parameter)
// {
//   int data = 0;
//   bool Beep = false;

//   while (1)
//   {
//     while (MonitorClient == nullptr)
//     {
//       vTaskDelay(10);
//       Beep = true;
//       // wait for monitor
//     }
//     if (Beep)
//     {
//       for (int i = 0; i < 2; i++)
//       {
//         digitalWrite(PIN_BEEP, HIGH);
//         vTaskDelay(100);
//         digitalWrite(PIN_BEEP, LOW);
//         vTaskDelay(50);
//       }
//       Beep = false;
//     }

//     data += 1;
//     String msg = "$opt:";
//     msg += String(OpticalFlowPosX);
//     msg += " ";
//     msg += String(OpticalFlowPosY);
//     msg += "  real:";
//     msg += String(RealPosX);
//     msg += " ";
//     msg += String(RealPosY);
//     msg += "  yaw:";
//     msg += String(YawAngle);
//     msg += "  mpu:";
//     msg += String(MPUYawAngle);
//     MonitorSend(msg);
//     // Serial.println(msg);
//     vTaskDelay(100);
//   }
//   vTaskDelete(NULL);
// }

// float K2 = 0.3;
// float K1 = 0.8;
// float currentYawRadian = 0;
// void SensorTask(void *parameter)
// {
//   while (1)
//   {
//     // vTaskSuspend(FeedbackHandler);
//     // vTaskSuspend(DriveHandler);
//     // vTaskSuspend(ControlHandler);
//     // Serial.print(YawAngle);
//     // Serial.print(" ");
//     // Serial.print(OpticalFlowYawAngle);
//     // Serial.print(" ");
//     // Serial.print(MPUYawAngle);
//     // Serial.print(" ");
//     // Serial.println(CurrentYaw);
//     //  Serial.print(" opt_x:");
//     // Serial.print(OpticalFlowPosX);
//     // Serial.print(" opt_y:");
//     // Serial.print(OpticalFlowPosY);
//     // Serial.print(" opt_x:");
//     // Serial.print(OpticalFlowSpeedX);
//     // Serial.print(" opt_y:");
//     // Serial.print(OpticalFlowSpeedY);
//     // Serial.println();
//     UpdateOpticalFlow();
//     CurrentYaw = K1 * CurrentYaw + (1 - K1) * OpticalFlowYawAngle;
//     currentYawRadian = CurrentYaw * 3.1415926 / 180;
//     RealVelocityX = sin(currentYawRadian) * OpticalFlowSpeedY / 157;
//     RealVelocityY = cos(currentYawRadian) * OpticalFlowSpeedY / 157;
//     RealPosX += RealVelocityX;
//     RealPosY += RealVelocityY;
//     // GetOpticalFlowQuality();
//     // CurrentYaw=OpticalFlowYawAngle;

//     // vTaskResume(FeedbackHandler);
//     // vTaskResume(DriveHandler);
//     // vTaskResume(ControlHandler);
//     vTaskDelay(5);
//     // delay(15);
//   }
//   vTaskDelete(NULL);
// }

// void MPUTask(void *parameter)
// {
//   while (1)
//   {
//     UpdateMPU();
//     Serial.print("#");
//     CurrentYaw = K2 * CurrentYaw + (1 - K2) * (CurrentYaw + MPUYawAngleIncrement);
//     MPUYawAngleIncrement = 0;
//     vTaskDelay(15);
//   }
//   vTaskDelete(NULL);
// }

// #define MAX_STATE 16
// int distanceSign = 1;
// int lastDistanceSign = 1;
// double coe = 0;
// void ControlTask(void *parameter)
// {
//   int state = 0;
//   while (1)
//   {
//     state += 1;
//     // 1 cycle

  
//     //     Serial.print(" ");
//     // Serial.print(RealPosY);
//     //     Serial.print(" ");
//     // Serial.print(RealPosY);

//     // position control
//     if (state % 4)
//     {
//       CurrentPosX = RealPosX;
//       CurrentPosY = RealPosY;
//       PosXController.Compute();
//       PosYController.Compute();
//       if(RealPosY>=25&&RealPosY<40){
//         PosXControl+=5/((50-RealPosY)/50);
//       }
//     }

//     // velocity control
//     if (state % 2)
//     {
//       float _TargetYaw = 0;
//       float XDiff = PosYControl - RealVelocityX;
//       float YDiff = PosXControl - RealVelocityY;
//       TargetVelocity = XDiff * XDiff + YDiff * YDiff;
//       if (TargetVelocity != 0)
//         TargetVelocity = sqrt(TargetVelocity);
//       float XProjDiff = TargetVelocity * sin(currentYawRadian);
//       VelocityController.Compute();
//       if (abs(XDiff) <= 1)
//         XDiff = 0;
//       if (abs(YDiff) <= 1)
//         YDiff = 0;
//       float atanVal;
//       if (abs(XDiff) <= 0.000000001)
//       {
//         XDiff = 0.000000001;
//       }
//       if (XDiff == 0 && YDiff == 0)
//         atanVal = 0;
//       else
//         // atanVal = XDiff == 0 ? 90 : atan(YDiff / XDiff) * 180 / 3.1415926;
//         atanVal = atan(YDiff / XDiff) * 180 / 3.1415926;

//       _TargetYaw = atanVal;
//       if (XDiff >= 0)
//         distanceSign = 1;
//       else
//         distanceSign = -1;

//       // if (XDiff > 0)
//       //   _TargetYaw = atanVal;
//       // else
//       //   _TargetYaw = atanVal + 180 * MathSign(YDiff);
//       if (TargetVelocity >= 10)
//       {
//         TargetYaw = (double)_TargetYaw;
//       }
//       YawDiff = GetAngleDiff(TargetYaw,CurrentYaw);
//       double abs_YawDiff = abs(YawDiff);

//       if (abs_YawDiff > 45)
//         coe = 0;
//       else
//         coe = -1 / 45 * YawDiff + 1;
//       if (abs(XDiff) <= 1 && abs_YawDiff < 10)
//         distanceSign = lastDistanceSign;
//       lastDistanceSign = distanceSign;
//     }

//     // yaw control
//     YawController.Compute();
//     if (ControlMode < MANUAL_CONTROLLER)
//     {
//       int distanceControlEnable = ControlMode == AUTO_CONTROLLER ? 1 : 0;
//       if (GamepadClient == nullptr)
//       {
//         if (TargetVelocity >= 2)
//         {
//           MotorOneControl = (int)YawControl + distanceControlEnable * (coe * distanceSign * VelocityControl);
//           MotorTwoControl = -(int)YawControl + distanceControlEnable * (coe * distanceSign * VelocityControl);
//         }
//         else
//         {
//           MotorOneControl = (int)YawControl;
//           MotorTwoControl = -(int)YawControl;
//         }
//       }
//       else
//       {
//         MotorOneControl = 0;
//         MotorTwoControl = 0;
//       }
//     }
//     else
//     {
//       MotorOneControl = 0;
//       MotorTwoControl = 0;
//     }

//     if (state >= MAX_STATE)
//     {
//       state = 0;
//     }
//     // vTaskDelay(50);
//       Serial.print("Current yaw:");
//     Serial.print(CurrentYaw);
//     Serial.print(" target yaw:");
//     Serial.print(TargetYaw);
//     Serial.print(" Yaw control:");
//     Serial.print(YawControl);
//     Serial.print(" pxc:");
//     Serial.print(PosXControl);
//     Serial.print(" pyc:");
//     Serial.print(PosYControl);
//     Serial.print(" x:");
//     Serial.print(RealPosX);
//     Serial.print(" y:");
//     Serial.print(RealPosY);
//     Serial.print(" v_vel:");
//     Serial.println(TargetVelocity);
//     vTaskDelay(25);
//   }
//   vTaskDelete(NULL);
// }

// void InitSystem()
// {
//   Serial.begin(115200);
//   // Serial2.begin(115200);
//   // pinMode(PIN_DEBUG_LED, OUTPUT);
//   pinMode(PIN_BEEP, OUTPUT);

//   //   ledcSetup(MOTORA_PWM_CHANNEL, 5000, 13);
//   //   ledcAttachPin(PIN_MOTOR_PWMA, MOTORA_PWM_CHANNEL);
//   //   ledcSetup(MOTORB_PWM_CHANNEL, 5000, 13);
//   //   ledcAttachPin(PIN_MOTOR_PWMB, MOTORB_PWM_CHANNEL);

//   InitOpticalFlow();
//   InitMPU6050();
//   InitComms();
//   InitController();
//   // MPUTest();
//   // TestOpticalFlow();
//   SERIAL_USED.println("System Init success");
//   for (int i = 0; i < 3; i++)
//   {
//     digitalWrite(PIN_BEEP, HIGH);
//     vTaskDelay(100);
//     digitalWrite(PIN_BEEP, LOW);
//     vTaskDelay(100);
//   }
// }
// void _ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255)
// {
//   // calculate duty, 8191 from 2 ^ 13 - 1
//   uint32_t duty = (8191 / valueMax) * min(value, valueMax);

//   // 向 LEDC 写入任务
//   ledcWrite(channel, duty);
// }
// void setup()
// {
//   InitSystem();

//   // xTaskCreate(DebugTask, "DebugTask", 1024, NULL, 1, NULL);
//   // xTaskCreate(BeepTask, "BeepTask", 1024, NULL, 1, NULL);
//   // xTaskCreate(Serial2Task, "Serial2Task", 4096, NULL, 2, NULL);

//   xTaskCreate(FeedBackTask, "FeedBackTask", 4096, NULL, 0, &FeedbackHandler);

//   xTaskCreate(SensorTask, "SensorTask", 4096, NULL, 4, &SensorHandler);
//   xTaskCreate(MPUTask, "SensorTask", 4096, NULL, 4, NULL);
//   xTaskCreate(ControlTask, "ControlTask", 4096, NULL, 2, &ControlHandler);
//   xTaskCreate(
//       DriveTask,      /*任务函数*/
//       "DriveTask",    /*带任务名称的字符串*/
//       2048,           /*堆栈大小，单位为字节*/
//       NULL,           /*作为任务输入传递的参数*/
//       0,              /*任务的优先级*/
//       &DriveHandler); /*任务句柄*/
// }

// void loop()
// {
//   ws.cleanupClients();
//   delay(10);
//   //   analogWrite(PIN_MOTOR_PWMB,150);
//   // analogWrite(PIN_MOTOR_PWMA,150);
//   // ledcAnalogWrite(0, 150);
//   // ledcAnalogWrite(1, 150);
// }
