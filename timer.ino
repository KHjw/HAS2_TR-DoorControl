void TimerInit(){
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
  DoorCheckTimer.deleteTimer(DoorCheckTimerId); 
  EmergencyTimerId = EmergencyTimer.setInterval(EmergencyTime, EmergencyCheck);
  EmergencyTimer.deleteTimer(EmergencyTimerId);
}

void DoorCheck(){
  String DoorReqSTR;
  String DoorStateSTR;

  for(int i=0; i<2; i++){
    DoorReqSTR += DoorEm_REQ[i];
    DoorStateSTR += DoorEm_STATE[i];
  }

  if(DoorReqSTR != DoorStateSTR){
    Serial.print("Door State Error :: ");
    if(IsRequestOpen == true){
      Serial.println("Not Open");
    }
    else
      Serial.println("Not Closed");
  }
  else{
    DoorCheckCNT = 0;
    DoorcheckTimer.deleteTimer(DoorcheckTimerId); 
  }
  DoorCheckCNT++;
}

void EmergencyCheck(){
  if((digitalRead(tDOOR_EMERGENCY) == HIGH) && (digitalRead(sDOOR_EMERGENCY) == HIGH)){
    Serial.println("ALL DOOR EMERGENCY OPEN");
    GuideLightShow(1,1,1,1);
  }
  else{
    if(digitalRead(tDOOR_EMERGENCY) == HIGH){
      Serial.println("TAGGER DOOR EMERGENCY OPEN");
      GuideLightShow(1,1,0,0);
    }
    if(digitalRead(sDOOR_EMERGENCY) == HIGH){
      Serial.println("SURVIVOR DOOR EMERGENCY OPEN");
      GuideLightShow(0,0,1,1);
    }
  }
}