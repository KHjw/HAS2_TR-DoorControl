void DoorSystemInit(){
  Serial.println("DoorSystemInit");
  for(int i=0; i<2; i++)
    pinMode(DoorEm_PIN[i] ,OUTPUT);
  for(int i=0; i<4; i++)
    pinMode(GuideLight_PIN[i] ,OUTPUT);
  for(int i=0; i<2; i++)
    pinMode(EmSensor_PIN[i] ,INPUT);
  pinMode(tDOOR_EMERGENCY ,INPUT);
  pinMode(sDOOR_EMERGENCY ,INPUT);
}

//****************************************Em SETUP**************************************************
void DoorOpen(int a, int b){
  DoorEm_REQ[0] = a;
  DoorEm_REQ[1] = b;
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1)  digitalWrite(DoorEm_PIN[i], LOW);
    else                    digitalWrite(DoorEm_PIN[i], HIGH);
  }
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}

//****************************************GuideLight SETUP**************************************************
void GuideLightShow(){
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1)  digitalWrite(GuideLight_PIN[i], HIGH);
    else                          digitalWrite(GuideLight_PIN[i], LOW);
  }
}

void GuideLightShow(int a, int b, int c, int d){
  GuideLight_STATE[0] = a;
  GuideLight_STATE[1] = b;
  GuideLight_STATE[2] = c;
  GuideLight_STATE[3] = d;
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1)  digitalWrite(GuideLight_PIN[i], HIGH);
    else                          digitalWrite(GuideLight_PIN[i], LOW);
  }
}

//****************************************EmergencyCheck SETUP**************************************************
void EmergencyCheck(){
  int EmergencyCNT = 0;
  if(digitalRead(tDOOR_EMERGENCY) == HIGH)  EmergencyCNT += 1;
  if(digitalRead(sDOOR_EMERGENCY) == HIGH)  EmergencyCNT += 2;

  if(EmergencyCNT != EmergencyCode){
    switch (EmergencyCNT){
    case 0:
      Serial.println("EMERGENCY OVER");
      has2_mqtt.Publish(my_topic, "EMG0");
      GuideLightShow(0,0,0,0);
      // 기존 코드 회귀
      break;
    case 1:
      Serial.println("TAGGER DOOR EMERGENCY OPEN");
      has2_mqtt.Publish(my_topic, "EMG1");
      GuideLight_STATE[0] = 1;
      GuideLight_STATE[1] = 1;
      GuideLightShow();
      break;
    case 2:
      Serial.println("SURVIVOR DOOR EMERGENCY OPEN");
      has2_mqtt.Publish(my_topic, "EMG2");
      GuideLight_STATE[2] = 1;
      GuideLight_STATE[3] = 1;
      GuideLightShow();
      break;
    case 3:
      Serial.println("ALL DOOR EMERGENCY OPEN");
      has2_mqtt.Publish(my_topic, "EMG3");
      GuideLightShow(1,1,1,1);
      break;
    default:
      break;
    }
    EmergencyCode = EmergencyCNT;
  }
}