void TimerInit(){
  Serial.println("TimerInit");
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
  DoorCheckTimer.deleteTimer(DoorCheckTimerId); 
  TaggerEnterTimerId = TaggerEnterTimer.setInterval(TaggerEnterTime, TaggerEnter);
  TaggerEnterTimer.deleteTimer(TaggerEnterTimerId); 
  SurvivorEnterTimerId = SurvivorEnterTimer.setInterval(SurvivorEnterTime, SurvivorEnter);
  SurvivorEnterTimer.deleteTimer(SurvivorEnterTimerId); 
}

void DoorCheck(){
  if(DoorCheckCNT >= 40)    DoorCheckTimer.deleteTimer(DoorCheckTimerId);
  Serial.print("[" + (String)(DoorCheckCNT) + "] ");
  for(int i=0; i<2; i++){
    if(digitalRead(EmSensor_PIN[i]) == LOW)  EmSensor_STATE[i] = 1;
    else                                     EmSensor_STATE[i] = 0;
  }

  if(DoorCheckCNT < DoorCheckLimit){
    if(DoorCheckCNT%2 == 0){
      long untilDoorCheck = (DoorCheckLimit-DoorCheckCNT+1)*0.5;
      Serial.println("DoorCheck in " + (String)(untilDoorCheck) + "sec");
    }
  }
  else{
    if(DoorEm_REQ != EmSensor_STATE){
      if(DoorEm_REQ[0] != EmSensor_STATE[0]){
        Serial.print("Tagger Door Error :: ");
        if(DoorEm_REQ[0] == 1){
          Serial.println("Not Open");
        }
        else{
          Serial.println("Not Closed");
        }
      }
      if(DoorEm_REQ[1] != EmSensor_STATE[1]){
        Serial.print("Survivor Door Error :: ");
        if(DoorEm_REQ[1] == 1){
          Serial.println("Not Open");
        }
        else{
          Serial.println("Not Closed");
        }
      }
    }
    else{
      DoorOpenRequest(0,0);
      GuideLightShow(0,0,0,0);
      DoorCheckCNT = 0;
      DoorCheckTimer.deleteTimer(DoorCheckTimerId); 
    }
  }
  DoorCheckCNT++;
}

void TaggerEnter(){
  long sec_remain = (TaggerEnterLimit-TaggerEnterCNT)* 0.5;
  Serial.println("Tagger Door Open for :: " + (String)(sec_remain) + " secondes");
  if(TaggerEnterCNT > TaggerEnterLimit){
    LastDataApply();
    tEnter_ptr = TaggerEnterCheck;
    TaggerEnterCNT = 0;
    TaggerEnterTimer.deleteTimer(TaggerEnterTimerId);
  }
  TaggerEnterCNT++;
}

void SurvivorEnter(){
  long sec_remain = (SurvivorEnterLimit-SurvivorEnterCNT)* 0.5;
  Serial.println("Survivor Door Open for :: " + (String)(sec_remain) + " secondes");
  if(SurvivorEnterCNT > SurvivorEnterLimit){
    LastDataApply();
    sEnter_ptr = SurvivorEnterCheck;
    SurvivorEnterCNT = 0;
    SurvivorEnterTimer.deleteTimer(SurvivorEnterTimerId);
  }
  SurvivorEnterCNT++;
}