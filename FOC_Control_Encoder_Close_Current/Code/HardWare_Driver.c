#include "tim.h"
#include "spi.h"
#include "HardWare_Driver.h"
void Motor_HardWare_Init(void)
{
  /* Only CC4 runs during current offset calibration. Preload zero line voltage. */
  uint16_t neutral = (uint16_t)(__HAL_TIM_GET_AUTORELOAD(&htim1) / 2U);
  Motor_Disable_PWM();
  Motor_SetPWM(neutral, neutral, neutral);
  if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
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

void Motor_SetPWM(uint16_t Tcmp_U,uint16_t Tcmp_V,uint16_t Tcmp_W)
{
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,Tcmp_U);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Tcmp_V);
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,Tcmp_W);
  //TIM1->CCR1 = FOC_Parameter->Tcmp_U;
  //TIM1->CCR2 = FOC_Parameter->Tcmp_V;
  //TIM1->CCR3 = FOC_Parameter->Tcmp_W;
}


uint16_t MT6701_ReadData(void)
{
    uint8_t rx[3] = {0};
    uint8_t tx[3] = {0};
    
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 3, 0xffffff);    	
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);  
    return ((rx[0] << 6) | (rx[1] >> 2)) & 0x3FFF;
}
