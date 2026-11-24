#ifndef myadc_H
#define myadc_H

#include "stdint.h"
#include "FOC_Algorithm.h"

typedef struct 
{
    uint16_t Ia_Raw;
    uint16_t Ib_Raw;
    uint16_t Ic_Raw;
    uint16_t Va_Raw;
    uint16_t Vb_Raw;
    uint16_t Vc_Raw;
	  uint16_t VBUS_Raw;

    float Ia;
    float Ib;
    float Ic;
    float Va;
    float Vb;
    float Vc;
	  float VBUS;
}myadc_Typedef;



void ADC_Init(void);
void ADC_RUN(myadc_Typedef *myadc_getvalue);
void ADC_Transform_Value(myadc_Typedef *myadc_getvalue,FOC_Parameter_TypeDef *FOC);

#endif
