

#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "NTPClient.h"

//const char *ssid = "Dialog 4G";
//const char *password = "746YL1F1LRR";

const char *ssid = "Hotcat Technologies";
const char *password = "hotcat@pan2021";

const long utcOffsetInSeconds = 9600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int SER_Pin = D7;   //pin 14 on the 75HC595 data
int RCLK_Pin = D8;  //pin 12 on the 75HC595 lunch
int SRCLK_Pin = D5; //pin 11 on the 75HC595 clock

int sec_pin = D4; //

//How many of the shift registers - change this ***
#define number_of_74hc595N 2

//numOfRegisterPins Counting number of pins
#define numOfRegisterPins number_of_74hc595N * 8

boolean registers[numOfRegisterPins];

int pins[] = {0, 1, 2, 3, 4, 5, 6, 7};
boolean numbers[10] [7] = {
  {1, 1, 1, 1, 1, 1, 0 },
  {0, 0, 1, 1, 0, 0, 0 },
  {1, 1, 0, 1, 1, 0, 1 },
  {0, 1, 0, 1, 1, 0, 1 },
  {0, 0, 1, 1, 0, 1, 1 },
  {0, 1, 1, 0, 1, 1, 1 },
  {1, 1, 1, 0, 1, 1, 1 },
  {0, 0, 1, 1, 1, 0, 0 },
  {1, 1, 1, 1, 1, 1, 1 },
  {0, 1, 1, 1, 1, 1, 1 },
};

boolean anode = 0;
boolean cathode = 1;
boolean mode = cathode;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);

  pinMode (sec_pin, OUTPUT);

  led();

  //  for (int j = 2; j < 10; j++)
  //    pinMode(j, OUTPUT);

  //reset all register pins
  clearRegisters();
//  for (int i = 8; i < 16; i++) {
//    setRegisterPin(i, LOW);
//  }
  writeRegisters();
}


//set all register pins to LOW
void clearRegisters() {
  for (int i = numOfRegisterPins - 1; i >=  0; i--) {
    registers[i] = LOW;
  }
}

void writeRegisters() {

  digitalWrite(RCLK_Pin, LOW);

  for (int i = numOfRegisterPins - 1; i >=  0; i--) {
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

void setRegisterPin(int index) {
  registers[index];
}
int digitCount = 4;
int digits[] = { -1, -1, -1, -1};


void loop() {

  writedisplay();

  timeClient.update();
  String time1 = timeClient.getFormattedTime();
  Serial.println(time1 + "");

//String time1="6666";

  int wp = digitCount - 1;
  int i = 0;
  for (; i < time1.length(); i++) {
    int digit = ((int)time1.charAt(i)) - 48;
    digits[wp] = digit;
    wp--;
    if (wp == -1) {
      break;
    }
  }
  for (; i < digitCount; i++) {
    digits[0] = -1;// numbert anith paththata write wena eka
  }




}


void display(int no) {

  for (int i = 0; i < 7; i++) {
    if (numbers[no] [i]) {
      
//      Serial.println(numbers[no] [i]);
       Serial.println("1");
      delay(1000);
      setRegisterPin(pins[i]);
    } else {
       Serial.println("0");
      setRegisterPin(pins[i]);
    }
  }
}


int writedigit = 0;
void writedisplay() {
  setRegisterPin(8 + writedigit);

  display(digits[writedigit]);
  writeRegisters();

  setRegisterPin(8 + writedigit);
  writedigit++;
  if (writedigit == digitCount) {
    writedigit = 0;
  }
}

//7805 5v reguletor
void led(){
  
  
  digitalWrite(sec_pin, HIGH);
  delay(1000);
  digitalWrite(sec_pin, LOW);
  delay(1000);
  Serial.println("Secound"+sec_pin);
  }

//  Serial.println("Time as String");
//  Serial.println(no);
//  Serial.println("-----------------");
//
//  Serial.println("Time String Length ");
//  Serial.println(no.length());
//  int timeAr[no.length()];
//  for(int i =0; i<no.length(); i++){
//    int a = int( no.charAt(i));
////    int value = (int)a.inChar;
//    Serial.println(a);
//    delay(3000);
//   display(a);
//  }
//    timeAr[i]=a;



// Serial.println("Time as String");
//Serial.println(no);
//Serial.println("-----------------");
