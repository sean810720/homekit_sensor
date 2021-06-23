/*
   my_accessory.c
   Define the accessory in C language using the Macro in characteristics.h
*/

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click the "Identify Accessory" button in Home APP.
void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Temperature Sensor");
homekit_characteristic_t cha_current_temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);
homekit_characteristic_t cha_humidity = HOMEKIT_CHARACTERISTIC_(CURRENT_RELATIVE_HUMIDITY, 0);
homekit_characteristic_t air_quality = HOMEKIT_CHARACTERISTIC_(AIR_QUALITY, 0);
homekit_characteristic_t pm25_density = HOMEKIT_CHARACTERISTIC_(PM25_DENSITY, 0);

homekit_accessory_t *accessories[] = {

  // 裝置橋接器
  HOMEKIT_ACCESSORY(.id = 1, .category = homekit_accessory_category_bridge, .services = (homekit_service_t*[]) {
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Home Sensors"),
      HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Venusean HomeKit"),
      HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "00000001"),
      HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
      HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
      HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
      NULL
    }),
    NULL
  }),

  // 裝置1: 溫度計
  HOMEKIT_ACCESSORY(.id = 2, .category = homekit_accessory_category_sensor, .services = (homekit_service_t*[]) {
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Temperature Sensor"),
                             HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
                             NULL
    }),
    HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Temperature"),
                             &cha_current_temperature,
                             NULL
    }),
    NULL
  }),

  // 裝置2: 溼度計
  HOMEKIT_ACCESSORY(.id = 3, .category = homekit_accessory_category_sensor, .services = (homekit_service_t*[]) {
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
                             HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
                             NULL
    }),
    HOMEKIT_SERVICE(HUMIDITY_SENSOR, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Humidity"),
                             &cha_humidity,
                             NULL
    }),
    NULL
  }),

  // 裝置3: 空氣品質檢測器
  HOMEKIT_ACCESSORY(.id = 4, .category = homekit_accessory_category_sensor, .services = (homekit_service_t*[]) {
    HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Air Quality Sensor"),
                             HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
                             NULL
    }),
    HOMEKIT_SERVICE(AIR_QUALITY_SENSOR, .primary = true, .characteristics = (homekit_characteristic_t*[]) {
      HOMEKIT_CHARACTERISTIC(NAME, "Air Quality"),
                             &air_quality,
                             &pm25_density,
                             NULL
    }),
    NULL
  }),

  NULL
};

// 密碼設定
homekit_server_config_t config = {
  .accessories = accessories,
  .password = "111-11-111"
};
