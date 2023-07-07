#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-DoorControl\\DoorControl.h"
#ifndef _DONE_DOORCONTROL_CODE_
#define _DONE_DOORCONTROL_CODE_

#include "Library_and_pin.h"

//****************************************Game System SETUP**************************************************


//****************************************MQTT SETUP**************************************************
HAS2_MQTT has2_mqtt;

//****************************************Timer SETUP**************************************************
SimpleTimer DoorcheckTimer;
SimpleTimer EmergencyTimer;

void TimerInit();

void DoorCheck();
void EmergencyCheck();

int DoorCheckTimerId;
int EmergencyTimerId;

// DoorCheck setup
unsigned long DoorCheckTime = 1000;
int DoorCheckCNT = 0;

// Emergency setup
unsigned long EmergencyTime = 1000;
int EmergencyCNT = 0;


//****************************************EMlock SETUP**************************************************
bool IsRequestOpen = true;
bool IsDoorOpen = true;

enum {tDOOR = 0, sDOOR};
int DoorEm_PIN[2] = {tDOOR_PIN, sDOOR_PIN};
int DoorEm_REQ[2] = {0};
int DoorEm_STATE[2] = {0};

enum {tHOME = 0, tPUBLIC, sHOME, sPUBLIC};
int GuideLightPIN[4] = {tHOME_PIN, tPUBLIC_PIN, sHOME_PIN, sPUBLIC_PIN};
int GuideLight_STATE[4] = {0};

#endif