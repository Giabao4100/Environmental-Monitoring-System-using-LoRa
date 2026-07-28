#include "MQ2.h"
#include "cmsis_os.h"

#define ADC_REF 3.3f

static uint16_t MQ2_ReadADC(MQ2_Typedef *mq2)
{
    uint32_t sum = 0;
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = mq2->channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(mq2->hadc, &sConfig);
    for(int i = 0; i < 8; i++)
    {
        HAL_ADC_Start(mq2->hadc);
        if(HAL_ADC_PollForConversion(mq2->hadc, 100) == HAL_OK)
        {
            sum += HAL_ADC_GetValue(mq2->hadc);
        }
        HAL_ADC_Stop(mq2->hadc);
        osDelay(2);
    }
    return (uint16_t)(sum / 8);
}

static float MQ2_CalcRs(uint16_t adc)
{
    if(adc == 0)
        return -1;
    float vout = ((float)adc * ADC_REF) / 4095.0f;
    if(vout <= 0.01f)
        return -1;
    return RL * ((VCC / vout) - 1.0f);
}

void mq2_init(MQ2_Typedef *mq2,
              ADC_HandleTypeDef *hadc,
              uint32_t channel)
{
    mq2->hadc = hadc;
    mq2->channel = channel;
    mq2->adc_value = 0;
    mq2->ppm_value = 0;
    mq2->R0 = 0;
    mq2->ready = 0;
    mq2->warmup_tick = HAL_GetTick();
}

void calibrate_mq2(MQ2_Typedef *mq2)
{
    float rs_sum = 0;
    uint16_t valid = 0;
    for(int i = 0; i < 20; i++)
    {
        uint16_t adc = MQ2_ReadADC(mq2);
        float rs = MQ2_CalcRs(adc);
        if(rs > 0)
        {
            rs_sum += rs;
            valid++;
        }
        osDelay(50);
    }
    if(valid)
    {
        float rs_air = rs_sum / valid;
        mq2->R0 = rs_air / 9.83f;
    }
}

float mq2_get_ppm(MQ2_Typedef *mq2)
{
    if(mq2->R0 <= 0)
        return -1;
    uint16_t adc = MQ2_ReadADC(mq2);
    mq2->adc_value = adc;
    float rs = MQ2_CalcRs(adc);
    if(rs <= 0)
        return -1;
    float ratio = rs / mq2->R0;
    return A * powf(ratio, B);
}

void mq2_handle(MQ2_Typedef *mq2)
{
    if(mq2->ready == 0)
    {
        if(HAL_GetTick() - mq2->warmup_tick >= 10000)
        {
            calibrate_mq2(mq2);
            mq2->ready = 1;
        }
        return;
    }
    float ppm = mq2_get_ppm(mq2);
    if(ppm > 0)
    {
        mq2->ppm_value = 0.4f * mq2->ppm_value + 0.6f * ppm;

    }
}