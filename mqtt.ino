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