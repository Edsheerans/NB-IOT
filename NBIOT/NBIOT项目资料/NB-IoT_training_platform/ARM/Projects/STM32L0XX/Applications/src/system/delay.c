#include "main.h"

void DelayMs(uint32_t ms)
{
    HAL_Delay(ms);
}

void Delay(float s)
{
    DelayMs(s * 1000.0f);
}
