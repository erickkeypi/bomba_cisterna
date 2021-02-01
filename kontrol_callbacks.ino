void startCallback(){
  machine.changeState("running");
  ki6o4.print("State changed: Running\n");
}

void stopCallback(){
  machine.changeState("stopped");
  ki6o4.print("State changed: Stopped\n");
}

void debugCallback(){
  debugActive = !debugActive;
  if(debugActive){
    debugTimer.start();
  }
  else{
    debugTimer.stop();
  }
}

void okCallback(){
  ki6o4.print("OK\n");
}

void helpCallback(){
  ki6o4.print("###HELP###\n");
  ki6o4.print("\t{start} -> Enciende la bomba (solo si es permitido)\n");
  ki6o4.print("\t{stop} -> Apaga la bomba\n");
  ki6o4.print("\t{debug} -> Muestra el estado de los contactos cada segundo\n");
}
