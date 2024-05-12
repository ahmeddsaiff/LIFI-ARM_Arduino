#include <LiquidCrystal.h>

int ir_rx = 3;
int mute = 13;
int led = 10;
int buzzer = 11;

int time_on = 0;
unsigned long time_start = 0;
unsigned long time_end = 0;
unsigned long time_now ;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.print("Closed!!");
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(mute, INPUT);
  pinMode(ir_rx, INPUT);
  Serial.begin(9600);
}

void loop() {
  int mute_val = digitalRead(mute);
  int ir_val = digitalRead(ir_rx);

  Serial.println(time_start);
  Serial.println(time_end);
  Serial.println(time_end - time_start);
  Serial.println();
  // delay(2000);

  if (mute_val == 1) {
    time_start = 0;
    time_end = 0;
    lcd.clear();
    lcd.print("muted...");
    for (int i = 0; i < 50; i++) {
      flashing();
    }
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }
  if (ir_val == 0) {
    if (time_on == 0) {
      time_on = 1;
      time_start = millis();
    }

  } else {
    if (time_on == 1) {
      time_on = 0;
      time_end = millis();
    }
  }
  //Start button is pressed
  //500 ms
  if ((time_end - time_start) >= 100 && (time_end - time_start) <= 700) {
    lcd.clear();
    lcd.print("All is okay..");
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
  }
  //Gas sensor 
  //1 sec
  if ((time_end - time_start) >= 800 && (time_end - time_start) <= 1200) {
    lcd.clear();
    lcd.print("Fire is detected !");
    flashing();
  }
  //UltraSonic sensor
  //1.5 secs
  if ((time_end - time_start) >= 1300 && (time_end - time_start) <= 1700) {
    lcd.clear();
    lcd.print("Door is opened !");
    flashing();
  }
  //Magnetic switch
  //2 sec
  if ((time_end - time_start) >= 1800 && (time_end - time_start) <= 2200) {
    lcd.clear();
    lcd.print("Intusion !!");
    flashing();
  }
  //Stop button is pressed
  // 2.5 sec
  if ((time_end - time_start) >= 2300 && (time_end - time_start) <= 2700) {
    lcd.clear();
    lcd.print("Closed !!");
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    time_start = 0;
    time_end = 0;
  }
}

void flashing() {
  digitalWrite(buzzer, HIGH);

  time_now = millis();
  digitalWrite(led, HIGH);
  while(millis() < (time_now + 50));

  time_now = millis();
  digitalWrite(led, LOW);
  while(millis() < (time_now + 50));
  // delay(500);
}