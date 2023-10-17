void DoorSystemInit(){
  Serial.println("DoorSystemInit");
  for(int i=0; i<2; i++)
    pinMode(DoorEm_PIN[i], OUTPUT);
  for(int i=0; i<4; i++)
    pinMode(GuideLight_PIN[i], OUTPUT);
  for(int i=0; i<2; i++)
    pinMode(EmSensor_PIN[i] ,INPUT_PULLUP);
  pinMode(tESW_PIN, INPUT_PULLUP);
  pinMode(sESW_PIN, INPUT_PULLUP);
  pinMode(tDSW_PIN, INPUT_PULLUP);
  pinMode(sDSW_PIN, INPUT_PULLUP);
}

//****************************************Em SETUP**************************************************
void DoorOpenRequest(){
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1)  digitalWrite(DoorEm_PIN[i], HIGH);
    else                    digitalWrite(DoorEm_PIN[i], LOW);
  }
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}

void DoorOpenRequest(int a, int b){
  DoorEm_REQ[0] = a;
  DoorEm_REQ[1] = b;
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1)  digitalWrite(DoorEm_PIN[i], HIGH);
    else                    digitalWrite(DoorEm_PIN[i], LOW);
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
void EmergencyCheckLoop(){
  int EmergencyCNT = 0;
  if(digitalRead(tESW_PIN) == LOW)  EmergencyCNT += 1;
  if(digitalRead(sESW_PIN) == LOW)  EmergencyCNT += 2;
  Serial.println("EmergencyCNT : " + (String)(EmergencyCNT));
  if(EmergencyCNT != EmergencyCode){
    Serial.println("EmergencyCode : " + (String)(EmergencyCode));
    switch (EmergencyCNT){
    case 0:
      Serial.println("EMERGENCY OVER");
      has2_mqtt.Send("my", "EMRG", "null");
      GuideLightShow(0,0,0,0);
      // LastDataApply();
      break;
    case 1:
      // LastDataApply();
      Serial.println("TAGGER DOOR EMERGENCY OPEN");
      has2_mqtt.Send("my", "EMRG", "tagger");
      GuideLight_STATE[0] = 1;
      GuideLight_STATE[1] = 1;
      GuideLightShow();
      break;
    case 2:
      // LastDataApply();
      Serial.println("SURVIVOR DOOR EMERGENCY OPEN");
      has2_mqtt.Send("my", "EMRG", "player");
      GuideLight_STATE[2] = 1;
      GuideLight_STATE[3] = 1;
      GuideLightShow();
      break;
    case 3:
      Serial.println("ALL DOOR EMERGENCY OPEN");
      has2_mqtt.Send("my", "EMRG", "all");
      GuideLightShow(1,1,1,1);
      break;
    default:
      break;
    }
    EmergencyCode = EmergencyCNT;
    delay(50);
  }
}

//****************************************DigitalSWCheck SETUP**************************************************
void EnterCheckLoop(){
  if(EmSensor_STATE[0] == 0)
    if(digitalRead(tDSW_PIN) == LOW)  tEnter_ptr();
  if(EmSensor_STATE[1] == 0)
    if(digitalRead(sDSW_PIN) == LOW)  sEnter_ptr();
}

//Enter_ptr 함수
void TaggerEnterCheck(){
  DoorEm_REQ[0] = 1;
  DoorOpenRequest();

  tEnter_ptr = EnterCheckWait;
  Serial.println("Tagger Door Enter!!!");
  TaggerEnterCNT = 0;
  TaggerEnterTimer.deleteTimer(TaggerEnterTimerId);
  TaggerEnterTimerId = TaggerEnterTimer.setInterval(TaggerEnterTime, TaggerEnter);
}

void SurvivorEnterCheck(){
  DoorEm_REQ[1] = 1;
  DoorOpenRequest();

  sEnter_ptr = EnterCheckWait;
  Serial.println("Survivor Door Enter!!!");
  SurvivorEnterCNT = 0;
  SurvivorEnterTimer.deleteTimer(SurvivorEnterTimerId);
  SurvivorEnterTimerId = SurvivorEnterTimer.setInterval(SurvivorEnterTime, SurvivorEnter);
}

void EnterCheckWait(){}