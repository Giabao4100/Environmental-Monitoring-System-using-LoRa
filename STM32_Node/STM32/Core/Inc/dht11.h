#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f1xx_hal.h"


#define DHT11_PORT GPIOB
#define DHT11_PIN  GPIO_PIN_9

extern float tCelsius;
extern float tFahrenheit;
extern float RH;

void microDelay(uint16_t delay);
uint8_t DHT11_Start(void);
uint8_t DHT11_Read(void);
void DHT11_GetData(void);

#endif