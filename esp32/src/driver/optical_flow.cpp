
#include "optical_flow.h"

int total_x1 = 0;
int total_y1 = 0;

int x = 0;
int y = 0;

int a = 0;
int b = 0;
float OpticalFlowPosX = 0;
float OpticalFlowPosY = 0;
float RealPosX = 0;
float RealPosY = 0;
float RealVelocityX = 0;
float RealVelocityY = 0;
float YawAngle = 0;
float OpticalFlowYawAngle = 0;
float OpticalFlowSpeedX = 0;
float OpticalFlowSpeedY = 0;
float OpticalFlowXOffset=0;
int OpticalFlowQuality = 0;
volatile byte movementflag = 0;
volatile int xydat[2];

byte frame[ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y];

struct MD
{
    byte motion;
    char dx, dy;
    byte squal;
    word shutter;
    byte max_pix;
};

MD OpticalData;
int convTwosComp(int b)
{
    //Convert from 2's complement
    if (b & 0x80)
    {
        b = -1 * ((b ^ 0xff) + 1);
    }
    return b;
}

int tdistance = 0;

void mousecam_write_reg(int reg, int val)
{
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, LOW);
#endif
    SPI.transfer(reg | 0x80);
    SPI.transfer(val);
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, HIGH);
#endif
    delayMicroseconds(50);
}

int mousecam_read_reg(int reg)
{
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, LOW);
#endif
    SPI.transfer(reg);
    delayMicroseconds(75);
    int ret = SPI.transfer(0xff);
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, HIGH);
#endif
    delayMicroseconds(1);
    return ret;
}
void mousecam_reset()
{
#if USE_RESET_PIN
    digitalWrite(PIN_MOUSECAM_RESET, HIGH);
    delay(1); // reset pulse >10us
    digitalWrite(PIN_MOUSECAM_RESET, LOW);
#endif
    delay(35); // 35ms from reset to functional
}

int mousecam_init()
{
#if USE_RESET_PIN
    pinMode(PIN_MOUSECAM_RESET, OUTPUT);
#endif
#if USE_CS_PIN
    pinMode(PIN_MOUSECAM_CS, OUTPUT);

    digitalWrite(PIN_MOUSECAM_CS, HIGH);
#endif
    mousecam_reset();

    int pid = mousecam_read_reg(ADNS3080_PRODUCT_ID);
    if (pid != ADNS3080_PRODUCT_ID_VAL)
        return -1;

    // turn on sensitive mode
    mousecam_write_reg(ADNS3080_CONFIGURATION_BITS, 0x19);

    return 0;
}

void mousecam_read_motion(struct MD *p)
{
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, LOW);
#endif
    SPI.transfer(ADNS3080_MOTION_BURST);
    delayMicroseconds(75);
    p->motion = SPI.transfer(0xff);
    p->dx = SPI.transfer(0xff);
    p->dy = SPI.transfer(0xff);
    p->squal = SPI.transfer(0xff);
    p->shutter = SPI.transfer(0xff) << 8;
    p->shutter |= SPI.transfer(0xff);
    p->max_pix = SPI.transfer(0xff);
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, HIGH);
#endif
    delayMicroseconds(5);
}

// pdata must point to an array of size ADNS3080_PIXELS_X x ADNS3080_PIXELS_Y
// you must call mousecam_reset() after this if you want to go back to normal operation
int mousecam_frame_capture(byte *pdata)
{
    mousecam_write_reg(ADNS3080_FRAME_CAPTURE, 0x83);
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, LOW);
#endif
    SPI.transfer(ADNS3080_PIXEL_BURST);
    delayMicroseconds(50);

    int pix;
    byte started = 0;
    int count;
    int timeout = 0;
    int ret = 0;
    for (count = 0; count < ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y;)
    {
        pix = SPI.transfer(0xff);
        delayMicroseconds(10);
        if (started == 0)
        {
            if (pix & 0x40)
                started = 1;
            else
            {
                timeout++;
                if (timeout == 100)
                {
                    ret = -1;
                    break;
                }
            }
        }
        if (started == 1)
        {
            pdata[count++] = (pix & 0x3f) << 2; // scale to normal grayscale byte range
        }
    }
#if USE_CS_PIN
    digitalWrite(PIN_MOUSECAM_CS, HIGH);
#endif
    delayMicroseconds(14);

    return ret;
}

void InitOpticalFlow()
{
#if USE_CS_PIN
    pinMode(PIN_SS, OUTPUT);
#endif
    pinMode(PIN_MISO, INPUT);
    pinMode(PIN_MOSI, OUTPUT);
    pinMode(PIN_SCK, OUTPUT);

    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    SPI.setDataMode(SPI_MODE3);
    SPI.setBitOrder(MSBFIRST);

    if (mousecam_init() == -1)
    {
        SERIAL_USED.println("Mouse cam failed to init");

        for (int i = 0; i < 3; i++)
        {
            digitalWrite(PIN_BEEP, HIGH);
            delay(1000);
            digitalWrite(PIN_BEEP, LOW);
            delay(200);
        }
        while (1)
        {
        }
    }
}

char asciiart(int k)
{
    static char foo[] = "WX86*3I>!;~:,`. ";
    return foo[k >> 4];
}

int GetOpticalFlowQuality(bool debug)
{
    MD md;
    mousecam_read_motion(&md);
    if (debug)
    {
        for (int i = 0; i < md.squal; i++)
            SERIAL_USED.print('*');
    }
    return md.squal;
}
double OPT_TO_CM= 1.5;
void UpdateOpticalFlow()
{
    MD md;
    mousecam_read_motion(&md);
    OpticalData = md;
    OpticalFlowQuality = md.squal;
    OpticalFlowSpeedX = convTwosComp(md.dx)*OPT_TO_CM;
    OpticalFlowSpeedY = convTwosComp(md.dy)*OPT_TO_CM;

    total_x1 = total_x1 + OpticalFlowSpeedX;
    total_y1 = total_y1 + OpticalFlowSpeedY;

    OpticalFlowPosX = (float)total_x1 / 157;
    OpticalFlowPosY = (float)total_y1 / 157;

    // YawAngle = (float)total_x1 * 3.1415926 / 3498.54;
    YawAngle = ((float)(total_x1-OpticalFlowXOffset) * MATH_PI) / (5804.159166);
    // if (abs(YawAngle) <= 0.05)
    // {
    //     YawAngle = 0;
    // }

    OpticalFlowYawAngle = YawAngle * 180 / MATH_PI;
}

void TestOpticalFlow()
{
    SERIAL_USED.println("test");
    while (1)
    {

#if 0
/*
    if(movementflag){

    tdistance = tdistance + convTwosComp(xydat[0]);
    SERIAL_USED.println("Distance = " + String(tdistance));
    movementflag=0;
    delay(3);
    }

  */
  // if enabled this section grabs frames and outputs them as ascii art

  if(mousecam_frame_capture(frame)==0)
  {
    int i,j,k;
    for(i=0, k=0; i<ADNS3080_PIXELS_Y; i++)
    {
      for(j=0; j<ADNS3080_PIXELS_X; j++, k++)
      {
        SERIAL_USED.print(asciiart(frame[k]));
        SERIAL_USED.print(' ');
      }
      SERIAL_USED.println();
    }
  }
  SERIAL_USED.println();
  delay(250);

#else

        // if enabled this section produces a bar graph of the surface quality that can be used to focus the camera
        // also drawn is the average pixel value 0-63 and the shutter speed and the motion dx,dy.

        int val = mousecam_read_reg(ADNS3080_PIXEL_SUM);
        MD md;
        mousecam_read_motion(&md);
        for (int i = 0; i < md.squal; i++)
            SERIAL_USED.print('*');
        SERIAL_USED.print(' ');
        SERIAL_USED.print((val * 100) / 351);
        SERIAL_USED.print(' ');
        SERIAL_USED.print(md.shutter);
        SERIAL_USED.print(" (");
        SERIAL_USED.print((int)md.dx);
        SERIAL_USED.print(',');
        SERIAL_USED.print((int)md.dy);
        SERIAL_USED.println(')');

        // SERIAL_USED.println(md.max_pix);
        delay(100);

        OpticalFlowSpeedX = convTwosComp(md.dx);
        OpticalFlowSpeedY = convTwosComp(md.dy);

        total_x1 = total_x1 + OpticalFlowSpeedX;
        total_y1 = total_y1 + OpticalFlowSpeedX;

        OpticalFlowPosX = total_x1 / 157;
        OpticalFlowPosY = total_y1 / 157;

        SERIAL_USED.print('\n');

        SERIAL_USED.println("Distance_x = " + String(OpticalFlowPosX));

        SERIAL_USED.println("Distance_y = " + String(OpticalFlowPosY));
        SERIAL_USED.print('\n');
        delay(250);

#endif
    }
}
