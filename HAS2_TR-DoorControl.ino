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