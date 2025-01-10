const int MR1 = 7;
const int MR2 = 6;
const int ML1 = 2;
const int ML2 = 3;
const int BUTON1 = 14;
const int BUTON2 = 9;
const int Led1 = 10;
const int Led2 = 11;
const int Led3 = 12;
const int Led4 = 13;




void drive(int L, int R) {

  if (L > 255) L = 255;
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
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(BUTON1, INPUT_PULLUP);
  pinMode(BUTON2, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BUTON2) == 0) { drive(0, 100); }
  if (digitalRead(BUTON1) == 0) { drive(100, 0); }
}
