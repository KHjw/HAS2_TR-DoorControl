#include <Arduino.h>
#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
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

void setup(void) {
  Serial.begin(115200);
  Serial.println("=============================ESP SETUP=============================");
  has2_mqtt.Setup("KT_GiGA_6C64","ed46zx1198", callback);         // ALL, myMAC 구독
  // has2_mqtt.AddSubscirbe("ALL");
  TimerInit();
  EmInit();
  NeopixelInit();
  Serial.println("===================================================================");
}

void loop(void) {
  has2_mqtt.ReadSubscirbe();
//   game_ptr();
//   DoorcheckTimer.run();
}

#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-DoorControl\\emlock.ino"
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
#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-DoorControl\\mqtt.ino"
void callback(char* topic, byte* payload, unsigned int length) 
{
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

  if(input_data == "OTA")               has2_mqtt.FirmwareUpdate("emlock");
  else if(input_data == "Setting")      game_ptr = Game_Setting;
  else if(input_data == "Ready")        game_ptr = Game_Ready;
  else if(input_data == "Activate")     game_ptr = Device_Activate;
  else if(input_data == "Open")         game_ptr = Device_Open;
  else if(input_data == "Lock")         game_ptr = Device_Lock;
}

void Device_tHOME{
  GuideLight_STATE[tPUBLIC] = 1;
  
}
#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-DoorControl\\timer.ino"
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
