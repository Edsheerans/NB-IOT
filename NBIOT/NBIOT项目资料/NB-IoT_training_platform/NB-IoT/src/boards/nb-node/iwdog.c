#include <math.h>
#include "board.h"

static IWDG_HandleTypeDef   IwdgHandle;

void iwdog_init()
{
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
    {
        printf("###system reset by IWDG###\n");
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    /*Counter Reload Value = 250ms/IWDG counter clock period
    												= 0.25s / (32/LsiFreq)
    												= LsiFreq/(32 * 4)
    												= LsiFreq/128 */

    //uwLsiFreq = 40000;		//40KHz,discreet value!!!
    //uwLsiFreq = GetLSIFrequency();

    IwdgHandle.Instance = IWDG;
    //IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
    //IwdgHandle.Init.Reload = uwLsiFreq / 128;
    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
    IwdgHandle.Init.Reload = 0xffff;
    IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;

    HAL_IWDG_Init(&IwdgHandle);
    HAL_IWDG_Start(&IwdgHandle);
}

void iwdog_refresh()
{
    HAL_IWDG_Refresh(&IwdgHandle);
}
