#ifndef __BOARD_H__
#define __BOARD_H__

#include "main.h"

void system_clock_config(void);
void io_interrupt_config(void);

void adc1_gpio_init(ADC_HandleTypeDef *hadc);
void adc1_init(void);
void adc1_set_channel0(void);
void adc1_start(void);
void adc1_stop(void);


void EXTI4_15_IRQHandler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif
