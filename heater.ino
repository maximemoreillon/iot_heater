/*
 * Wifi Heater
 * 
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */

// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include <OneWire.h> // For the temperature sensor
//#include <DallasTemperature.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> // Another DNS server to help find the device on a network
#include "uptime_formatter.h"

#include "credentials.h";
#include "iot_config_home.h"; // MQTT broker URL + connection config


// Device info
#define HOSTNAME "heater"
#define FIRMWARE_VERSION "0.2.3"

// MQTT settings
#define MQTT_COMMAND_TOPIC "heater/bedroom/command"
#define MQTT_STATUS_TOPIC "heater/bedroom/status"
#define MQTT_TEMPERATURE_STATUS_TOPIC "temperature/heater/status"

#define MQTT_LAST_WILL "OFF"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// Pin mapping
#define RELAY_PIN D2
#define LED_PIN D8
#define BUTTON_PIN D1
#define DALLAS_PIN D6

// Timing
#define TEMPERATURE_PUBLISH_PERIOD 60000


// Temperature sensor
ESP8266WebServer web_server(80);
WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);
//OneWire oneWire(DALLAS_PIN);
//DallasTemperature sensors(&oneWire);

char* relay_state = "OFF";


void setup()
{
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish

  io_setup();
  wifi_setup();
  MQTT_setup();
  OTA_setup();
  MDNS.begin(HOSTNAME);
  //sensors.begin();
  web_server_setup();

}


void loop() {
  ArduinoOTA.handle();
  wifi_connection_manager();
  MQTT_connection_manager();
  web_server.handleClient();
  MQTT_client.loop();
  MDNS.update();
  read_button();
}
