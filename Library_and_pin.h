#include <Arduino.h>

#include <HAS2_MQTT.h>
#include <SimpleTimer.h>

#define tDOOR_PIN    18
#define sDOOR_PIN    19
#define tSENSOR_PIN  21
#define sSENSOR_PIN  22
#define tDOOR_EMERGENCY   5
#define sDOOR_EMERGENCY   2

#define tHOME_PIN    12
#define tPUBLIC_PIN  13
#define sHOME_PIN    14
#define sPUBLIC_PIN  15