#include "board.h"

TIM_HandleTypeDef tim6_handle;
TIM_HandleTypeDef tim21_handle;
static tim_elapsed_cb tim6_elapsed_cb = NULL;
static tim_elapsed_cb tim21_elapsed_cb = NULL;

void tim_init(tim_id_t id)    
{
    uint32_t uwPrescalerValue = 0;
    if(id == tim21)
    {
        __HAL_RCC_TIM21_CLK_ENABLE();
        uwPrescalerValue = (uint32_t) ((SystemCoreClock / 10000) - 1);
        tim21_handle.Instance = TIM21;
        tim21_handle.Init.Period = 10 - 1;
        tim21_handle.Init.Prescaler = uwPrescalerValue;
        tim21_handle.Init.ClockDivision = 0;
        tim21_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
        HAL_NVIC_SetPriority(TIM21_IRQn, 0, 3);
        HAL_NVIC_EnableIRQ(TIM21_IRQn);
        HAL_TIM_Base_Init(&tim21_handle);
        HAL_TIM_Base_Start_IT(&tim21_handle);
    }
    else if(id == tim6)
    {
        uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock / 1000000) - 1);

        __HAL_RCC_TIM6_CLK_ENABLE();
        tim6_handle.Instance = TIM6;
        tim6_handle.Init.Period = 103;
        tim6_handle.Init.Prescaler = uwPrescalerValue;
        tim6_handle.Init.ClockDivision = 0;
        tim6_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
        HAL_NVIC_SetPriority(TIM6_IRQn, 0, 2);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
        HAL_TIM_Base_Init(&tim6_handle);
        HAL_TIM_Base_Start_IT(&tim6_handle);
        HAL_TIM_Base_Stop_IT(&tim6_handle);
    }
}

void set_tim_cb(tim_id_t id, tim_elapsed_cb cb)
{
    if(id == tim21)
    {
        tim21_elapsed_cb = cb;
    }
    else if(id == tim6)
    {
        tim6_elapsed_cb = cb;
    }
}

void TIM21_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&tim21_handle);
}

void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&tim6_handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &tim6_handle)
    {
        if(tim6_elapsed_cb != NULL)
            tim6_elapsed_cb();
    }
    else if(htim == &tim21_handle)
    {
        if(tim21_elapsed_cb != NULL)
            tim21_elapsed_cb();
    }
}
