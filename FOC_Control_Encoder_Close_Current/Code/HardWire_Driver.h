#ifndef HardWare_Driver_H
#define HardWare_Driver_H

#include "headfile.h"
void Motor_HardWire_Init(void);
void Motor_Enable_PWM(void);
void Motor_Disable_PWM(void);
void Motor_SetPWM(FOC_Parameter_TypeDef *FOC_Parameter);



#endif
