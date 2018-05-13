#include "board.h"

/*================================================================
                       SHT2x Command Code
================================================================*/
/*
  Command                         Code              Comment
  Trigger Temperature Measurement 0xe3              Hold master
  Trigger Humidity Measurement    0xe5              Hold master
  Trigger Temperature Measuremeng 0xf3              No Hold master
  Trigger Humidity Measurement    0xf5              No Hold master
  Write user register             0xe6
  Read user register              0xe7
  Soft Reset                      0xfe
*/
#define SOFT_RESET      0xFE
#define READ_REGISTER   0xE7
#define WRITE_REGISTER  0xE6
#define TRIGGER_TEMP    0xE3
#define TRIGGER_HUMI    0xE5


static uint8_t I2cDeviceAddr = 0;
static bool SHT2xInitialized = false;

uint8_t SHT2xInit(void)
{
    uint8_t regVal = 0;

    SHT2xReset(); //reset I2C device to avoid power sync problem.

    SHT2xSetDeviceAddr(SHT2x_I2C_ADDRESS);

    if(SHT2xInitialized == false)
    {
        SHT2xInitialized = true;

        /* Test if SHT2x device has be reset and work fine */
        SHT2xRead(READ_REGISTER, &regVal);
        if((regVal & 0x02) != 0x02)
        {
            return FAIL;
        }

        /* Set RH/TH resolution 11bits */
        SHT2xWrite(WRITE_REGISTER, 0x83);
    }
    return SUCCESS;
}

uint8_t SHT2xReset(void)
{
//	I2cResetBus(&I2c);

    SHT2xWrite(SOFT_RESET, 0);   // Reset the SHT2x

    return SUCCESS;
}

uint8_t SHT2xWrite(uint8_t addr, uint8_t data)
{
    return I2cWriteBuffer(&I2c, I2cDeviceAddr, addr, &data, 1);
}

uint8_t SHT2xRead(uint8_t addr, uint8_t *data)
{
    return I2cReadBuffer(&I2c, I2cDeviceAddr, addr, data, 1);
}

uint8_t SHT2xReadBuffer(uint8_t addr, uint8_t *data, uint8_t size)
{
    return I2cReadBuffer(&I2c, I2cDeviceAddr, addr, data, size);
}

static uint16_t SHT2xMeasure(uint8_t addr)
{
    uint8_t raw_data[3] = {0};
    uint16_t tmp = 0;

    if(SHT2xReadBuffer(addr, raw_data, 3) != SUCCESS)
    {
        I2cDeInit(&I2c);
        DelayMs(10);
        I2cInit(&I2c, I2C_SCL, I2C_SDA);
        DelayMs(10);
        SHT2xInit();
        SHT2xReadBuffer(addr, raw_data, 3);
    }

    /* MSB<<8 + LSB&0xFC : refer to datasheet setion 5.4*/
    tmp = ((raw_data[0] << 8) + (raw_data[1] & 0xFC));

    //TODO: should put it in node?
    //temp = (temp * 175.72) / 65536 - 46.85;

    return tmp;
}

void SHT2xGetTempHumi(uint16_t *temp, uint16_t *humi)
{
    *temp = SHT2xMeasure(TRIGGER_TEMP);
    *humi = SHT2xMeasure(TRIGGER_HUMI);
}

void SHT2xSetDeviceAddr(uint8_t addr)
{
    I2cDeviceAddr = addr;
}

uint8_t SHT2xGetDeviceAddr(void)
{
    return I2cDeviceAddr;
}
