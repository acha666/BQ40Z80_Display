#include "main.h"
#include "driver/i2c.h"
#include "bq40z80.h"

#define TAG "i2c_task"
void vTaskI2C(void *arg)
{
    BQ40Z80_Data device_data;
    ESP_LOGI(TAG, "Initlizing I2C");
    auto device = BQ40Z80(17, 16, I2C_NUM_1);
    ESP_LOGI(TAG, "Done");

    device.set_capm(1);

    device_data.fcc = device.get_full_charge_capacity();
    while (1)
    {
        device_data.voltage = device.get_voltage();
        device_data.current = device.get_current();
        device_data.capacity = device.get_remaining_capacity();
        device_data.rsoc = device.get_rsoc();
        // ESP_LOGI(TAG, "Voltage: %d mV", device.get_voltage());
        // ESP_LOGI(TAG, "Current: %d mA", device.get_current());
        // ESP_LOGI(TAG, "Remaining Capacity: %d mAh", device.get_remaining_capacity());
        // ESP_LOGI(TAG, "FCC: %d mAh", device.get_fcc());
        xQueueSend(myQueue, &device_data, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}