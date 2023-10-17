void TestInit(){
  Serial.println("TestInit");
  TestTimerId = TestTimer.setInterval(TestTime, TestFunc);
}

void TestFunc(){
  Serial.println("TestCNT" + (String)(TestCNT));
  // int LCBP = has2_mqtt.GetData("my", "LCBP").toInt();
  Serial.println("my LCBP : " + (String)(has2_mqtt.GetData("my", "LCBP").toInt()));
  if(TestCNT == 10){
    DoorCheckCNT = 0;
    DoorCheckTimer.deleteTimer(DoorCheckTimerId); 
    if(TestOpen){
      Serial.println("Test :: All Door Open");
      DoorOpenRequest(1,0);
      GuideLightShow(1,1,1,1);
      TestOpen = false;
      current_ds = "TestOpen";
    }
    else{
      Serial.println("Test :: All Door Closed");
      DoorOpenRequest(0,0);
      GuideLightShow(0,0,0,0);
      TestOpen = true;
      current_ds = "TestClosed";
    }
    TestCNT = 0;
  }
  TestCNT ++;
}

void CheckInit(){
  Serial.println("CheckInit");
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}

void SerialControl(){
  if(Serial.available() > 0){
    String command = Serial.readString();
    Serial.println("Inupt :: Code " + command);
    
    if(command == "00"){
      GuideLightShow(0,0,0,0);
      DoorOpenRequest(0,0);
    }
    else if(command == "01"){
      GuideLightShow(0,0,1,1);
      DoorOpenRequest(0,1);
    }
    else if(command == "10"){
      GuideLightShow(1,1,0,0);
      DoorOpenRequest(1,0);
    }
    else if(command == "11"){
      GuideLightShow(1,1,1,1);
      DoorOpenRequest(1,1);
    }
    else
      Serial.println("ERROR");
  }
  Serial.print(".");
  delay(250);
}