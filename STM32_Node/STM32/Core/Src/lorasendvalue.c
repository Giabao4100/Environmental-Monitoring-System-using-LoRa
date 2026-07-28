#include "lorasendvalue.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

void lora_send_sensor_value(UART_HandleTypeDef *huart, float temp, float hum, float ppm, uint8_t tt)
{
    char buff[128];
    int length;

    // Log 1: Sending
    length = snprintf(buff, sizeof(buff),
             "LOG:Sending LoRa packet: NodeId:1, Temp:%.1f, Humidity:%.1f, PPM:%.2f, TT:%d\r\n",
             temp, hum, ppm, tt);
    HAL_UART_Transmit(huart, (uint8_t*)buff, length, 1000);
    osDelay(50);  // tang t? 10 lên 100ms


    length = snprintf(buff, sizeof(buff),
             "temp:%.2f;hum:%.2f;ppm:%.2f;tt:%d\r\n",
             temp, hum, ppm, tt);
    HAL_UART_Transmit(huart, (uint8_t*)buff, length, 1000);
    osDelay(50);  

}
