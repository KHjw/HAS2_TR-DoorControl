# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
 /*

 * @file HAS2_TR-DoorControl.ino

 * @author 홍진우 KH.jinu (kevinlike@naver.com)

 * @brief

 * @version 1.0

 * @date 2023-07-07

 *

 * @copyright Copyright (c) 2023

 */
# 11 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino"
# 12 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\HAS2_TR-DoorControl.ino" 2

void setup(void){
  Serial.begin(115200);
  Serial.println("=============================ESP SETUP=============================");
  has2_mqtt.Setup("train_room", "Code3824@", callback); // ALL, myMAC 구독
  has2_mqtt.Situation("start", "my");
  TimerInit();
  DoorSystemInit();
  // TestInit();
  // CheckInit();
  GuideLightShow(1,1,1,1);
  DoorOpenRequest(1,1);
  Serial.println("===================================================================");
}

void loop(void){
  // SerialControl();
  // EnterCheckLoop();
  // EmergencyCheckLoop();
  has2_mqtt.ReadSubscirbe();
  // DoorCheckTimer.run();
  // TaggerEnterTimer.run();
  // SurvivorEnterTimer.run();
}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\emlock.ino"
void DoorSystemInit(){
  Serial.println("DoorSystemInit");
  for(int i=0; i<2; i++)
    pinMode(DoorEm_PIN[i], 0x03);
  for(int i=0; i<4; i++)
    pinMode(GuideLight_PIN[i], 0x03);
  for(int i=0; i<2; i++)
    pinMode(EmSensor_PIN[i] ,0x05);
  pinMode(14, 0x05);
  pinMode(34, 0x05);
  pinMode(15, 0x05);
  pinMode(35, 0x05);
}

//****************************************Em SETUP**************************************************
void DoorOpenRequest(){
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1) digitalWrite(DoorEm_PIN[i], 0x1);
    else digitalWrite(DoorEm_PIN[i], 0x0);
  }
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}

void DoorOpenRequest(int a, int b){
  DoorEm_REQ[0] = a;
  DoorEm_REQ[1] = b;
  for(int i=0; i<2; i++){
    if(DoorEm_REQ[i] == 1) digitalWrite(DoorEm_PIN[i], 0x1);
    else digitalWrite(DoorEm_PIN[i], 0x0);
  }
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
}

//****************************************GuideLight SETUP**************************************************
void GuideLightShow(){
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1) digitalWrite(GuideLight_PIN[i], 0x1);
    else digitalWrite(GuideLight_PIN[i], 0x0);
  }
}

void GuideLightShow(int a, int b, int c, int d){
  GuideLight_STATE[0] = a;
  GuideLight_STATE[1] = b;
  GuideLight_STATE[2] = c;
  GuideLight_STATE[3] = d;
  for(int i=0; i<4; i++){
    if(GuideLight_STATE[i] == 1) digitalWrite(GuideLight_PIN[i], 0x1);
    else digitalWrite(GuideLight_PIN[i], 0x0);
  }
}

//****************************************EmergencyCheck SETUP**************************************************
void EmergencyCheckLoop(){
  int EmergencyCNT = 0;
  if(digitalRead(14) == 0x0) EmergencyCNT += 1;
  if(digitalRead(34) == 0x0) EmergencyCNT += 2;
  Serial.println("EmergencyCNT : " + (String)(EmergencyCNT));
  if(EmergencyCNT != EmergencyCode){
    Serial.println("EmergencyCode : " + (String)(EmergencyCode));
    switch (EmergencyCNT){
    case 0:
      Serial.println("EMERGENCY OVER");
      has2_mqtt.Send("my", "EMRG", "null");
      GuideLightShow(0,0,0,0);
      // LastDataApply();
      break;
    case 1:
      // LastDataApply();
      Serial.println("TAGGER DOOR EMERGENCY OPEN");
      has2_mqtt.Send("my", "EMRG", "tagger");
      GuideLight_STATE[0] = 1;
      GuideLight_STATE[1] = 1;
      GuideLightShow();
      break;
    case 2:
      // LastDataApply();
      Serial.println("SURVIVOR DOOR EMERGENCY OPEN");
      has2_mqtt.Send("my", "EMRG", "player");
      GuideLight_STATE[2] = 1;
      GuideLight_STATE[3] = 1;
      GuideLightShow();
      break;
    case 3:
      Serial.println("ALL DOOR EMERGENCY OPEN");
      has2_mqtt.Send("my", "EMRG", "all");
      GuideLightShow(1,1,1,1);
      break;
    default:
      break;
    }
    EmergencyCode = EmergencyCNT;
    delay(50);
  }
}

//****************************************DigitalSWCheck SETUP**************************************************
void EnterCheckLoop(){
  if(EmSensor_STATE[0] == 0)
    if(digitalRead(15) == 0x0) tEnter_ptr();
  if(EmSensor_STATE[1] == 0)
    if(digitalRead(35) == 0x0) sEnter_ptr();
}

//Enter_ptr 함수
void TaggerEnterCheck(){
  DoorEm_REQ[0] = 1;
  DoorOpenRequest();

  tEnter_ptr = EnterCheckWait;
  Serial.println("Tagger Door Enter!!!");
  TaggerEnterCNT = 0;
  TaggerEnterTimer.deleteTimer(TaggerEnterTimerId);
  TaggerEnterTimerId = TaggerEnterTimer.setInterval(TaggerEnterTime, TaggerEnter);
}

void SurvivorEnterCheck(){
  DoorEm_REQ[1] = 1;
  DoorOpenRequest();

  sEnter_ptr = EnterCheckWait;
  Serial.println("Survivor Door Enter!!!");
  SurvivorEnterCNT = 0;
  SurvivorEnterTimer.deleteTimer(SurvivorEnterTimerId);
  SurvivorEnterTimerId = SurvivorEnterTimer.setInterval(SurvivorEnterTime, SurvivorEnter);
}

void EnterCheckWait(){}
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\mqtt.ino"
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
  if(data == "p1" || data == "t1") DoorOpenRequest(0,0);
  else if(data == "p54") MoveTo(sCOMMON);
  else if(data == "p89") MoveTo(sHOME);
  else if(data == "t52") MoveTo(tCOMMON);
  else if(data == "t64") MoveTo(tHOME);
  else if(data == "p60" || data == "p94" || data == "t54" || data == "t67"){
    DoorOpenRequest(0,0);
    GuideLightShow(0,0,0,0);
  }
  else Serial.println("Receved Unsigned SCN");
}

void DS_Apply(String data){
  if(data == "OTA") has2_mqtt.FirmwareUpdate("emlock");
  else if(data == "setting") DoorOpenRequest(1,1);
  else if(data == "ready") DoorOpenRequest(0,0);
  else if(data == "activate") DoorOpenRequest(1,1);
  else if(data == "open") DoorOpenRequest(1,1);
  else if(data == "lock") DoorOpenRequest(0,0);
  else if(data == "minigame") DoorOpenRequest(1,1);
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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\test.ino"
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
# 1 "c:\\Github\\HAS2-TR\\0_KHjinu\\HAS2_TR-DoorControl\\timer.ino"
void TimerInit(){
  Serial.println("TimerInit");
  DoorCheckTimerId = DoorCheckTimer.setInterval(DoorCheckTime, DoorCheck);
  DoorCheckTimer.deleteTimer(DoorCheckTimerId);
  TaggerEnterTimerId = TaggerEnterTimer.setInterval(TaggerEnterTime, TaggerEnter);
  TaggerEnterTimer.deleteTimer(TaggerEnterTimerId);
  SurvivorEnterTimerId = SurvivorEnterTimer.setInterval(SurvivorEnterTime, SurvivorEnter);
  SurvivorEnterTimer.deleteTimer(SurvivorEnterTimerId);
}

void DoorCheck(){
  if(DoorCheckCNT >= 40) DoorCheckTimer.deleteTimer(DoorCheckTimerId);
  Serial.print("[" + (String)(DoorCheckCNT) + "] ");
  for(int i=0; i<2; i++){
    if(digitalRead(EmSensor_PIN[i]) == 0x0) EmSensor_STATE[i] = 1;
    else EmSensor_STATE[i] = 0;
  }

  if(DoorCheckCNT < DoorCheckLimit){
    if(DoorCheckCNT%2 == 0){
      long untilDoorCheck = (DoorCheckLimit-DoorCheckCNT+1)*0.5;
      Serial.println("DoorCheck in " + (String)(untilDoorCheck) + "sec");
    }
  }
  else{
    if(DoorEm_REQ != EmSensor_STATE){
      if(DoorEm_REQ[0] != EmSensor_STATE[0]){
        Serial.print("Tagger Door Error :: ");
        if(DoorEm_REQ[0] == 1){
          Serial.println("Not Open");
        }
        else{
          Serial.println("Not Closed");
        }
      }
      if(DoorEm_REQ[1] != EmSensor_STATE[1]){
        Serial.print("Survivor Door Error :: ");
        if(DoorEm_REQ[1] == 1){
          Serial.println("Not Open");
        }
        else{
          Serial.println("Not Closed");
        }
      }
    }
    else{
      DoorOpenRequest(0,0);
      GuideLightShow(0,0,0,0);
      DoorCheckCNT = 0;
      DoorCheckTimer.deleteTimer(DoorCheckTimerId);
    }
  }
  DoorCheckCNT++;
}

void TaggerEnter(){
  long sec_remain = (TaggerEnterLimit-TaggerEnterCNT)* 0.5;
  Serial.println("Tagger Door Open for :: " + (String)(sec_remain) + " secondes");
  if(TaggerEnterCNT > TaggerEnterLimit){
    LastDataApply();
    tEnter_ptr = TaggerEnterCheck;
    TaggerEnterCNT = 0;
    TaggerEnterTimer.deleteTimer(TaggerEnterTimerId);
  }
  TaggerEnterCNT++;
}

void SurvivorEnter(){
  long sec_remain = (SurvivorEnterLimit-SurvivorEnterCNT)* 0.5;
  Serial.println("Survivor Door Open for :: " + (String)(sec_remain) + " secondes");
  if(SurvivorEnterCNT > SurvivorEnterLimit){
    LastDataApply();
    sEnter_ptr = SurvivorEnterCheck;
    SurvivorEnterCNT = 0;
    SurvivorEnterTimer.deleteTimer(SurvivorEnterTimerId);
  }
  SurvivorEnterCNT++;
}
