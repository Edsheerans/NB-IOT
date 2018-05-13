#ifndef __TIM_BOARD_H_
#define __TIM_BOARD_H_

typedef enum{
    tim21,
    tim6,
}tim_id_t;

typedef void (*tim_elapsed_cb)(void);

extern TIM_HandleTypeDef tim6_handle;
extern TIM_HandleTypeDef tim21_handle;

void tim_init(tim_id_t tim_id);
void set_tim_cb(tim_id_t id, tim_elapsed_cb cb);
#endif
