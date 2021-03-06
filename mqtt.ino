void MQTT_setup(){
  Serial.println(F("[MQTT] MQTT setup"));
  
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCallback(MQTT_message_callback);
}

void MQTT_connection_manager(){

  static int MQTT_connected = -1; // 1: connected, 0: disconnected, -1: unknown
  static long last_MQTT_connection_attempt;
  
  if(!MQTT_client.connected()) {
    if(MQTT_connected != 0){
      // MQTT connection status changed to "disconnected"
      MQTT_connected = 0;

      Serial.println(F("[MQTT] Disconnected"));
    }

    if(millis() - last_MQTT_connection_attempt > 1000){
      last_MQTT_connection_attempt = millis();

      // Prepare a last will
      StaticJsonDocument<200> outbound_JSON_message;
      outbound_JSON_message["state"] = "disconnected";
      char last_will[100];
      serializeJson(outbound_JSON_message, last_will, sizeof(last_will));
  
      MQTT_client.connect(
        HOSTNAME,
        MQTT_USERNAME,
        MQTT_PASSWORD,
        MQTT_STATUS_TOPIC,
        MQTT_QOS,
        MQTT_RETAIN,
        last_will
      );
    }
        
  }
  else {
    if(MQTT_connected != 1){
      // MQTT connection status changed to "connected"
      MQTT_connected = 1;

      Serial.println(F("[MQTT] Connected"));
      
      Serial.println(F("[MQTT] Subscribing to topics"));
      MQTT_client.subscribe(MQTT_COMMAND_TOPIC);
      
      MQTT_publish_state();
    }
  }
}

void MQTT_publish_state(){
  // Send the state through MQTT
  Serial.println(F("[MQTT] publish of state"));
  
  // Create a JSON document object
  StaticJsonDocument<200> outbound_JSON_message;

  // Assign the relay state to the state key of the JSON payload
  outbound_JSON_message["state"] = relay_state;

  // Prepare a buffer to send the JSON object
  char JSONmessageBuffer[100];
  serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
  //Send the payload
  MQTT_client.publish(MQTT_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
}



void MQTT_message_callback(char* topic, byte* payload, unsigned int length) {

  // Debugging
  Serial.print(F("[MQTTT] message received on "));
  Serial.print(topic);
  Serial.print(F(", payload: "));
  for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
  Serial.println();

  

  // Parsing payload as JSON
  StaticJsonDocument<200> inbound_JSON_message;
  deserializeJson(inbound_JSON_message, payload);

  if(!inbound_JSON_message.containsKey("state")) {
    Serial.println("[MQTT] payload does not contain state key");
    return;
  }


  // Extracting the command state from payload
  // Normally a const char* but copied using strdup so as to turn it into a char* for manipulation with strlwr
  char* command_state = strdup(inbound_JSON_message["state"]);

  // Reacting accordingly
  if(strcmp(strlwr(command_state), "on") == 0) turn_on();
  else if(strcmp(strlwr(command_state), "off") == 0) turn_off();

  // Free memory
  free(command_state);
}
