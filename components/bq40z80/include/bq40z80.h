#ifndef __BQ40Z80_H
#define __BQ40Z80_H

// struct bq40z80_battery_status
// {
//     bool OCA, TCA, OTA, TDA, RCA, RTA, INIT, DSG, FC, FD, EC3, EC2, EC1, EC0;
// };

#ifdef __cplusplus
extern "C"
{
#endif

#include "bq40z80_sbs.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_FREQ_HZ 50000   /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_TICK 1000 / portTICK_PERIOD_MS

    class BQ40Z80
    {
    public:
        /**
         * Initlize the I2C bus
         * @param i2c_scl_io GPIO number used for I2C master clock
         * @param i2c_sda_io GPIO number used for I2C master data
         * @param i2c_master_num I2C master i2c port number,
         *                       the number of i2c peripheral interfaces available will depend on the chip,
         *                       such as I2C_NUM_0
         * @param device_address 7-bit address of BQ40Z80 chip, default to 0x0b
         * @return none
         */
        BQ40Z80(uint8_t i2c_scl_io, uint8_t i2c_sda_io, i2c_port_t i2c_master_num, uint8_t device_address = 0x0b);

        bool CAPM;

        /**
         * Read the temperature of device(0x08)
         * @return temperature, unit: 0.1 degree celsius
         */
        uint16_t get_temperature();

        /**
         * Read the battery voltage(0x09)
         * @return voltage, unit: millivolts
         */
        uint16_t get_voltage();

        /**
         * Read the battery current(0x0A)
         * @return voltage, unit: milliamps
         */
        uint16_t get_current();

        /**
         * Read the relative state of charge(RSOC)(0x0D)
         * RSOD is the predicted remaining battery capacity as a percentage of FCC
         * @return RSOC value, unit: percantage
         */
        uint8_t get_rsoc();

        /**
         * Read the predicted remaining battery capacity(0x0F)
         * If BatteryMode()[CAPM] = 0, then the data reports in mAh.
         * If BatteryMode()[CAPM] = 1, then the data reports in cWh.
         * @return Remaining capacity value, unit: mAh/cWh
         */
        uint16_t get_remaining_capacity();

        /**
         * Read the full charge capacity(FCC)(0x10)
         * FCC is the predicted battery capacity when fully charged
         * If BatteryMode()[CAPM] = 0, then the data reports in mAh.
         * If BatteryMode()[CAPM] = 1, then the data reports in cWh.
         * @return Full charge capacity, unit: mAh/cWh
         */
        uint16_t get_fcc();

        /**
         * Set the BatteryMode(0x03)[CAPM]
         * If [CAPM] = 0, then the FCC and RC data reports in mAh.
         * If [CAPM] = 1, then the FCC and RC data reports in cWh.
         * @param val bool value of CAPM bit
         */
        void set_capm(bool val);

        void update_basic_info();

        /**
         * Set the BatteryStatus(0x03)
         * @param val 16-bit value of BatteryStatus
         */
        void set_battery_mode(uint16_t val);

        /**
         * Get the BatteryStatus(0x03)
         * @return 16-bit value of BatteryStatus
         */
        uint16_t get_battery_mode();

    private:
        i2c_port_t I2C_MASTER_NUM;
        uint8_t DEVICE_ADDRESS;

        /**
         * Read a 16-bit SBS Command Result from device
         *
         * @param reg_addr 8-bit SBS Command
         * @param data data buffer
         * @return error code
         */
        esp_err_t read_word(uint8_t reg_addr, uint16_t *data);

        /**
         * Read a 16-bit SBS Command Data to device
         *
         * @param reg_addr 8-bit SBS Command
         * @param data data buffer
         * @return error code
         */
        esp_err_t write_word(uint8_t reg_addr, uint16_t data);
    };

#ifdef __cplusplus
}
#endif
#endif
