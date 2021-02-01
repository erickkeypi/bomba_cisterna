void stoppedState(){

  ki6o4.write(OUT2,lowContact); //salida de activacion del led "low"
  ki6o4.write(OUT1,0); //salida de activacion de la bomba

  if(!manualContact && !lowContact && startContact){
    machine.changeState("running");
    ki6o4.print("State changed: Running\n");
    mqttPublish();
  }
}

void runningState(){

  ki6o4.write(OUT2,lowContact); //salida de activacion del led "low"
  ki6o4.write(OUT1,1); //salida de activacion de la bomba

  if(manualContact || lowContact || stopContact){
    machine.changeState("stopped");
    ki6o4.print("State changed: Stopped\n");
    mqttPublish();
  }
}
