#ifndef _DONE_DOORCONTROL_CODE_
#define _DONE_DOORCONTROL_CODE_

#include "Library_and_pin.h"

//****************************************MQTT SETUP**************************************************
HAS2_MQTT has2_mqtt;

String myMAC = "";
String myDN = "";
String current_ds = "";
String current_scn = "";

void callback(char* topic, byte* payload, unsigned int length);
void Mqtt_myDN(String input_data);
void SCN_Apply(String data);
void DS_Apply(String data);
void LastDataApply();
void SituationSend();

void MoveTo(int where);

//****************************************Timer SETUP**************************************************
SimpleTimer DoorCheckTimer;
SimpleTimer TaggerEnterTimer;
SimpleTimer SurvivorEnterTimer;
int DoorCheckTimerId;
int TaggerEnterTimerId;
int SurvivorEnterTimerId;

void TimerInit();
void DoorCheck();
void TaggerEnter();
void SurvivorEnter();
  
// DoorCheck setup
unsigned long DoorCheckTime = 500;
int DoorCheckCNT = 0;
int DoorCheckLimit = 8;             // 2초부터 .5초마다

// TaggerEnter setup
unsigned long TaggerEnterTime = 500;
int TaggerEnterCNT = 0;
int TaggerEnterLimit = 4;           // 2초동안 도어 오픈

// SurvivorEnter setup
unsigned long SurvivorEnterTime = 500;
int SurvivorEnterCNT = 0;
int SurvivorEnterLimit = 4;         // 2초동안 도어 오픈


//****************************************EMlock SETUP**************************************************
void DoorSystemInit();

// Em Setup
enum {tDOOR = 0, sDOOR};
int DoorEm_PIN[2] = {tOPEN_PIN, sOPEN_PIN};
int DoorEm_REQ[2] = {0};
int EmSensor_PIN[2] = {tCHECK_PIN, sCHECK_PIN};
int EmSensor_STATE[2] = {0};

void DoorOpenRequest();
void DoorOpenRequest(int a, int b);

// GuideLight Setup
enum {tHOME = 0, tCOMMON, sHOME, sCOMMON};
int GuideLight_PIN[4] = {tHOME_PIN, tCOMMON_PIN, sHOME_PIN, sCOMMON_PIN};
int GuideLight_STATE[4] = {0};

void GuideLightShow();
void GuideLightShow(int a, int b, int c, int d);

// EmergencyCheck Setup
int EmergencyCode = 0;
void EmergencyCheckLoop();
// EnterCheck Setup
void EnterCheckLoop();
void TaggerEnterCheck();
void SurvivorEnterCheck();
void EnterCheckWait();

void (*tEnter_ptr)() = TaggerEnterCheck;
void (*sEnter_ptr)() = SurvivorEnterCheck;

//****************************************Test SETUP**************************************************
SimpleTimer TestTimer;
int TestTimerId;

void TestInit();
void TestFunc();
void CheckInit();

unsigned long TestTime = 1000;
int TestCNT = 10;
bool TestOpen = true;

#endif