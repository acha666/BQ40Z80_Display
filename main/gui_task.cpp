#include "main.h"
#include "lvgl.h"
#include "lvgl_helpers.h"
#include "ui.h"

void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(portTICK_PERIOD_MS);
}
SemaphoreHandle_t xGuiSemaphore;

void vTaskGUI(void *arg)
{
    xGuiSemaphore = xSemaphoreCreateMutex();
    lv_init();          // lvgl内核初始化
    lvgl_driver_init(); // lvgl显示接口初始化

    static lv_disp_draw_buf_t draw_buf;
    lv_color_t *buf1 = reinterpret_cast<lv_color_t *>(heap_caps_malloc(LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t), MALLOC_CAP_DMA));
    lv_color_t *buf2 = reinterpret_cast<lv_color_t *>(heap_caps_malloc(LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(lv_color_t), MALLOC_CAP_DMA));
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LV_HOR_RES_MAX * LV_VER_RES_MAX); /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;         /*A variable to hold the drivers. Must be static or global.*/
    lv_disp_drv_init(&disp_drv);           /*Basic initialization*/
    disp_drv.draw_buf = &draw_buf;         /*Set an initialized buffer*/
    disp_drv.flush_cb = disp_driver_flush; /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = 240;                /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = 120;                /*Set the vertical resolution in pixels*/
    lv_disp_drv_register(&disp_drv);       /*Register the driver and save the created display objects*/
    esp_register_freertos_tick_hook(reinterpret_cast<esp_freertos_tick_cb_t>(lv_tick_task));

    ui_init();
    xTaskCreatePinnedToCore(vTaskGUIUpdate, "GUI Update Task", 1024 * 3, NULL, 2, NULL, 0);

    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            lv_timer_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }
}

void vTaskGUIUpdate(void *arg)
{
    BQ40Z80_Data received_data;
    while (1)
    {
        if (xQueueReceive(myQueue, &received_data, portMAX_DELAY) == pdTRUE)
        {
            char buf[64];
            sprintf(buf, "%05.2f", (float)received_data.voltage / 1000);
            lv_label_set_text(ui_Voltage, buf);
            sprintf(buf, "%05.2f", (float)received_data.current / 1000);
            lv_label_set_text(ui_Current, buf);

            sprintf(buf, "%05.2f", (float)received_data.capacity / 100);
            lv_label_set_text(ui_Capacity, buf);

            sprintf(buf, "%03d", received_data.rsoc);
            lv_label_set_text(ui_RSOC, buf);

            lv_arc_set_value(ui_Arc1, received_data.rsoc);
        }
    }
}