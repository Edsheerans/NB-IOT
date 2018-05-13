#include "adxl362.h"

/* Buffer used for transmission */
uint8_t aTxBuffer[10];

/* Buffer used for reception */
uint8_t aRxBuffer[200];

/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

/***************************************************************************//**
 * @brief Initializes communication with the device and checks if the part is
 *        present by reading the device id.
 *
 * @return 1 if the initialization was successful and the device is present,
 *         0 if an error occurred.
*******************************************************************************/
unsigned char ADXL362_Init(void)
{
    unsigned char regValue = 0;
    unsigned char status   = 0;

    status = SPI_Init(0, 2000000, 0, 1);
    ADXL362_GetRegisterValue(&regValue, ADXL362_REG_PARTID, 1);

    if((regValue != ADXL362_PART_ID))
    {
        status = 0;
    }
    return status;
}

/***************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerValue - Data value to write.
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes. Accepted values: 0 - 1.
 *
 * @return None.
*******************************************************************************/
void ADXL362_SetRegisterValue(unsigned short registerValue,
                              unsigned char  registerAddress,
                              unsigned char  bytesNumber)
{
    unsigned char buffer[4] = {0, 0, 0, 0};

    buffer[0] = ADXL362_WRITE_REG;
    buffer[1] = registerAddress;
    buffer[2] = (registerValue & 0x00FF);
    buffer[3] = (registerValue >> 8);
    SPI_Write(ADXL362_SLAVE_ID, buffer, bytesNumber + 2);
}

/***************************************************************************//**
 * @brief Performs a burst read of a specified number of registers.
 *
 * @param pReadData - The read values are stored in this buffer.
 * @param registerAddress - The start address of the burst read.
 * @param bytesNumber - Number of bytes to read.
 *
 * @return None.
*******************************************************************************/
void ADXL362_GetRegisterValue(unsigned char* pReadData,
                              unsigned char  registerAddress,
                              unsigned char  bytesNumber)
{
    unsigned char buffer[10];
    unsigned char index = 0;
    
    buffer[0] = ADXL362_READ_REG;
    buffer[1] = registerAddress;
    for(index = 0; index < bytesNumber; index++)
    {
        buffer[index + 2] = pReadData[index];
    }
    SPI_Read(ADXL362_SLAVE_ID, buffer, bytesNumber + 2);
    for(index = 0; index < bytesNumber; index++)
    {
        pReadData[index] = buffer[index + 2];
    }
}

/***************************************************************************//**
 * @brief Reads multiple bytes from the device's FIFO buffer.
 *
 * @param pBuffer - Stores the read bytes.
 * @param bytesNumber - Number of bytes to read.
 *
 * @return None.
*******************************************************************************/
void ADXL362_GetFifoValue(unsigned char* pBuffer, unsigned short bytesNumber)
{
    unsigned char  buffer[20];
    unsigned short index = 0;

    buffer[0] = ADXL362_WRITE_FIFO;
    for(index = 0; index < bytesNumber; index++)
    {
        buffer[index + 1] = pBuffer[index];
    }
    SPI_Read(ADXL362_SLAVE_ID, buffer, bytesNumber + 1);
    for(index = 0; index < bytesNumber; index++)
    {
        pBuffer[index] = buffer[index + 1];
    }
}

/***************************************************************************//**
 * @brief Resets the device via SPI communication bus.
 *
 * @return None.
*******************************************************************************/
void ADXL362_SoftwareReset(void)
{
    ADXL362_SetRegisterValue(ADXL362_RESET_KEY, ADXL362_REG_SOFT_RESET, 1);
}

/***************************************************************************//**
 * @brief Places the device into standby/measure mode.
 *
 * @param pwrMode - Power mode.
 *                  Example: 0 - standby mode.
 *		                     1 - measure mode.
 *                           2 - autosleep mode
 *
 * @return None.
*******************************************************************************/
void ADXL362_SetPowerMode(unsigned char pwrMode)
{
    unsigned char oldPowerCtl = 0;
    unsigned char newPowerCtl = 0;

		if(pwrMode == 2)
		{
			newPowerCtl = newPowerCtl | 0x06;								//?????????????
	    	ADXL362_SetRegisterValue(newPowerCtl, ADXL362_REG_POWER_CTL, 1);
		
		}
		else
		{
    		ADXL362_GetRegisterValue(&oldPowerCtl, ADXL362_REG_POWER_CTL, 1);
    		newPowerCtl = oldPowerCtl & ~ADXL362_POWER_CTL_MEASURE(0x3);
    		newPowerCtl = newPowerCtl |
                  (pwrMode * ADXL362_POWER_CTL_MEASURE(ADXL362_MEASURE_ON));	
   			ADXL362_SetRegisterValue(newPowerCtl, ADXL362_REG_POWER_CTL, 1);
		}
}

/***************************************************************************//**
 * @brief Selects the measurement range.
 *
 * @param gRange - Range option.
 *                  Example: ADXL362_RANGE_2G  -  +-2 g
 *                           ADXL362_RANGE_4G  -  +-4 g
 *                           ADXL362_RANGE_8G  -  +-8 g
 *                           
 * @return None.
*******************************************************************************/
void ADXL362_SetRange(unsigned char gRange)
{
    unsigned char oldFilterCtl = 0;
    unsigned char newFilterCtl = 0;

    ADXL362_GetRegisterValue(&oldFilterCtl, ADXL362_REG_FILTER_CTL, 1);
    newFilterCtl = oldFilterCtl & ~ADXL362_FILTER_CTL_RANGE(0x3);
    newFilterCtl = newFilterCtl | ADXL362_FILTER_CTL_RANGE(gRange);
    ADXL362_SetRegisterValue(newFilterCtl, ADXL362_REG_FILTER_CTL, 1);
}

/***************************************************************************//**
 * @brief Selects the Output Data Rate of the device.
 *
 * @param outRate - Output Data Rate option.
 *                  Example: ADXL362_ODR_12_5_HZ  -  12.5Hz
 *                           ADXL362_ODR_25_HZ    -  25Hz
 *                           ADXL362_ODR_50_HZ    -  50Hz
 *                           ADXL362_ODR_100_HZ   -  100Hz
 *                           ADXL362_ODR_200_HZ   -  200Hz
 *                           ADXL362_ODR_400_HZ   -  400Hz
 *
 * @return None.
*******************************************************************************/
void ADXL362_SetOutputRate(unsigned char outRate)
{
    unsigned char oldFilterCtl = 0;
    unsigned char newFilterCtl = 0;

    ADXL362_GetRegisterValue(&oldFilterCtl, ADXL362_REG_FILTER_CTL, 1);
    newFilterCtl = oldFilterCtl & ~ADXL362_FILTER_CTL_ODR(0x7);
    newFilterCtl = newFilterCtl | ADXL362_FILTER_CTL_ODR(outRate);
    ADXL362_SetRegisterValue(newFilterCtl, ADXL362_REG_FILTER_CTL, 1);
}

/***************************************************************************//**
 * @brief Reads the 3-axis raw data from the accelerometer.
 *
 * @param x - Stores the X-axis data(as two's complement).
 * @param y - Stores the X-axis data(as two's complement).
 * @param z - Stores the X-axis data(as two's complement).
 *
 * @return None.
*******************************************************************************/
#ifdef ADXL362_READ_XYZ_REG 
void ADXL362_GetXyz(short* x, short* y, short* z)
{
    unsigned char xyzValues[6] = {0, 0, 0, 0, 0, 0};

    ADXL362_GetRegisterValue(xyzValues, ADXL362_REG_XDATA_L, 6);
    *x = ((short)xyzValues[1] << 8) + xyzValues[0];
    *y = ((short)xyzValues[3] << 8) + xyzValues[2];
    *z = ((short)xyzValues[5] << 8) + xyzValues[4];
}
#endif

/***************************************************************************//**
 * @brief Reads the temperature of the device.
 *
 * @return tempCelsius - The value of the temperature(degrees Celsius).
*******************************************************************************/
#ifdef ADXL362_READ_TEMP
float ADXL362_ReadTemperature(void)
{
    unsigned char rawTempData[2] = {0, 0};
    short         signedTemp     = 0;
    float         tempCelsius    = 0;

    ADXL362_GetRegisterValue(rawTempData, ADXL362_REG_TEMP_L, 2);
    signedTemp = (short)(rawTempData[1] << 8) + rawTempData[0];
    tempCelsius = (float)signedTemp * 0.065;
    
    return tempCelsius;
}
#endif

/***************************************************************************//**
 * @brief Configures the FIFO feature.
 *
 * @param mode - Mode selection.
 *               Example: ADXL362_FIFO_DISABLE        -  FIFO is disabled.
 *                        ADXL362_FIFO_OLDEST_SAVED   -  Oldest saved mode.
 *                        ADXL362_FIFO_STREAM         -  Stream mode.
 *                        ADXL362_FIFO_TRIGGERED      -  Triggered mode.
 * @param waterMarkLvl - Specifies the number of samples to store in the FIFO.
 * @param enTempRead - Store Temperature Data to FIFO.
 *                     Example: 1 - temperature data is stored in the FIFO
 *                                  together with x-, y- and x-axis data.
 *                              0 - temperature data is skipped.
 *
 * @return None.
*******************************************************************************/
void ADXL362_FifoSetup(unsigned char  mode,
                       unsigned short waterMarkLvl,
                       unsigned char  enTempRead)
{
    unsigned char writeVal = 0;

    writeVal = ADXL362_FIFO_CTL_FIFO_MODE(mode) |
               (enTempRead * ADXL362_FIFO_CTL_FIFO_TEMP) |
               ADXL362_FIFO_CTL_AH;
    ADXL362_SetRegisterValue(writeVal, ADXL362_REG_FIFO_CTL, 1);
    ADXL362_SetRegisterValue(waterMarkLvl, ADXL362_REG_FIFO_SAMPLES, 2);
}

/***************************************************************************//**
 * @brief Configures activity detection.
 *
 * @param refOrAbs - Referenced/Absolute Activity Select.
 *                   Example: 0 - absolute mode.
 *                            1 - referenced mode.
 * @param threshold - 11-bit unsigned value that the adxl362 samples are
 *                    compared to.
 * @param time - 8-bit value written to the activity timer register. The amount
 *               of time (in seconds) is: time / ODR, where ODR - is the output 
 *               data rate.
 *
 * @return None.
*******************************************************************************/
void ADXL362_SetupActivityDetection(unsigned char  refOrAbs,
                                    unsigned short threshold,
                                    unsigned char  time)
{
    unsigned char oldActInactReg = 0;
    unsigned char newActInactReg = 0;

    /* Configure motion threshold and activity timer. */
    ADXL362_SetRegisterValue((threshold & 0x7FF), ADXL362_REG_THRESH_ACT_L, 2);
    ADXL362_SetRegisterValue(time, ADXL362_REG_TIME_ACT, 1);
    /* Enable activity interrupt and select a referenced or absolute
       configuration. */
    ADXL362_GetRegisterValue(&oldActInactReg, ADXL362_REG_ACT_INACT_CTL, 1);
    newActInactReg = oldActInactReg & ~ADXL362_ACT_INACT_CTL_ACT_REF;
    newActInactReg |= ADXL362_ACT_INACT_CTL_ACT_EN |
                     (refOrAbs * ADXL362_ACT_INACT_CTL_ACT_REF);
    ADXL362_SetRegisterValue(newActInactReg, ADXL362_REG_ACT_INACT_CTL, 1);
}

/***************************************************************************//**
 * @brief Configures inactivity detection.
 *
 * @param refOrAbs - Referenced/Absolute Inactivity Select.
 *                   Example: 0 - absolute mode.
 *                            1 - referenced mode.
 * @param threshold - 11-bit unsigned value that the adxl362 samples are
 *                    compared to.
 * @param time - 16-bit value written to the inactivity timer register. The 
 *               amount of time (in seconds) is: time / ODR, where ODR - is the 
 *               output  data rate.
 *
 * @return None.
*******************************************************************************/
void ADXL362_SetupInactivityDetection(unsigned char  refOrAbs,
                                      unsigned short threshold,
                                      unsigned short time)
{
    unsigned char oldActInactReg = 0;
    unsigned char newActInactReg = 0;
    
    /* Configure motion threshold and inactivity timer. */
    ADXL362_SetRegisterValue((threshold & 0x7FF),
                              ADXL362_REG_THRESH_INACT_L,
                              2);
    ADXL362_SetRegisterValue(time, ADXL362_REG_TIME_INACT_L, 2);
    /* Enable inactivity interrupt and select a referenced or absolute
       configuration. */
    ADXL362_GetRegisterValue(&oldActInactReg, ADXL362_REG_ACT_INACT_CTL, 1);
    newActInactReg = oldActInactReg & ~ADXL362_ACT_INACT_CTL_INACT_REF;
    newActInactReg |= ADXL362_ACT_INACT_CTL_INACT_EN |
                     (refOrAbs * ADXL362_ACT_INACT_CTL_INACT_REF);
    ADXL362_SetRegisterValue(newActInactReg, ADXL362_REG_ACT_INACT_CTL, 1);
}

void ADXL362_INTMAP1()
{
	unsigned char INTMAP1_Reg = 0;	
	INTMAP1_Reg |= 0x01;  //int1 => data ready
 	ADXL362_SetRegisterValue(INTMAP1_Reg, ADXL362_REG_INTMAP1, 1);
}

void ADXL362_INTMAP2()
{
	unsigned char INTMAP2_Reg = 0;
	INTMAP2_Reg |= 0x40; //int2 => AWAKE
 	ADXL362_SetRegisterValue(INTMAP2_Reg, ADXL362_REG_INTMAP2, 1);
}

void ADXL362_ClearFifoValue(uint16_t bytesNumber)
{
    unsigned char  buffer[1];

    buffer[0] = ADXL362_WRITE_FIFO;
    SPI_Read_ClearFIFO(buffer, bytesNumber + 1);
}

/*****************************************************************************
 * @brief Configures Spi Interface.
 *
******************************************************************************/
/***************************************************************************//**
* @brief Initializes the SPI communication peripheral.
*
* @param lsbFirst - Transfer format (0 or 1).
*					 Example: 0x0 - MSB first.
*							  0x1 - LSB first.
* @param clockFreq - SPI clock frequency (Hz).
*					  Example: 1000 - SPI clock frequency is 1 kHz.
* @param clockPol - SPI clock polarity (0 or 1).
*					 Example: 0x0 - Idle state for clock is a low level; active
*									state is a high level;
*						  0x1 - Idle state for clock is a high level; active
*									state is a low level.
* @param clockEdg - SPI clock edge (0 or 1).
*					 Example: 0x0 - Serial output data changes on transition
*									from idle clock state to active clock state;
*							  0x1 - Serial output data changes on transition
*									from active clock state to idle clock state.
*
* @return status - Result of the initialization procedure.
*					Example: 1 - if initialization was successful;
*							 0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char SPI_Init(unsigned char lsbFirst,
						   unsigned long clockFreq,
						   unsigned char clockPol,
						   unsigned char clockEdg)
{
/*##-1- Configure the SPI peripheral #######################################*/
/* Set the SPI parameters */
	SpiHandle.Instance				 = SPIx;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	SpiHandle.Init.Direction		 = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase 		 = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity		 = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation	 = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial	 = 7;
	SpiHandle.Init.DataSize 		 = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit 		 = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS				 = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode			 = SPI_TIMODE_DISABLE;
	SpiHandle.Init.Mode 			 = SPI_MODE_MASTER;
		
	HAL_SPI_Init(&SpiHandle);
					
	return(1);
}
	
/***************************************************************************//**
* @brief Reads data from SPI.
*
* @param slaveDeviceId - The ID of the selected slave device.
* @param data - Data represents the write buffer as an input parameter and the
*				 read buffer as an output parameter.
* @param bytesNumber - Number of bytes to read.
*
* @return Number of read bytes.
*******************************************************************************/
unsigned char SPI_Read(unsigned char slaveDeviceId,
						   unsigned char* dat,
						   unsigned char bytesNumber)
{
	
	unsigned char byte = 0; 	
	for(byte = 0; byte < bytesNumber; byte++)
	{
		aTxBuffer[byte] = dat[byte];
	}
		
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
		
	HAL_SPI_TransmitReceive_IT(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, bytesNumber);
		
	while (HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_READY)
	{
	} 
			
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
		
	for(byte = 0; byte < bytesNumber; byte++)
	{
		dat[byte] = aRxBuffer[byte];
	}
		
	return(bytesNumber);
	
}
						   
/***************************************************************************//**
* @brief Reads data from SPI.
*
* @param slaveDeviceId - The ID of the selected slave device.
* @param data - Data represents the write buffer as an input parameter and the
*				 read buffer as an output parameter.
* @param bytesNumber - Number of bytes to read.
*
* @return Number of read bytes.
*******************************************************************************/
void SPI_Read_ClearFIFO(
						   unsigned char* dat,
						   uint16_t bytesNumber)
{
		
	aTxBuffer[0] = dat[0];
		
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_IT(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, bytesNumber);
		
	while (HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_READY)
	{
	} 
		
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	
	
}
						   
/***************************************************************************//**
* @brief Writes data to SPI.
*
* @param slaveDeviceId - The ID of the selected slave device.
* @param data - Data represents the write buffer.
* @param bytesNumber - Number of bytes to write.
*
* @return Number of written bytes.
*******************************************************************************/
unsigned char SPI_Write(unsigned char slaveDeviceId,
							unsigned char* dat,
							unsigned char bytesNumber)
{
	
	unsigned char byte = 0; 	
	for(byte = 0; byte < bytesNumber; byte++)
	{
		aTxBuffer[byte] = dat[byte];
	}
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_IT(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, bytesNumber);
		
	while (HAL_SPI_GetState(&SpiHandle) != HAL_SPI_STATE_READY)
	{
	} 
		
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
	
	return(bytesNumber);
	
}

