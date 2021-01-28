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

#define _KI6O4_ESP8266  //only IN2-5 OUT1-3
#include <KI6O4.h>
bool run = false;

void setup(){
  ki6o4.begin();
  Serial.begin(115200);
}

void loop(){

  bool manual = ki6o4.read(IN2);
  bool start = ki6o4.read(IN3);
  bool stop = ki6o4.read(IN4);
  bool low = ki6o4.read(IN5);

  if(!manual && !low){
    if(start){
      run = true;
    }
    if(stop){
      run =false;
    }
  } else {
    run = false;
  }

  ki6o4.write(OUT1,run);
  ki6o4.write(OUT2,low);
}
