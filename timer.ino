void TimerInit(){
  Serial.println("TimerInit");
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
  DoorCheckTimer.deleteTimer(DoorCheckTimerId); 
}

void DoorCheck(){
  for(int i=0; i<2; i++){
  if(digitalRead(EmSensor_PIN[i]) == HIGH)  DoorEm_STATE[i] = 1;
  else                                      DoorEm_STATE[i] = 0;
  }

  if(DoorEm_REQ != DoorEm_STATE){
    if(DoorEm_REQ[0] != DoorEm_STATE[0]){
      Serial.print("Tagger Door Error :: ");
      if(DoorEm_REQ[0] == 1){
        Serial.println("Not Open");
      }
      else{
        Serial.println("Not Closed");
        if(DoorCheckCNT > DoorCheckLimit)    
          has2_mqtt.Publish(my_topic, "ERROR");
      }
    }
    if(DoorEm_REQ[1] != DoorEm_STATE[1]){
      Serial.print("Survivor Door Error :: ");
      if(DoorEm_REQ[1] == 1){
        Serial.println("Not Open");
      }
      else{
        Serial.println("Not Closed");
        if(DoorCheckCNT > DoorCheckLimit)    
          has2_mqtt.Publish(my_topic, "ERROR");
      }
    }
  }
  else{
    DoorOpen(0,0);
    GuideLightShow(0,0,0,0);
    DoorCheckCNT = 0;
    DoorCheckTimer.deleteTimer(DoorCheckTimerId); 
  }
  DoorCheckCNT++;
}