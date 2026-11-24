#include "myadc.h"
#include "adc.h"
#include "state_machine.h"
void ADC_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADCEx_InjectedStart(&hadc2);
    HAL_ADCEx_InjectedStart_IT(&hadc1);
    
}

void ADC_RUN(myadc_Typedef *myadc_getvalue)
{
//    myadc_getvalue->Va_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_3);
//    myadc_getvalue->Vb_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
//    myadc_getvalue->Vc_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_3);
    myadc_getvalue->Ic_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
    myadc_getvalue->Ib_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
    myadc_getvalue->Ia_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
	myadc_getvalue->VBUS_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_2);

    myadc_getvalue->Ia = ((myadc_getvalue->Ia_Raw - 2048) * (3.3f / 4096)) / (0.001f * 20);
    myadc_getvalue->Ib = ((myadc_getvalue->Ib_Raw - 2048) * (3.3f / 4096)) / (0.001f * 20);
    myadc_getvalue->Ic = ((myadc_getvalue->Ic_Raw - 2048) * (3.3f / 4096)) / (0.001f * 20);
//    myadc_getvalue->Va = myadc_getvalue->Va_Raw * (3.3f / 4096) * 21;
//    myadc_getvalue->Vb = myadc_getvalue->Vb_Raw * (3.3f / 4096) * 21;
//    myadc_getvalue->Vc = myadc_getvalue->Vc_Raw * (3.3f / 4096) * 21;
	myadc_getvalue->VBUS = myadc_getvalue->VBUS_Raw * (3.3f / 4096) * 21;
}
