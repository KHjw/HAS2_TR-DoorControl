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