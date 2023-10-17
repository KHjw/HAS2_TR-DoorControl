void callback(char* topic, byte* payload, unsigned int length){
  String input_data = "";

  for(int i=0; i<length; i++)
    input_data += (char)payload[i];
  has2_mqtt.SaveByTopic(topic, input_data);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  Serial.println(input_data);

  GuideLightShow(0,0,0,0);
  DoorCheckCNT = 0;
  String myDS = has2_mqtt.GetData("my", "DS");
  if(myDS == "scenario"){
    String mySCN = has2_mqtt.GetData("my", "SCN");
    if(mySCN != current_scn){
      SCN_Apply(mySCN);
      current_scn = mySCN;
    }
  }
  if(myDS != current_ds){
    DS_Apply(myDS);
    current_ds = myDS;
  }
}

void Mqtt_myDN(String input_data){
  if((char)myDN[1] != 'A'){
    for(int i=0; i<3; i++)
      myDN += (char)input_data[i];
  }
}

void SCN_Apply(String data){
  Serial.println("Scenario at :: " + data);
  if(data == "p1" || data == "t1")  DoorOpenRequest(0,0);
  else if(data == "p54")            MoveTo(sCOMMON);
  else if(data == "p89")            MoveTo(sHOME);
  else if(data == "t52")            MoveTo(tCOMMON);
  else if(data == "t64")            MoveTo(tHOME);
  else if(data == "p60" || data == "p94" || data == "t54" || data == "t67"){
    DoorOpenRequest(0,0);
    GuideLightShow(0,0,0,0);
  }
  else                              Serial.println("Receved Unsigned SCN");
}

void DS_Apply(String data){
  if(data == "OTA")             has2_mqtt.FirmwareUpdate("emlock");
  else if(data == "setting")    DoorOpenRequest(1,1);
  else if(data == "ready")      DoorOpenRequest(0,0);
  else if(data == "activate")   DoorOpenRequest(1,1);
  else if(data == "open")       DoorOpenRequest(1,1);
  else if(data == "lock")       DoorOpenRequest(0,0);
  else if(data == "minigame")   DoorOpenRequest(1,1);
}

void LastDataApply(){
  DoorCheckCNT = 0;
  Serial.println("LastDataApply :: " + current_ds);
  if(current_ds == "scenario")
    SCN_Apply(current_scn);
  else
    DS_Apply(current_ds);
}

void SituationSend(){
  Serial.println("Stituation \"closed\" :: Publish");
  has2_mqtt.Situation("closed", myDN);
}

//****************************************Control SETUP**************************************************
void MoveTo(int where){
  switch (where){
  case tHOME:
    DoorOpenRequest(1,0);
    GuideLightShow(0,1,0,0);
    break;
  case tCOMMON:
    DoorOpenRequest(1,0);
    GuideLightShow(1,0,0,0);
    break;
  case sHOME:
    DoorOpenRequest(0,1);
    GuideLightShow(0,0,0,1);
    break;
  case sCOMMON:
    DoorOpenRequest(0,1);
    GuideLightShow(0,0,1,0);
    break;
  default:
    DoorOpenRequest(0,0);
    GuideLightShow(0,0,0,0);
    break;
  }
  // DoorCheckTimer.deleteTimer(DoorCheckTimerId);
  // DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}