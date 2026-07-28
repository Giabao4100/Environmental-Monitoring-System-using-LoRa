#ifndef LORASENDVALUE_H
#define LORASENDVALUE_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

void lora_send_sensor_value(UART_HandleTypeDef *huart, float temp, float hum, float ppm, uint8_t tt);

#endif