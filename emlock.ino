void EmControl(int code, bool IsOpen){
  if(IsOpen == true)  DoorEm_REQ[code] = 1;
  else                DoorEm_REQ[code] = 0;
  DoorcheckTimerId = DoorcheckTimer.setInterval(DoorcheckTime, DoorCheck);
}

void EmApply(int a, int b){
  DoorEm_REQ[2] = {a,b};
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1)  digitalWrite(DoorEm_PIN[i], LOW);
    else                    digitalWrite(DoorEm_PIN[i], HIGH);
  }
  DoorcheckTimerId = DoorcheckTimer.setInterval(DoorcheckTime, DoorCheck);
}

void GuideLightControl(int code, bool IsOn){
  if(IsOn == true)  GuideLight_STATE[code] = 1;
  else              GuideLight_STATE[code] = 0;
}

void GuideLightShow(int a, int b, int c, int d){
  GuideLight_STATE[4] = {a,b,c,d};
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1)  digitalWrite(GuideLight_PIN[i], HIGH);
    else                          digitalWrite(GuideLight_PIN[i], LOW);
  }
}

void AllDoorClosed(){

}

void AllDoorOpen(){
  
}

void EmergencyOpen(){

}