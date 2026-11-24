#ifndef VOFA_H
#define VOFA_H


#include "stdint.h"
#include "string.h"
#include "usbd_cdc_if.h"
/*================== 宏定义 ==================*/
#define data_num  10                      /* 定义发送数量 */
#define data_len  (data_num * 4 + 4)     /* 定义发送长度 */


/*================== 结构体 ==================*/
typedef struct 
{
    float Send_Data_Array[data_num];        
    uint8_t Byte_Data_Array[data_len];      
}VofaSend_TypeDef;


/*================== 变量声明 ==================*/
extern VofaSend_TypeDef VofaSend;


/*================== 函数声明 ==================*/
void Vofa_Send_Task(void);
void Vofa_Data_Process(void);

#endif
