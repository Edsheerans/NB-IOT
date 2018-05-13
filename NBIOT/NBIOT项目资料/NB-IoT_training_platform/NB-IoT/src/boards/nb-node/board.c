/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Target board general functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "board.h"

/*!
 * Unique Devices IDs register set ( STM32L1xxx )
 */
#define         ID1                                 ( 0x1FF80050 )
#define         ID2                                 ( 0x1FF80054 )
#define         ID3                                 ( 0x1FF80064 )

/*
 * MCU objects
 */
I2c_t I2c;
Gpio_t GpsPps;
Gpio_t GpsRx;
Gpio_t GpsTx;
Gpio_t GpsPowerEn;

/*!
 * System Clock Configuration
 */
static void SystemClockConfig(void);

/*!
 * System Clock Re-Configuration when waking up from STOP mode
 */
static void SystemClockReConfig(void);

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;

/*!
 * Flag to indicate the Status of LED3
 */
volatile uint8_t Led3Status = 1;

/*!
 * Nested interrupt counter.
 *
 * \remark Interrupt should only be fully disabled once the value is 0
 */
static uint8_t IrqNestLevel = 0;

void BoardDisableIrq(void)
{
    __disable_irq();
    IrqNestLevel++;
}

void BoardEnableIrq(void)
{
    IrqNestLevel--;
    if(IrqNestLevel == 0)
    {
        __enable_irq();
    }
}

void BoardInitPeriph(void)
{
    // Init temperature and humidity sensor
    SHT2xInit();

    // Init GPS
    GpsInit();
}

void BoardInitMcu(void)
{

    if(McuInitialized == false)
    {
        HAL_Init();

        SystemClockConfig();

#if defined( UART_PRINTF )
        printf_init();
#endif

        I2cInit(&I2c, I2C_SCL, I2C_SDA);
        // sort timer init
        timer_init();

        // board TIM init
        tim_init(tim21);
        set_tim_cb(tim21, timer_task);
    }
    else
    {
        SystemClockReConfig();
    }
}

void BoardDeInitMcu(void)
{
    Gpio_t ioPin;

    UartDeInit(&Uart1);
    I2cDeInit(&I2c);
#if defined( UART_PRINTF )
    UartDeInit( &Uart2);
#endif
    GpioInit(&ioPin, OSC_HSE_IN, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0);
    GpioInit(&ioPin, OSC_HSE_OUT, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0);

    GpioInit(&ioPin, OSC_LSE_IN, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0);
    GpioInit(&ioPin, OSC_LSE_OUT, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0);
}

uint32_t BoardGetRandomSeed(void)
{
    return ((*(uint32_t*)ID1) ^ (*(uint32_t*)ID2) ^ (*(uint32_t*)ID3));
}

void BoardGetUniqueId(uint8_t *id)
{
    id[7] = ((*(uint32_t*)ID1) + (*(uint32_t*)ID3)) >> 24;
    id[6] = ((*(uint32_t*)ID1) + (*(uint32_t*)ID3)) >> 16;
    id[5] = ((*(uint32_t*)ID1) + (*(uint32_t*)ID3)) >> 8;
    id[4] = ((*(uint32_t*)ID1) + (*(uint32_t*)ID3));
    id[3] = ((*(uint32_t*)ID2)) >> 24;
    id[2] = ((*(uint32_t*)ID2)) >> 16;
    id[1] = ((*(uint32_t*)ID2)) >> 8;
    id[0] = ((*(uint32_t*)ID2));
}

void SystemClockConfig(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
    RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        assert_param(FAIL);
    }

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                   RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        assert_param(FAIL);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        assert_param(FAIL);
    }

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /*    HAL_NVIC_GetPriorityGrouping*/
//    HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void SystemClockReConfig(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE */
    __HAL_RCC_HSE_CONFIG(RCC_HSE_ON);

    /* Wait till HSE is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
    {
    }

    /* Enable PLL */
    __HAL_RCC_PLL_ENABLE();

    /* Wait till PLL is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    {
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

uint8_t GetBoardPowerSource(void)
{
#if defined( USE_USB_CDC )
    if(GpioRead(&UsbDetect) == 1)
    {
        return BATTERY_POWER;
    }
    else
    {
        return USB_POWER;
    }
#else
    return BATTERY_POWER;
#endif
}

#ifdef USE_FULL_ASSERT
/*
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while(1)
    {
    }
}
#endif
