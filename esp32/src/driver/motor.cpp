#include "motor.h"

Motor motor1 = Motor(PIN_MOTOR_AIN1, PIN_MOTOR_AIN2, PIN_MOTOR_PWMA, 1, PIN_MOTOR_STBY, MOTORA_PWM_CHANNEL);
Motor motor2 = Motor(PIN_MOTOR_BIN1, PIN_MOTOR_BIN2, PIN_MOTOR_PWMB, 1, PIN_MOTOR_STBY, MOTORB_PWM_CHANNEL);
int MotorOneSpeed = 0;
int MotorTwoSpeed = 0;
int MotorOneControl = 0;
int MotorTwoControl = 0;
bool MotorOneForwarding = true;
bool MotorTwoForwarding = true;

void DriveMotors()
{
#if ENABLE_MOTOR_DEBUG
    SERIAL_USED.print(MotorOneSpeed);
    SERIAL_USED.print(" ");
    SERIAL_USED.print(MotorTwoSpeed);
#endif
    if (MotorOneSpeed + MotorOneControl == 0)
    {
        motor1.brake();
    }
    else
    {
        if (MotorOneForwarding)
        {
#if ENABLE_MOTOR_DEBUG
            SERIAL_USED.print(" f");
#endif
            motor1.drive(MotorOneSpeed + MotorOneControl);
        }
        else
        {
#if ENABLE_MOTOR_DEBUG
            SERIAL_USED.print(" b");
#endif
            motor1.drive(-MotorOneSpeed + MotorOneControl);
        }
    }

    if (MotorTwoSpeed + MotorTwoControl == 0)
    {
        motor2.brake();
    }
    else
    {
        if (MotorTwoForwarding)
        {
#if ENABLE_MOTOR_DEBUG
            SERIAL_USED.print("f");
#endif
            motor2.drive(MotorTwoSpeed + MotorTwoControl);
        }
        else
        {
#if ENABLE_MOTOR_DEBUG
            SERIAL_USED.print("b");
#endif
            motor2.drive(-MotorTwoSpeed + MotorTwoControl);
        }
    }
#if ENABLE_MOTOR_DEBUG
    SERIAL_USED.println();
#endif
}