#ifndef HardWare_Driver_H
#define HardWare_Driver_H

#include "stdint.h"

void Motor_HardWare_Init(void);
void Motor_Enable_PWM(void);
void Motor_Disable_PWM(void);
void Motor_SetPWM(uint16_t Tcmp_U,uint16_t Tcmp_V,uint16_t Tcmp_W);
uint16_t MT6701_ReadData(void);

#endif
