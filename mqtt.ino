void mqttSubscripciones(){
  ki6o4.print("Configurando suscripciones\n");
  client.subscribe(MQTT_COMMAND_TOPIC);
  // client.subscribe("test/cmd");
}

void mqttPublish(){
  // int st = !digitalRead(LED_BUILTIN);
  // sprintf(str, "%d", state);
  // Serial.println(state);
  mqttResponse["manual"] = manualContact;
  mqttResponse["running"] = ki6o4.read(OUT1);
  mqttResponse["low_level"] = ki6o4.read(OUT2);
  serializeJson(mqttResponse,str,200);

  client.publish(MQTT_TOPIC_STATE,str);
  client.publish(MQTT_TOPIC_AVAILABILITY,"online");
}

//////////////////////////////////
//////////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) {
  //topic contiene el topic recibido
  //debe distinguirse si se ha subscrito a mas de un  topic
  //payload contiene los datos enviados
  //length es el tamano en bytes de los datos
  if(String(topic) == String(MQTT_COMMAND_TOPIC)){
    if(payload[0] == '0'){
      machine.changeState("stopped");
      ki6o4.print("State changed: Stopped\n");
    }
    else if(payload[0] == '1'){
      machine.changeState("running");
      ki6o4.print("State changed: Running\n");
    }
  }
}
