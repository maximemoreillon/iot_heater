void io_setup(){
  
  Serial.println("[IO] IO initialization");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Initial state
  if(strcmp(relay_state,"ON") == 0) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }
}

void turn_on(){
  Serial.println("[IO] Turning relay ON");
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  relay_state = "ON";
  MQTT_publish_state();
}

void turn_off(){
  Serial.println("[IO] Turning relay OFF");
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  relay_state = "OFF";
  MQTT_publish_state();
}

void toggle_state(){
  Serial.println(F("[IO] Toggling state"));
  if(strcmp(relay_state,"OFF") == 0) turn_on();
  else if(strcmp(relay_state,"ON") == 0) turn_off();
}

void read_button() {
  const long button_debounce_delay = 50;
  static int last_button_reading;
  static long last_button_reading_change_time;
  static int button_state = LOW; // Output
  
  int button_reading = digitalRead(BUTTON_PIN);

  // Check if reading changed
  if(button_reading != last_button_reading) {
    // Acknowledge button reading change
    last_button_reading = button_reading;
    
    // Record time of last button reading change
    last_button_reading_change_time = millis();
  }

  // Only acknowledge the button reading if state did not change for long enough
  if(millis()-last_button_reading_change_time > button_debounce_delay) {
    
    if(button_reading != button_state) {
      // Save reading as button state if state and reading don't match (prevents saving all the time)
      button_state = button_reading;


      // IF the button has changed to being pressed for long enough
      if(button_state == LOW) {
        Serial.println("[IO] Button pressed");
        toggle_state();
      }
    }
  }
}

void read_temperature() {
  static long lastPublish;

  // Periodic publish of temperature
  if (millis() - lastPublish > TEMPERATURE_PUBLISH_PERIOD){
    lastPublish = millis();
    //sensors.requestTemperatures(); // Send the command to get temperatures
    //float temperature = sensors.getTempCByIndex(0);
    //MQTT_client.publish(MQTT_TEMPERATURE_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, String(temperature).c_str());
  }
}
