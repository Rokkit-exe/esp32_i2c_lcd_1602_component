#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>
#include "esp_err.h"         // Needed for esp_err_t
#include "driver/i2c_master.h"

/**
 * @brief Configuration structure for LCD initialization
 */
typedef struct {
    i2c_master_bus_handle_t bus_handle; // The handle of the initialized I2C bus
    uint8_t lcd_addr;                   // Usually 0x27
} lcd_i2c_config_t;

/**
 * @brief Initialize the LCD using an existing I2C bus handle
 */
esp_err_t lcd_i2c_init(const lcd_i2c_config_t *conf);

/**
 * @brief Clears the display and returns cursor to (0,0)
 */
esp_err_t lcd_clear(void);

/**
 * @brief Sends a full 16-character row atomically
 * @param row 0 for first line, 1 for second line
 * @param str The string to display (auto-padded to 16 chars)
 */
esp_err_t lcd_send_row(uint8_t row, const char *str);

#endif // LCD_I2C_H

