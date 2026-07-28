#ifndef MQ2_H
#define MQ2_H

#include "stm32f1xx_hal.h"
#include <math.h>

#define RL      5.0f
#define VCC    3.3f

// LPG Curve
#define A       2800.0f
#define B       -2.2f

typedef struct
{
    ADC_HandleTypeDef *hadc;
    uint32_t channel;

    uint16_t adc_value;
    float ppm_value;

    float R0;

    uint8_t ready;
    uint32_t warmup_tick;

} MQ2_Typedef;

void mq2_init(MQ2_Typedef *mq2,
              ADC_HandleTypeDef *hadc,
              uint32_t channel);

void mq2_handle(MQ2_Typedef *mq2);

void calibrate_mq2(MQ2_Typedef *mq2);

float mq2_get_ppm(MQ2_Typedef *mq2);

#endif