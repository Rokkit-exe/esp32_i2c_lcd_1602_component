idf_component_register(
  SRCS "i2c_lcd.c"
  INCLUDE_DIRS "include"
  REQUIRES freertos driver esp_rom log
)
