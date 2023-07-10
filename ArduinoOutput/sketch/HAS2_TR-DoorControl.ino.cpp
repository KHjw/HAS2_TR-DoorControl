#include <Arduino.h>
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
 /*
 * @file HAS2_TR-DoorControl.ino
 * @author 홍진우 KH.jinu (kevinlike@naver.com)
 * @brief
 * @version 1.0
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 */

#include "DoorControl.h"

#line 13 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
void setup(void);
#line 22 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
void loop(void);
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void DoorSystemInit();
#line 14 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void DoorOpen(int a, int b);
#line 25 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void GuideLightShow();
#line 32 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void GuideLightShow(int a, int b, int c, int d);
#line 44 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void EmergencyCheck();
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length);
#line 31 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\mqtt.ino"
void MoveTo(int where);
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\timer.ino"
void TimerInit();
#line 7 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\timer.ino"
void DoorCheck();
#line 13 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
void setup(void) {
  Serial.begin(115200);
  Serial.println("=============================ESP SETUP=============================");
  has2_mqtt.Setup("KT_GiGA_6C64","ed46zx1198", callback);         // ALL, myMAC 구독
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
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
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
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\mqtt.ino"
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
  if(input_data == "OTA")               has2_mqtt.FirmwareUpdate("emlock");
  else if(input_data == "Setting")      DoorOpen(1,1);
  else if(input_data == "Ready")        DoorOpen(1,1);
  else if(input_data == "Activate")     DoorOpen(1,1);
  else if(input_data == "TO_tHOME")     MoveTo(tHOME);
  else if(input_data == "TO_tPUBLIC")   MoveTo(tPUBLIC);
  else if(input_data == "TO_sHOME")     MoveTo(sHOME);
  else if(input_data == "TO_tPUBLIC")   MoveTo(sPUBLIC);
  else if(input_data == "Open")         DoorOpen(1,1);
  else if(input_data == "Lock")         DoorOpen(0,0);
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
#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\timer.ino"
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
