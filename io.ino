void io_setup(){
  
  Serial.println("[IO] IO initialization");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Initially OFF
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
}

void turn_on(){
  Serial.println("[IO] Turning relay ON");
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  iot_kernel.device_state = "on";
  iot_kernel.mqtt_publish_state();
}

void turn_off(){
  Serial.println("[IO] Turning relay OFF");
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  iot_kernel.device_state = "off";
  iot_kernel.mqtt_publish_state();
}

void toggle(){
  Serial.println(F("[IO] Toggling state"));
  if(iot_kernel.device_state == "off"){
    turn_on();
  }
  else if(iot_kernel.device_state == "on"){
    turn_off();
  }
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
        toggle();
      }
    }
  }
}
