#include "main.h"

QueueHandle_t myQueue;

extern "C" void app_main(void)
{
    myQueue = xQueueCreate(10, sizeof(BQ40Z80_Data));

    xTaskCreatePinnedToCore(vTaskGUI, "GUI Task", 1024 * 3, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(vTaskI2C, "I2C Task", 1024 * 3, NULL, 2, NULL, 0);
    
    // xTaskCreatePinnedToCore(touch_task, "touch task", 1024 * 3, NULL, 2, NULL, 0);
}
