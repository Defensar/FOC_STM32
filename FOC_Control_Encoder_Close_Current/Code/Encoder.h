#ifndef Encoder_H
#define Encoder_H

#include "stdint.h"
/**
 * @brief 编码器结构体
 * 
 */
typedef struct 
{
    //编码器参数
    uint8_t Motorpn;     //极对数
    uint32_t Line;       //编码器位数
    uint8_t Dir;         //编码器方向

    //编码器数据与误差
    uint32_t Pulse_Data; //编码器原始数据
    uint32_t Phase;      //编码器误差

    //编码器角度
    float Theta;         //编码器角度
    float Theta_e;       //电角度


}Encoder_Typedef;

/**
 * @brief 编码器角度模式状态枚举
 * 
 */
typedef enum
{
    Theta_Mode_Zero = 0,//角度为0
    Theta_Mode_Vf,      //Vf角度自增
    Theta_Mode_Encoder, //编码器角度
}Encoder_Mode_Typedef;



void Encoder_Para_Init(Encoder_Typedef *Encoder_Para,uint32_t line,uint8_t dir,uint8_t motorpn,uint32_t phase);
void Encoder_Para_Run(Encoder_Typedef *Encoder_Para,uint32_t Pulse_Data);

#endif


