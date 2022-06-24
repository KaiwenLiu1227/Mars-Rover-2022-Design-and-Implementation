#include "mpu6050.h"

Adafruit_MPU6050 mpu;
float MPUYawOffset = 0;
float MPUYawAngle = 0;
float MPUYawAngleIncrement=0;
float IncrementCoe = 0;
float dt = 15;
bool initOffset = false;
void InitMPU6050()
{
    if (!mpu.begin())
    {
        SERIAL_USED.println("Failed to find MPU6050 chip");
        for (int i = 0; i < 3; i++)
        {
            digitalWrite(PIN_BEEP, HIGH);
            delay(1000);
            digitalWrite(PIN_BEEP, LOW);
            delay(200);
        }
        while (1)
            ;
    }

    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    // mpu.setMotionDetectionThreshold(1);
    // mpu.setMotionDetectionDuration(20);
    // mpu.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
    // mpu.setInterruptPinPolarity(true);
    // mpu.setMotionInterrupt(true);
    delay(500);
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    MPUYawOffset = g.gyro.x;
    IncrementCoe = 9 * dt / 100;
}

void UpdateMPU()
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float increment = g.gyro.x - MPUYawOffset;
    if (abs(increment) <= 0.015)
        increment = 0;
    MPUYawAngleIncrement= increment * IncrementCoe;
    MPUYawAngle+=MPUYawAngleIncrement;
}

void MPUTest()
{
    while (1)
    {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        float increment = g.gyro.x - MPUYawOffset;
        Serial.print(increment);
        Serial.print(" ");
        if (abs(increment) <= 0.015)
            increment = 0;
        MPUYawAngle += increment * IncrementCoe;
        Serial.println(MPUYawAngle);
        delay(5);
    }

    // /* Get new sensor events with the readings */
}