/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Bleeper board UART driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "board.h"

#include "uart-board.h"

UART_HandleTypeDef Uart1Handle;
UART_HandleTypeDef Uart2Handle;
uint8_t Uart1init = false;
uint8_t Uart2init = false;
uint8_t RxData = 0;
uint8_t TxData = 0;
Uart_t Uart1;
Uart_t Uart2;
#if defined( USE_USB_CDC )
Uart_t UartUsb;
#endif


void UartMcuInit(Uart_t *obj, uint8_t uartId, PinNames tx, PinNames rx)
{
    /** avoid uart port inited twice **/
    if((UART_1 == uartId) && (!Uart1init))
    {
        obj->UartId = uartId;
        Uart1init = true;

        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        __HAL_RCC_USART1_CLK_ENABLE();

        GpioInit(&obj->Tx, tx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF4_USART1);
        GpioInit(&obj->Rx, rx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF4_USART1);
    }
    else if((UART_2 == uartId) && (!Uart2init))
    {
        obj->UartId = uartId;
        Uart2init = true;

        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        __HAL_RCC_USART2_CLK_ENABLE();

        GpioInit(&obj->Tx, tx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF4_USART2);
        GpioInit(&obj->Rx, rx, PIN_ALTERNATE_FCT, PIN_PUSH_PULL, PIN_PULL_UP, GPIO_AF4_USART2);
    }
}

void UartMcuConfig(Uart_t *obj, UartMode_t mode, uint32_t baudrate, WordLength_t wordLength, StopBits_t stopBits, Parity_t parity, FlowCtrl_t flowCtrl)
{
    UART_HandleTypeDef * UartHandle;
    IRQn_Type USART_IRQn;

    if(UART_1 == obj->UartId)
    {
        UartHandle = &Uart1Handle;
        UartHandle->Instance = USART1;
        USART_IRQn = USART1_IRQn;
    }
    else if(UART_2 == obj->UartId)
    {
        UartHandle = &Uart2Handle;
        UartHandle->Instance = USART2;
        USART_IRQn = USART2_IRQn;
    }
    else
        return;//avoid double init

    UartHandle->Init.BaudRate = baudrate;

    if(mode == TX_ONLY)
    {
        if(obj->FifoTx.Data == NULL)
        {
            assert_param(FAIL);
        }
        UartHandle->Init.Mode = UART_MODE_TX;
    }
    else if(mode == RX_ONLY)
    {
        if(obj->FifoRx.Data == NULL)
        {
            assert_param(FAIL);
        }
        UartHandle->Init.Mode = UART_MODE_RX;
    }
    else if(mode == RX_TX)
    {
        if((obj->FifoTx.Data == NULL) || (obj->FifoRx.Data == NULL))
        {
            assert_param(FAIL);
        }
        UartHandle->Init.Mode = UART_MODE_TX_RX;
    }
    else
    {
        assert_param(FAIL);
    }

    if(wordLength == UART_8_BIT)
    {
        UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
    }
    else if(wordLength == UART_9_BIT)
    {
        UartHandle->Init.WordLength = UART_WORDLENGTH_9B;
    }

    switch(stopBits)
    {
    case UART_2_STOP_BIT:
        UartHandle->Init.StopBits = UART_STOPBITS_2;
        break;
    case UART_1_STOP_BIT:
    default:
        UartHandle->Init.StopBits = UART_STOPBITS_1;
        break;
    }

    if(parity == NO_PARITY)
    {
        UartHandle->Init.Parity = UART_PARITY_NONE;
    }
    else if(parity == EVEN_PARITY)
    {
        UartHandle->Init.Parity = UART_PARITY_EVEN;
    }
    else
    {
        UartHandle->Init.Parity = UART_PARITY_ODD;
    }

    if(flowCtrl == NO_FLOW_CTRL)
    {
        UartHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    }
    else if(flowCtrl == RTS_FLOW_CTRL)
    {
        UartHandle->Init.HwFlowCtl = UART_HWCONTROL_RTS;
    }
    else if(flowCtrl == CTS_FLOW_CTRL)
    {
        UartHandle->Init.HwFlowCtl = UART_HWCONTROL_CTS;
    }
    else if(flowCtrl == RTS_CTS_FLOW_CTRL)
    {
        UartHandle->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
    }

    UartHandle->Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(UartHandle) != HAL_OK)
    {
        assert_param(FAIL);
    }

    HAL_NVIC_SetPriority(USART_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART_IRQn);

    /* Enable the UART Data Register not empty Interrupt */
    HAL_UART_Receive_IT(UartHandle, &RxData, 1);
}

void UartMcuDeInit(Uart_t *obj)
{
    /** avoid uart port deinited twice **/
    if((UART_1 == obj->UartId) && (Uart1init))
    {
        Uart1init = false;
        obj->UartId = 0;

        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
        __HAL_RCC_USART1_CLK_DISABLE();

        GpioInit(&obj->Tx, obj->Tx.pin, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0);
        GpioInit(&obj->Rx, obj->Rx.pin, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0);
    }
    else if((UART_2 == obj->UartId) && (Uart2init))
    {
        Uart2init = false;
        obj->UartId = 0;

        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
        __HAL_RCC_USART2_CLK_DISABLE();

        GpioInit(&obj->Tx, obj->Tx.pin, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0);
        GpioInit(&obj->Rx, obj->Rx.pin, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0);
    }
}

uint8_t UartMcuPutChar(Uart_t *obj, uint8_t data)
{
    UART_HandleTypeDef * UartHandle;

    if(UART_1 == obj->UartId)
        UartHandle = &Uart1Handle;
    else if(UART_2 == obj->UartId)
        UartHandle = &Uart2Handle;
    else
        return 0;

    __disable_irq();
    TxData = data;

    if(IsFifoFull(&obj->FifoTx) == false)
    {
        FifoPush(&obj->FifoTx, data);

        // Trig UART Tx interrupt to start sending the FIFO contents.

        __HAL_UART_ENABLE_IT(UartHandle, USART_IT_TC);
        __enable_irq();
        return 0; // OK
    }
    __enable_irq();

    return 1; // Busy
}

uint8_t UartMcuGetChar(Uart_t *obj, uint8_t *data)
{
    __disable_irq();

    if(IsFifoEmpty(&obj->FifoRx) == false)
    {

        *data = FifoPop(&obj->FifoRx);
        __enable_irq();
        return 0;
    }
    __enable_irq();
    return 1;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle)
{
    Uart_t * Uart;

    if(USART1 == handle->Instance)
        Uart = &Uart1;
    else if(USART2 == handle->Instance)
        Uart = &Uart2;

    if(IsFifoEmpty(&(Uart->FifoTx)) == false)
    {
        TxData = FifoPop(&(Uart->FifoTx));
        //  Write one byte to the transmit data register
        HAL_UART_Transmit_IT(handle, &TxData, 1);
    }

    if(Uart->IrqNotify != NULL)
    {
        Uart->IrqNotify(UART_NOTIFY_TX);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
    Uart_t * Uart;

    if(USART1 == handle->Instance)
        Uart = &Uart1;
    else if(USART2 == handle->Instance)
        Uart = &Uart2;

    if(IsFifoFull(&(Uart->FifoRx)) == false)
    {
        // Read one byte from the receive data register
        FifoPush(&(Uart->FifoRx), RxData);
    }
    if(Uart->IrqNotify != NULL)
    {
        Uart->IrqNotify(UART_NOTIFY_RX);
    }

    HAL_UART_Receive_IT(handle, &RxData, 1);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *handle)
{
    HAL_UART_Receive_IT(handle, &RxData, 1);
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&Uart1Handle);
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&Uart2Handle);
}

