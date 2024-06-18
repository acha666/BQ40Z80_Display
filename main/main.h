#ifndef __MAIN_H
#define __MAIN_H

/************** Includes ***************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_freertos_hooks.h"
#include "esp_heap_caps.h"
#include "driver/spi_master.h"


void vTaskGUI(void *arg);
void vTaskGUIUpdate(void *arg);
void vTaskI2C(void *arg);
void vTaskTouch(void *arg);

extern QueueHandle_t xGuiUpdateQueue;
extern SemaphoreHandle_t xGuiSemaphore;

typedef struct
{
    uint16_t voltage;
    uint16_t current;
    uint16_t fcc;
    uint16_t capacity;
    uint16_t rsoc;
    uint16_t remaining_time;
} BQ40Z80_Data;

#endif