/*
 * Wifi Heater
 * 
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */

// Libraries
#include "iotKernel.h"
//#include <OneWire.h> // For the temperature sensor
//#include <DallasTemperature.h>




// Pin mapping
#define RELAY_PIN D2
#define LED_PIN D8
#define BUTTON_PIN D1
#define DALLAS_PIN D6

// MQTT
#define MQTT_COMMAND_TOPIC "heater/living/command"
#define MQTT_STATUS_TOPIC "heater/living/status"

IotKernel iot_kernel("heater","0.0.6"); 


void setup() {
  io_setup();
  iot_kernel.init();
  mqtt_config();
}



void loop() {
  iot_kernel.loop();
  read_button();
}
