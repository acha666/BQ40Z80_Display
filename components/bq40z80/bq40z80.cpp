#ifdef __cplusplus
extern "C"
{
#endif
#include "bq40z80.h"
    BQ40Z80::BQ40Z80(uint8_t i2c_scl_io, uint8_t i2c_sda_io, i2c_port_t i2c_master_num, uint8_t device_address)
    {
        this->DEVICE_ADDRESS = device_address;
        this->I2C_MASTER_NUM = i2c_master_num;

        i2c_config_t conf;
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = i2c_sda_io;
        conf.scl_io_num = i2c_scl_io;
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
        conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

        i2c_param_config(i2c_master_num, &conf);

        ESP_ERROR_CHECK(i2c_driver_install(i2c_master_num, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0));
    }

    /***************************** Public Functions *****************************/
    uint16_t BQ40Z80::get_temperature()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_Current, &buf));
        return buf - 2732; // raw data unit: 0.1 Kelvin
    }

    uint16_t BQ40Z80::get_voltage()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_Voltage, &buf));
        return buf;
    }

    uint16_t BQ40Z80::get_current()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_Current, &buf));
        return buf;
    }

    uint8_t BQ40Z80::get_rsoc()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_RelativeStateOfCharge, &buf));
        return buf;
    }

    uint16_t BQ40Z80::get_remaining_capacity()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_RemainingCapacity, &buf));
        return buf;
    }

    uint16_t BQ40Z80::get_fcc()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_FullChargeCapacity, &buf));
        return buf;
    }

    uint16_t BQ40Z80::get_battery_mode()
    {
        uint16_t buf;
        ESP_ERROR_CHECK(this->read_word(BQ40Z80_SBS_BatteryMode, &buf));
        return buf;
    }

    void BQ40Z80::set_battery_mode(u_int16_t val)
    {
        ESP_ERROR_CHECK(this->write_word(BQ40Z80_SBS_BatteryMode, val));
        this->update_basic_info();
    }

    void BQ40Z80::set_capm(bool val)
    {
        uint16_t buf = this->get_battery_mode();
        if (val)
            buf |= 0x8000;
        else
            buf &= 0x7fff;
        this->set_battery_mode(buf);
    }

    void BQ40Z80::update_basic_info()
    {
        uint16_t buf;
        buf = this->get_battery_mode();
        this->CAPM = (buf >> 15) & 0x0001;
    }

    /***************************** Private Functions *****************************/
    // esp_err_t BQ40Z80::i2c_send_byte(uint8_t data)
    // {
    //     esp_err_t err;

    //     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    //     i2c_master_start(cmd);
    //     i2c_master_write_byte(cmd, (this->DEVICE_ADDRESS << 1) | I2C_MASTER_WRITE, 1);
    //     i2c_master_write_byte(cmd, data, 1);
    //     i2c_master_stop(cmd);

    //     err = i2c_master_cmd_begin(this->I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_TICK);
    //     i2c_cmd_link_delete(cmd);

    //     return err;
    // }

    esp_err_t BQ40Z80::read_word(uint8_t reg_addr, uint16_t *data)
    {
        esp_err_t err;
        uint8_t buf[2];
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (this->DEVICE_ADDRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, reg_addr, I2C_MASTER_ACK);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (this->DEVICE_ADDRESS << 1) | I2C_MASTER_READ, 1);
        i2c_master_read(cmd, buf, 2, I2C_MASTER_ACK);
        i2c_master_stop(cmd);

        err = i2c_master_cmd_begin(this->I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_TICK);
        i2c_cmd_link_delete(cmd);

        *data = (buf[1] << 8) | buf[0];

        return err;
    }

    esp_err_t BQ40Z80::write_word(uint8_t reg_addr, uint16_t data)
    {
        esp_err_t err;
        uint8_t buf[2];
        buf[1] = data >> 8;
        buf[0] = data & 0x00FF;

        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (this->DEVICE_ADDRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK);
        i2c_master_write_byte(cmd, reg_addr, I2C_MASTER_ACK);
        i2c_master_write(cmd, buf, 2, I2C_MASTER_ACK);
        i2c_master_stop(cmd);

        err = i2c_master_cmd_begin(this->I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_TICK);
        i2c_cmd_link_delete(cmd);

        return err;
    }

#ifdef __cplusplus
}
#endif