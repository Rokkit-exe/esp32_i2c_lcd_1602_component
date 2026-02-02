#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>
#include "esp_err.h"         // Needed for esp_err_t
#include "driver/i2c_master.h"

typedef struct {
    uint8_t lcd_addr;                   // I2C address of the LCD
    int master_sda_io;                      // GPIO pin for SDA (optional)
    int master_scl_io;                  // GPIO pin for SCL (optional)
    int master_freq_hz;                // I2C clock frequency (optional)
} i2c_lcd_config_t;

/**
 * @brief Initialize the LCD using an existing I2C bus handle
 */
esp_err_t i2c_lcd_init(const i2c_lcd_config_t *conf);

/**
 * @brief Clears the display and returns cursor to (0,0)
 */
esp_err_t lcd_clear(void);

/**
 * @brief Sends a single command byte to the LCD
 * @param cmd The command byte to send
 */
esp_err_t lcd_send_command(uint8_t cmd);

/**
 * @brief Sends a full 16-character row atomically
 * @param row 0 for first line, 1 for second line
 * @param str The string to display (auto-padded to 16 chars)
 */
esp_err_t lcd_send_row(uint8_t row, const char *str);

#endif // LCD_I2C_H

