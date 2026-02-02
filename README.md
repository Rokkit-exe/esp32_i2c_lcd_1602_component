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
i2c_lcd_config_t cfg = {
    .lcd_addr = LCD_ADDR
    .master_sda_io = I2C_MASTER_SDA_IO,
    .master_scl_io = I2C_MASTER_SCL_IO,
    .master_freq_hz = I2C_MASTER_FREQ_HZ,
};
i2c_lcd_init(&cfg);

// Print text on the LCD
i2c_send_row(0, "Hello, World!");
i2c_send_row(1, "ESP32 I2C LCD");
```
