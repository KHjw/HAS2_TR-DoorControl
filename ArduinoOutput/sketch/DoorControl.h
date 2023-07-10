#line 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\DoorControl.h"
#ifndef _DONE_DOORCONTROL_CODE_
#define _DONE_DOORCONTROL_CODE_

#include "Library_and_pin.h"

//****************************************MQTT SETUP**************************************************
HAS2_MQTT has2_mqtt;

void callback(char* topic, byte* payload, unsigned int length);
void MoveTo(int where);

//****************************************Timer SETUP**************************************************
SimpleTimer DoorCheckTimer;
int DoorCheckTimerId;

void TimerInit();
void DoorCheck();

// DoorCheck setup
unsigned long DoorCheckTime = 500;
int DoorCheckCNT = 0;
int DoorCheckLimit = 4;           // 2초부터 .5초마다

//****************************************EMlock SETUP**************************************************
void DoorSystemInit();

// Em Setup
enum {tDOOR = 0, sDOOR};
int DoorEm_PIN[2] = {tDOOR_PIN, sDOOR_PIN};
int DoorEm_REQ[2] = {0};
int EmSensor_PIN[2] = {tSENSOR_PIN, sSENSOR_PIN};
int DoorEm_STATE[2] = {0};

void DoorOpen(int a, int b);

// GuideLight Setup
enum {tHOME = 0, tPUBLIC, sHOME, sPUBLIC};
int GuideLight_PIN[4] = {tHOME_PIN, tPUBLIC_PIN, sHOME_PIN, sPUBLIC_PIN};
int GuideLight_STATE[4] = {0};

void GuideLightShow();
void GuideLightShow(int a, int b, int c, int d);

// EmergencyCheck Setup
int EmergencyCode = 0;

void EmergencyCheck();

#endif