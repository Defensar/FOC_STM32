#include "headfile.h"

void Motor_HardWare_Init(void)
{
  Motor_Enable_PWM();
  HAL_TIM_Base_Start_IT(&htim6);
}

void Motor_Enable_PWM(void)
{
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_3);
}

void Motor_Disable_PWM(void)
{
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_3);
}

void Motor_SetPWM(FOC_Parameter_TypeDef *FOC_Parameter)
{
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,FOC_Parameter->Tcmp_U);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,FOC_Parameter->Tcmp_V);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,FOC_Parameter->Tcmp_W);
  //TIM1->CCR1 = FOC_Parameter->Tcmp_U;
  //TIM1->CCR2 = FOC_Parameter->Tcmp_V;
  //TIM1->CCR3 = FOC_Parameter->Tcmp_W;
}
