#include <IRremote.hpp>
#include <EEPROM.h>
//int and const
int s1 = 0;
int s2 = 0;
int s3 = 0;
int dir = 1;
const int MR1 = 6;
int state_options = 0;
const int MR2 = 7;
const int ML1 = 2;
const int ML2 = 3;
const int BUTON1 = 14;
const int BUTON2 = 9;
const int Led1 = 10;
const int Led2 = 11;
int dir2 = 1;
int flag = 1;


const int Led3 = 12;
const int Led4 = 13;
const int Sen1 = A0;
const int Sen2 = A1;
const int Sen3 = A3;
const byte RECEIVE_PIN = 8;
const byte command_prog = 0xB;
const byte StartStop_command = 0x7;
const int ADDRESSEEPROM = 5;

void drive(int L, int R);

const int deltaT1 =
  800;
unsigned long endT1 = 0;


const int deltaT2 =
  1;
unsigned long endT2 = 0;





enum {
  cmdNO,
  cmdSTART,
  cmdSTOP,
  cmdPROGRAM,
  // StartStop_command,

};

int Snsr = 0;
int dohio = 2;
int ButonSTAIN1 = 0;
int ButonSTAIN2 = 0;


//читаю датчики
void readsensors() {
  s1 = !digitalRead(Sen1);
  s2 = !digitalRead(Sen2);
  s3 = !digitalRead(Sen3);
  digitalWrite(Led1, s1);
  digitalWrite(Led2, s2);
  digitalWrite(Led3, s3);
  Snsr = s1 * 4 + s2 * 2 + s3 * 1;
}

void once(){
  if(flag==1){

  }
}


//езда по прямой
void test() {

  drive(100, 100);
}

//светодиод моргает один раз
void Blink() {
  digitalWrite(Led1, 1);
  delay(500);
  digitalWrite(Led1, 0);
}

//работа с судейским пультом
int IR_Remote() {
  byte cmdRet = cmdNO;
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.address == command_prog) {
      dohio = IrReceiver.decodedIRData.command;
      cmdRet = cmdPROGRAM;
      digitalWrite(Led3, 1);
      delay(500);
      digitalWrite(Led3, 0);
      EEPROM.write(ADDRESSEEPROM, dohio);
      EEPROM.commit();
      //saveEEPROM();
      Serial.println(dohio, HEX);
    }
    if (IrReceiver.decodedIRData.address == StartStop_command) {
      if (IrReceiver.decodedIRData.command == dohio + 1) {
        cmdRet = cmdSTART;
      }
    }
    if (IrReceiver.decodedIRData.address == StartStop_command) {
      if (IrReceiver.decodedIRData.command == dohio) {
        cmdRet = cmdSTOP;
      }
    }

    IrReceiver.resume();
  }
  return cmdRet;
}

//поиск противника
void milllis() {
  readsensors();

  if (millis() >= endT1) {
    dir = dir * (-1);

    

    if(flag==1){
       endT1 = millis() + deltaT1/2;
       flag=0;  
          }
          else  {endT1 = millis() + deltaT1;}
  }


//
//drive(-40,40);
//delay(40);
//drive(0,0);
  drive(dir * 40, -dir * 40);
  if (Snsr > 0) { return; }
}

//выезд 
void millis_run(int period) {

  digitalWrite(Led2, 1);
  endT2 = millis() + period;
  drive(100, 100);

  while (millis() < endT2) {
    readsensors();

    if (Snsr > 0) {
      drive(0, 0);
      return;
    }

  }  //while



  drive(0, 0);

  return;
}

//работа с судейским пультом №2
bool chekIReceive() {

  bool result = false;
  if (IrReceiver.decode()) {

    if ((IrReceiver.decodedIRData.address) == 0x0B) {

      dohio = IrReceiver.decodedIRData.command;
      Serial.println("Programming");
      Blink();

      result = true;

    } else if ((IrReceiver.decodedIRData.address) == 0x07) {
      // start/stop
      if ((IrReceiver.decodedIRData.command) == (dohio + 1)) {

        // -------------------------------------------------------command start
        Serial.println("Start");
        result = true;
        Blink();
        // BoiStart();

      } else if ((IrReceiver.decodedIRData.command) == dohio) {

        // ------------------------------------------------------command stop
        result = true;
        Serial.println("Stop");
        // Blink();
        //drive(0, 0);
      }
    }
    //IrReceiver.printIRResultShort(&Serial, true);
    IrReceiver.resume();
  }
  return result;
}

//бой
void BoiStart_run() {
drive(120,120);
delay(55);
drive(0,0);
  millis_run(250);



  drive(0, 0);
  while (1) {
    int cmdRet = IR_Remote();
    if (cmdRet == cmdSTOP) {
      drive(0, 0);
      while (1);
        
    }
    readsensors();

    if (Snsr == 0) {
      ///drive(40, -40);
      ///delay(50);
      //drive(0, 0);
      milllis();
    } else if (Snsr == 3) {
      drive(155, 100);
    } else if (Snsr == 6) {
      drive(100, 155);
    }



    else if (Snsr == 1) {
      drive(165, 95);
    } else if (Snsr == 4) {
      drive(95, 165);
    }


    else if (Snsr == 2) {
      drive(125, 125);
    } else if (Snsr == 5) {
      drive(160, 160);
    } else if (Snsr == 7) {
      drive(255, 255);
    }
    //readsensors();
  }
}

//драйв езда
void drive(int L, int R) {
  R = R * 1.4;
  R = constrain(R, -255, 255);
  L = constrain(L, -255, 255);
  if (R > 255) R = 255;
  if (L > 0) {
    digitalWrite(ML1, 1);
    analogWrite(ML2, 255 - L);
  } else {
    digitalWrite(ML2, 1);
    analogWrite(ML1, 255 + L);
  }

  if (R > 0) {
    digitalWrite(MR1, 1);
    analogWrite(MR2, 255 - R);
  } else {
    digitalWrite(MR2, 1);
    analogWrite(MR1, 255 + R);
  }
}

void setup() {
  //endT1 = millis() + deltaT1;/////
  //endT2 = millis() + deltaT2;/////

  EEPROM.begin(256);
  pinMode(ML2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(Sen1, INPUT);
  pinMode(Sen2, INPUT);
  pinMode(Sen3, INPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(BUTON1, INPUT_PULLUP);
  pinMode(BUTON2, INPUT_PULLUP);
  pinMode(RECEIVE_PIN, INPUT);
  IrReceiver.begin(RECEIVE_PIN);
  dohio = EEPROM.read(ADDRESSEEPROM);
}

void loop() {
  //milllis();
  drive(0, 0);
  int cmdRet = IR_Remote();
  if (cmdRet == cmdSTART) {

    BoiStart_run();
    Blink();
  }
  readsensors();

  
  

  // millis_run(10000);
}
