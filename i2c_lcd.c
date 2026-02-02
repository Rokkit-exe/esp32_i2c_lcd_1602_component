#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/i2c_master.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

#include "i2c_lcd.h"

static const char *TAG = "I2C_LCD";

/* ===================== LCD BITS & COMMANDS ===================== */
#define LCD_RS        0x01
#define LCD_ENABLE    0x04
#define LCD_BACKLIGHT 0x08

#define CMD_CLEAR             0x01
#define CMD_DISPLAY_ON        0x0C
#define CMD_FUNCTION_SET_4BIT 0x28
#define CMD_SET_DDRAM_ADDR    0x80
#define CMD_INIT_8BIT         0x30
#define CMD_INIT_4BIT         0x20
#define CMD_ENTRY             0x06

/* ===================== GLOBALS ===================== */
// These stay static so they are private to this file
static i2c_master_bus_handle_t bus_handle = NULL;
static i2c_master_dev_handle_t lcd_dev = NULL;
static SemaphoreHandle_t i2c_mutex = NULL;

/* ===================== PRIVATE HELPERS ===================== */

static esp_err_t lcd_i2c_tx(uint8_t *data, size_t len) {
    if (!i2c_mutex || !lcd_dev) return ESP_ERR_INVALID_STATE;

    if (xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    esp_err_t err = i2c_master_transmit(lcd_dev, data, len, -1);
    xSemaphoreGive(i2c_mutex);
    return err;
}

static esp_err_t lcd_write_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    uint8_t pkt[2] = { (uint8_t)(data | LCD_ENABLE), (uint8_t)(data & ~LCD_ENABLE) };

    esp_err_t err = lcd_i2c_tx(pkt, sizeof(pkt));
    esp_rom_delay_us(50); 
    return err;
}

static void pack_byte_to_buffer(uint8_t *buf, int *idx, uint8_t val, uint8_t mode) {
    uint8_t high = (val & 0xF0) | mode | LCD_BACKLIGHT;
    buf[(*idx)++] = high | LCD_ENABLE;
    buf[(*idx)++] = high & ~LCD_ENABLE;
    
    uint8_t low = ((val << 4) & 0xF0) | mode | LCD_BACKLIGHT;
    buf[(*idx)++] = low | LCD_ENABLE;
    buf[(*idx)++] = low & ~LCD_ENABLE;
}

static void lcd_send_byte(uint8_t val, uint8_t mode) {
    lcd_write_nibble(val & 0xF0, mode);
    lcd_write_nibble((val << 4) & 0xF0, mode);
}

/* ===================== PUBLIC API ===================== */
void lcd_clear(void) {
    lcd_send_byte(CMD_CLEAR, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
}

esp_err_t lcd_i2c_init(i2c_master_bus_config_t * bus_cfg, i2c_device_config_t * dev_cfg) {
    if (bus_cfg == NULL || dev_cfg == NULL) return ESP_ERR_INVALID_ARG;

    ESP_ERROR_CHECK(i2c_new_master_bus(bus_cfg, &bus_handle));

    esp_err_t err = i2c_master_bus_add_device(bus_handle, dev_cfg, &lcd_dev);
    if (err != ESP_OK) return err;

    if (i2c_mutex == NULL) {
        i2c_mutex = xSemaphoreCreateMutex();
    }

    // Standard Hitatchi HD44780 Init Sequence
    vTaskDelay(pdMS_TO_TICKS(100));
    lcd_write_nibble(CMD_INIT_8BIT, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_write_nibble(CMD_INIT_8BIT, 0);
    esp_rom_delay_us(150);
    lcd_write_nibble(CMD_INIT_8BIT, 0);
    esp_rom_delay_us(150);
    lcd_write_nibble(CMD_INIT_4BIT, 0);
    esp_rom_delay_us(150);

    lcd_send_byte(CMD_FUNCTION_SET_4BIT, 0);
    lcd_send_byte(CMD_DISPLAY_ON, 0);
    lcd_send_byte(CMD_ENTRY, 0);
    lcd_clear();

    ESP_LOGI(TAG, "LCD initialized");
    return ESP_OK;
}

esp_err_t lcd_send_row(uint8_t row, const char *str) {
    uint8_t buf[68];
    int idx = 0;
    uint8_t addr = (row == 0) ? (CMD_SET_DDRAM_ADDR | 0x00) : (CMD_SET_DDRAM_ADDR | 0x40);

    pack_byte_to_buffer(buf, &idx, addr, 0);

    for (int i = 0; i < 16; i++) {
        uint8_t c = (i < strlen(str)) ? (uint8_t)str[i] : (uint8_t)' ';
        pack_byte_to_buffer(buf, &idx, c, LCD_RS);
    }

    return lcd_i2c_tx(buf, 68);
}

