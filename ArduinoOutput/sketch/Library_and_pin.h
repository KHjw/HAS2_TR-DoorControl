#line 1 "c:\\Github\\HAS2_TR\\HAS2_TR-DoorControl\\Library_and_pin.h"
#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include <HAS2_MQTT.h>
#include <SimpleTimer.h>

#define NEOPIN       27

#define tDOOR_PIN    18  
#define sDOOR_PIN    19
#define tDOOR_EMERGENCY    21
#define sDOOR_EMERGENCY    22

#define tHOME_PIN    12
#define tPUBLIC_PIN  13
#define sHOME_PIN    14
#define sPUBLIC_PIN  15