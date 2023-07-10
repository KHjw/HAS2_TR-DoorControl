# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
 /*

 * @file HAS2_TR-DoorControl.ino

 * @author 홍진우 KH.jinu (kevinlike@naver.com)

 * @brief

 * @version 1.0

 * @date 2023-07-07

 *

 * @copyright Copyright (c) 2023

 */
# 11 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
# 12 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino" 2

void setup(void) {
  Serial.begin(115200);
  Serial.println("=============================ESP SETUP=============================");
  has2_mqtt.Setup("KT_GiGA_6C64","ed46zx1198", callback); // ALL, myMAC 구독
  TimerInit();
  DoorSystemInit();
  Serial.println("===================================================================");
}

void loop(void) {
  EmergencyCheck();
  has2_mqtt.ReadSubscirbe();
  // game_ptr();
  DoorCheckTimer.run();
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void DoorSystemInit(){
  Serial.println("DoorSystemInit");
  for(int i=0; i<2; i++)
    pinMode(DoorEm_PIN[i] ,0x03);
  for(int i=0; i<4; i++)
    pinMode(GuideLight_PIN[i] ,0x03);
  for(int i=0; i<2; i++)
    pinMode(EmSensor_PIN[i] ,0x01);
  pinMode(5 ,0x01);
  pinMode(2 ,0x01);
}

//****************************************Em SETUP**************************************************
void DoorOpen(int a, int b){
  DoorEm_REQ[0] = a;
  DoorEm_REQ[1] = b;
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1) digitalWrite(DoorEm_PIN[i], 0x0);
    else digitalWrite(DoorEm_PIN[i], 0x1);
  }
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}

//****************************************GuideLight SETUP**************************************************
void GuideLightShow(){
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1) digitalWrite(GuideLight_PIN[i], 0x1);
    else digitalWrite(GuideLight_PIN[i], 0x0);
  }
}

void GuideLightShow(int a, int b, int c, int d){
  GuideLight_STATE[0] = a;
  GuideLight_STATE[1] = b;
  GuideLight_STATE[2] = c;
  GuideLight_STATE[3] = d;
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1) digitalWrite(GuideLight_PIN[i], 0x1);
    else digitalWrite(GuideLight_PIN[i], 0x0);
  }
}

//****************************************EmergencyCheck SETUP**************************************************
void EmergencyCheck(){
  int EmergencyCNT = 0;
  if(digitalRead(5) == 0x1) EmergencyCNT += 1;
  if(digitalRead(2) == 0x1) EmergencyCNT += 2;

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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length){
  static bool start = false;
  if(!start){
    start = true;
    has2_mqtt.Publish(my_topic, "start");
    return ;
  }
  String input_data = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  for (int i = 0; i < length; i++) {
    input_data += (char)payload[i];
  }
  Serial.println(input_data);

  GuideLightShow(0,0,0,0);
  if(input_data == "OTA") has2_mqtt.FirmwareUpdate("emlock");
  else if(input_data == "Setting") DoorOpen(1,1);
  else if(input_data == "Ready") DoorOpen(1,1);
  else if(input_data == "Activate") DoorOpen(1,1);
  else if(input_data == "TO_tHOME") MoveTo(tHOME);
  else if(input_data == "TO_tPUBLIC") MoveTo(tPUBLIC);
  else if(input_data == "TO_sHOME") MoveTo(sHOME);
  else if(input_data == "TO_tPUBLIC") MoveTo(sPUBLIC);
  else if(input_data == "Open") DoorOpen(1,1);
  else if(input_data == "Lock") DoorOpen(0,0);
}

//****************************************Control SETUP**************************************************
void MoveTo(int where){
  switch (where){
  case tHOME:
    DoorOpen(1,0);
    GuideLightShow(0,1,0,0);
    break;
  case tPUBLIC:
    DoorOpen(1,0);
    GuideLightShow(1,0,0,0);
    break;
  case sHOME:
    DoorOpen(0,1);
    GuideLightShow(0,0,0,1);
    break;
  case sPUBLIC:
    DoorOpen(0,1);
    GuideLightShow(0,0,1,0);
    break;
  default:
    DoorOpen(0,0);
    GuideLightShow(0,0,0,0);
    break;
  }
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\timer.ino"
void TimerInit(){
  Serial.println("TimerInit");
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
  DoorCheckTimer.deleteTimer(DoorCheckTimerId);
}

void DoorCheck(){
  for(int i=0; i<2; i++){
  if(digitalRead(EmSensor_PIN[i]) == 0x1) DoorEm_STATE[i] = 1;
  else DoorEm_STATE[i] = 0;
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
