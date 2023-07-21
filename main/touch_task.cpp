#include "main.h"
#include "driver/touch_sensor.h"
#include "driver/gpio.h"

#define TAG "touch_test"

void vTaskTouch(void *arg)
{
    touch_pad_init();
    touch_pad_config(TOUCH_PAD_NUM8);
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_0V);
    touch_pad_denoise_t denoise = {
        /* The bits to be cancelled are determined according to the noise level. */
        .grade = TOUCH_PAD_DENOISE_BIT4,
        .cap_level = TOUCH_PAD_DENOISE_CAP_L4,
    };
    touch_pad_denoise_set_config(&denoise);
    touch_pad_denoise_enable();
    ESP_LOGI(TAG, "Denoise function init");

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_fsm_start();

    uint32_t value;
    while (1)
    {

        touch_pad_read_raw_data(TOUCH_PAD_NUM8, &value); // 读取原始数据
        ESP_LOGI(TAG, "raw data = %lu", value);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}