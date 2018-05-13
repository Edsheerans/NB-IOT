#ifndef __HMC5883L_H__
#define __HMC5883L_H__

#include "main.h"

void hmc5883l_init(void);

void hmc5883l_single_read(void);

float hmc5883l_get_data(int16_t *data);

int16_t convert_data(uint8_t up,uint8_t low);

#endif
