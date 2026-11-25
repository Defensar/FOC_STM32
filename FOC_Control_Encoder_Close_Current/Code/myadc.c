#include "myadc.h"
#include "adc.h"

#define ADC_OFFSET_SETTLE_SAMPLES 32U
#define ADC_OFFSET_AVERAGE_SAMPLES 256U

static void ADC_ResetOffset(myadc_Typedef *adc)
{
    adc->Ia_Offset = 0.0f;
    adc->Ib_Offset = 0.0f;
    adc->Ic_Offset = 0.0f;
    adc->OffsetSampleCount = 0U;
    adc->OffsetReady = 0U;
}

void ADC_Init(myadc_Typedef *adc)
{
    ADC_ResetOffset(adc);
    if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK ||
        HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) != HAL_OK ||
        HAL_ADCEx_InjectedStart(&hadc2) != HAL_OK ||
        HAL_ADCEx_InjectedStart_IT(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* Called once per injected sequence, with all three phase outputs disabled. */
void ADC_CalibrateOffset(myadc_Typedef *adc)
{
    if (adc->Ia_Raw == 0U || adc->Ia_Raw >= 4095U ||
        adc->Ib_Raw == 0U || adc->Ib_Raw >= 4095U ||
        adc->Ic_Raw == 0U || adc->Ic_Raw >= 4095U)
    {
        ADC_ResetOffset(adc);
        return;
    }

    adc->OffsetSampleCount++;
    if (adc->OffsetSampleCount <= ADC_OFFSET_SETTLE_SAMPLES)
    {
        return;
    }

    adc->Ia_Offset += adc->Ia_Raw;
    adc->Ib_Offset += adc->Ib_Raw;
    adc->Ic_Offset += adc->Ic_Raw;
    if (adc->OffsetSampleCount == ADC_OFFSET_SETTLE_SAMPLES + ADC_OFFSET_AVERAGE_SAMPLES)
    {
        adc->Ia_Offset /= ADC_OFFSET_AVERAGE_SAMPLES;
        adc->Ib_Offset /= ADC_OFFSET_AVERAGE_SAMPLES;
        adc->Ic_Offset /= ADC_OFFSET_AVERAGE_SAMPLES;
        adc->OffsetReady = 1U;
    }
}
void ADC_RUN(myadc_Typedef *myadc_getvalue)
{
    myadc_getvalue->Ic_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_1);
    myadc_getvalue->Ib_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_2);
    myadc_getvalue->Ia_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1);
	myadc_getvalue->VBUS_Raw = (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2,ADC_INJECTED_RANK_2);

    myadc_getvalue->VBUS = myadc_getvalue->VBUS_Raw * (3.3f / 4096) * 21;
    if (myadc_getvalue->OffsetReady)
    {
        const float amps_per_count = (3.3f / 4096.0f) / (0.001f * 20.0f);
        myadc_getvalue->Ia = (myadc_getvalue->Ia_Raw - myadc_getvalue->Ia_Offset) * amps_per_count;
        myadc_getvalue->Ib = (myadc_getvalue->Ib_Raw - myadc_getvalue->Ib_Offset) * amps_per_count;
        myadc_getvalue->Ic = (myadc_getvalue->Ic_Raw - myadc_getvalue->Ic_Offset) * amps_per_count;
    }
}

void ADC_Transform_Value(myadc_Typedef *myadc_getvalue,FOC_Parameter_TypeDef *FOC)
{
    FOC->Iu = myadc_getvalue->Ia;
    FOC->Iv = myadc_getvalue->Ib;
    FOC->Iw = myadc_getvalue->Ic;
}
