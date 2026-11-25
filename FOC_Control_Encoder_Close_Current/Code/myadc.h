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

    float Ia_Offset;  /* ADC counts, valid when OffsetReady is set. */
    float Ib_Offset;
    float Ic_Offset;
    uint16_t OffsetSampleCount;
    uint8_t OffsetReady;
}myadc_Typedef;



void ADC_Init(myadc_Typedef *adc);
void ADC_CalibrateOffset(myadc_Typedef *adc);
void ADC_RUN(myadc_Typedef *myadc_getvalue);
void ADC_Transform_Value(myadc_Typedef *myadc_getvalue,FOC_Parameter_TypeDef *FOC);

#endif
