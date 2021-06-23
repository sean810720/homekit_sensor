#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

//include the Arduino library for your real sensor here, e.g. <DHT.h>

// DHT11
#include <SimpleDHT.h>
#define DHT11_PIN 14
SimpleDHT11 dht11;

// Air Sensor
#include <MQ135.h>
#define AIR_SENSOR_PIN A0
MQ135 airSensor = MQ135(AIR_SENSOR_PIN);

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  // homekit_storage_reset();
  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
}


//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_humidity;
extern "C" homekit_characteristic_t air_quality;
extern "C" homekit_characteristic_t pm25_density;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();

  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }

  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}

void my_homekit_report() {

  // Get DHT11 data
  byte temperature = 0;
  byte humidity    = 0;
  uint8_t err      = SimpleDHTErrSuccess;

  while ((err = dht11.read(DHT11_PIN, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(1000);
    return;
  }

  // Get Air Sensor data
  // uint8_t air = airSensor.getCorrectedPPM((float)temperature, (float)humidity);
  // uint8_t air = analogRead(AIR_SENSOR_PIN);
  uint8_t air = airSensor.getPPM();

  // Read your real sensor here.

  // 溫度
  float temperature_value = temperature;
  cha_current_temperature.value.float_value = temperature_value;
  homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);
  LOG_D("Current temperature: %.1f", temperature_value);

  // 濕度
  float humidity_value = humidity;
  cha_humidity.value.float_value = humidity_value;
  homekit_characteristic_notify(&cha_humidity, cha_humidity.value);
  LOG_D("Current Humidity: %.1f", humidity_value);

  // 空氣品質
  uint8_t air_quality_value = 0; // 未知

  // 根據 PM2.5 數值判斷
  if (air <= 11.0) {
    air_quality_value = 1; // 極佳

  } else if (air > 11.0 && air <= 23.0) {
    air_quality_value = 2; // 良好

  } else if (air > 23.0 && air <= 35.0) {
    air_quality_value = 3; // 普通

  } else if (air > 35.0 && air <= 41.0) {
    air_quality_value = 4; // 不佳
    
  } else if (air > 41.0) {
    air_quality_value = 5; // 極差
  }

  air_quality.value.uint8_value = air_quality_value;
  homekit_characteristic_notify(&air_quality, air_quality.value);
  LOG_D("Current Air Quality: %i", air_quality_value);

  // PM2.5
  float pm25_density_value = air;
  pm25_density.value.float_value = pm25_density_value;
  homekit_characteristic_notify(&pm25_density, pm25_density.value);
  LOG_D("Current Air PM2.5: %.1f", pm25_density_value);
}
