# esp32_i2c_lcd_1602_component

ESP-IDF library to interact with I2C LCD 1602 display

# Overview

This repository contains an ESP-IDF component that allows you to interface with a 1602 LCD display using the I2C protocol.
The component provides functions to initialize the display, send commands, and print text.

# Features

- Initialize the 1602 LCD display over I2C
- Send commands to control the display
- Print text on the display
- Compatible with ESP-IDF framework

# Requirements

- ESP-IDF v4.0 or later
- I2C LCD 1602 display module
- ESP32 development board

# API

```c
#include "i2c_lcd.h"

#define I2C_MASTER_SCL_IO           22          /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           21          /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_FREQ_HZ          100000      /*!< I2C master clock frequency */
#define LCD_ADDR                    0x27        /*!< I2C address of the LCD [0x27 or 0x3F] */
// Initialize the LCD
i2c_master_bus_config_t bus_cfg = {
    .i2c_port = -1,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
};

i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = LCD_ADDR,
    .scl_speed_hz = I2C_MASTER_FREQ_HZ,
};
i2c_lcd_init(&bus_cfg, &dev_cfg);

// Print text on the LCD
i2c_send_row(0, "Hello, World!");
i2c_send_row(1, "ESP32 I2C LCD");
```
