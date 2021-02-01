
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ESP_SSID);
  WiFi.begin(ESP_SSID, ESP_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//////////////////////////////////
//////////////////////////////////
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(ESP_ID,MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      mqttSubscripciones();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(2000);
    }
  }
}

//////////////////////////////////
//////////////////////////////////
void setup_server(){
  client.setServer(ESP_MQTT_SERVER, 1883);
  client.setCallback(callback);
}
