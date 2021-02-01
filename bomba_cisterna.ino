/*
Copyright 2021 Erick Garcia

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// #define _KI6O4_ARDUINO_NANO  //only IN2-5 OUT1-3
#define _KI6O4_ESP8266  //only IN2-5 OUT1-3
#include <KI6O4.h>
#include <KontrolMin.h>
#include <StateMachine.h>
#include <TimeEvent.h>

#ifdef _KI6O4_ESP8266
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"
#include "config.h"

#define MQTT_TOPIC_STATE "homeassistant/exterior/bomba_agua/state"
#define MQTT_TOPIC_AVAILABILITY "homeassistant/exterior/bomba_agua/availability"
#define MQTT_COMMAND_TOPIC "homeassistant/exterior/bomba_agua/command"

WiFiClient espClient;
PubSubClient client(espClient);
char str[200];

TimeEvent mqttResponseTimer = TimeEvent(MQTT_RESPONSE_TIME);

const size_t capacity = JSON_OBJECT_SIZE(10) + 100;
StaticJsonDocument<capacity> mqttResponse;
#endif

#ifdef _KI6O4_ARDUINO_NANO
TimeEvent statusLedTimer = TimeEvent(1000);
#endif

StateMachine machine = StateMachine();
KontrolMin kontrol = KontrolMin();
TimeEvent debugTimer = TimeEvent(1000);

bool manualContact = false;
bool lowContact = false;
bool startContact = false;
bool stopContact = false;

bool debugActive = false;

void setup(){
  ki6o4.begin(); //iniciando modulo KI6O4
  Serial.begin(115200); //iniciando comunicacion serial
  delay(500);
  ki6o4.print("###INIT###\n");
  delay(500);
  machine.changeState("stopped");
  ki6o4.print("State changed: Stopped\n");

  debugTimer.repeat();
  debugTimer.stop();

  #ifdef _KI6O4_ARDUINO_NANO
  statusLedTimer.repeat();
  statusLedTimer.start();
  #endif

  #ifdef _KI6O4_ESP8266
  setup_wifi();
  setup_server();

  mqttResponseTimer.repeat();
  mqttResponseTimer.start();
  #endif
}


/////////////////////////////////////////////////////////
//LOOP
void loop(){

  manualContact = ki6o4.read(IN2);
  startContact = ki6o4.read(IN3);
  stopContact = ki6o4.read(IN4);
  lowContact = ki6o4.read(IN5);

  //Kontrol
  if(Serial.available()){
    kontrol.update(Serial.read());
  }

  kontrol.addListener("start",startCallback);
  kontrol.addListener("stop",stopCallback);
  kontrol.addListener("debug",debugCallback);
  kontrol.addListener("ok",okCallback);
  kontrol.addListener("help",helpCallback);

  //StateMachine
  machine.stateCase("running",runningState);
  machine.stateCase("stopped",stoppedState);

  if(debugTimer.run()){
    ki6o4.print("###DEBUG###\nStartContact: ");
    ki6o4.print(startContact);
    ki6o4.print("\nStopContact: ");
    ki6o4.print(stopContact);
    ki6o4.print("\nLowContact: ");
    ki6o4.print(lowContact);
    ki6o4.print("\nManualContact: ");
    ki6o4.print(manualContact);
    ki6o4.print("\n");
  }

  #ifdef _KI6O4_ESP8266

  //////////////////////////RECONECT
  if (!client.connected()) {
    reconnect();
  }
  //////////////////////////
  client.loop();

  if(mqttResponseTimer.run()){
    mqttPublish();
  }

  #endif

  #ifdef _KI6O4_ARDUINO_NANO
  if(statusLedTimer.run()){
    digitalWrite(STATUS,!digitalRead(STATUS));
  }
  #endif
}
