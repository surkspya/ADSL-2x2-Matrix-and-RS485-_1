#include <SoftwareSerial.h>

SoftwareSerial RS485Serial(5, 3);  


const int r1 = 11;
const int g1 = 12;
const int b1 = 10;

const int r2 = 13;
const int g2 = 6;
const int b2 = 7;

const int c1 = 8;
const int c2 = 9;

int pattern = 0;
unsigned long lastrefresh = 0;
int refreshinterval = 2; 

bool ledcolor[2][2][3];  
          // ROW, COL, RGB

void setup() {
  pinMode(r1, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);

  clearLEDS();
  setPattern(pattern);

  RS485Serial.begin(9600);
}

void clearLEDS() {
  digitalWrite(r1, LOW);
  digitalWrite(g1, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(g2, LOW);
  digitalWrite(b2, LOW);
  digitalWrite(c1, HIGH);
  digitalWrite(c2, HIGH);
}

void refreshleds() {
  static int col = 0;
  clearLEDS();

  if (col == 0) {
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);

    digitalWrite(r1, ledcolor[0][0][0]);
    digitalWrite(g1, ledcolor[0][0][1]);
    digitalWrite(b1, ledcolor[0][0][2]);

    digitalWrite(r2, ledcolor[1][0][0]);
    digitalWrite(g2, ledcolor[1][0][1]);
    digitalWrite(b2, ledcolor[1][0][2]);
  } else {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);

    digitalWrite(r1, ledcolor[0][1][0]);
    digitalWrite(g1, ledcolor[0][1][1]);
    digitalWrite(b1, ledcolor[0][1][2]);

    digitalWrite(r2, ledcolor[1][1][0]);
    digitalWrite(g2, ledcolor[1][1][1]);
    digitalWrite(b2, ledcolor[1][1][2]);
  }
  col = 1 - col;
}

void setPattern(int p) {
  clearLEDS();
  memset(ledcolor, 0, sizeof(ledcolor));

  if (p == 0) {
    ledcolor[0][0][0] = 1; // ro1/co1 red
    ledcolor[1][1][1] = 1; // ro2/co2 blue
  } else if (p == 1) {
    ledcolor[0][1][2] = 1; // ro1/co2 blue
    ledcolor[1][0][1] = 1; // ro2/co1 green
  } else if (p == 2) {
    ledcolor[0][0][0] = 1; // ro1/co1 red
    ledcolor[0][1][1] = 1; // ro1/co2 green
    ledcolor[1][0][2] = 1; // ro2/co1 blue
    ledcolor[1][1][2] = 1; // ro2/co2 blue
  } else if (p == 3) {
    ledcolor[0][0][0] = 1; // ro1/co1 red
    ledcolor[0][1][1] = 1; // ro1/co2 green
    ledcolor[1][0][2] = 1; // ro2/co1 blue
    ledcolor[1][1][0] = 1; // ro2/co2 red
    ledcolor[1][1][1] = 1; // ro2/co2 green
  }
}

void loop() {
  // check RS485 data
  if (RS485Serial.available()) {
    String msg = RS485Serial.readStringUntil('\n');
    msg.trim();
    if (msg == "BUTTON_PRESSED") {
      pattern = (pattern + 1) % 4; 
      setPattern(pattern);
    }
  }

  // refresh LEDs fast enough for persistence
  if (millis() - lastrefresh >= refreshinterval) {
    lastrefresh = millis();
    refreshleds();
  }
}
