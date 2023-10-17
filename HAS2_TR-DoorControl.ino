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

void setup(void){
  Serial.begin(115200);
  Serial.println("=============================ESP SETUP=============================");
  has2_mqtt.Setup("train_room", "Code3824@", callback);         // ALL, myMAC 구독
  has2_mqtt.Situation("start", "my");
  TimerInit();
  DoorSystemInit();
  // TestInit();
  // CheckInit();
  GuideLightShow(1,1,1,1);
  DoorOpenRequest(1,1);
  Serial.println("===================================================================");
}

void loop(void){
  // SerialControl();
  // EnterCheckLoop();
  // EmergencyCheckLoop();
  has2_mqtt.ReadSubscirbe();
  // DoorCheckTimer.run();
  // TaggerEnterTimer.run();
  // SurvivorEnterTimer.run();
}