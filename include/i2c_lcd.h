#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <stdint.h>
#include "esp_err.h"         // Needed for esp_err_t
#include "driver/i2c_master.h"


/**
 * @brief Initialize the LCD using an existing I2C bus handle
 */
esp_err_t lcd_i2c_init(i2c_master_bus_config_t * bus_cfg, i2c_device_config_t * dev_cfg);

/**
 * @brief Clears the display and returns cursor to (0,0)
 */
void lcd_clear(void);

/**
 * @brief Sends a full 16-character row atomically
 * @param row 0 for first line, 1 for second line
 * @param str The string to display (auto-padded to 16 chars)
 */
esp_err_t lcd_send_row(uint8_t row, const char *str);

#endif // LCD_I2C_H

