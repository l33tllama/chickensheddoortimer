
/* Door timer by Leo

*/
// TODO: Swap pin 2 or 3 for 10 - to enable interrupts and sleep
// mode for power savings..
/* Pins in use
 *  0,1 - TX,RX for debug
 *  2, 3, 4, 5 - LCD
 *  6, 7, 8 - Rotary encoder
 *  9 - Piezo
 *  10 - unused (PWM!)
 *  11, 12 - LCD
 *  13 - unused
 */
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Button.h>
#include "time.h"
#include "sun_moon_time.h"
#include "RTClib.h"

// Pins
#define PIN_ENC_A 7
#define PIN_ENC_B 8
#define PIN_ENC_BTN 6 
#define PIN_PIEZO 9

// Button settings
#define DEBOUNCE_MS 20
#define PULLUP false
#define INVERT false

// Encoder states
#define ENC_SAME 0
#define ENC_INC 1
#define ENC_DEC 2

// Menu states
#define STATE_MAIN_MENU 0x01
#define STATE_MANUAL_CONTROL 0x02
#define STATE_OPEN_TIME_SET 0x03
#define STATE_CLOSE_TIME_SET 0x04
#define STATE_DATETIME_SET 0x05
#define MENU_COUNT 4

#define STATE_DATE_SET 0x00
#define STATE_TIME_SET 0x01

int encoderPos = 0;
int lastEncoderPos = 0;
int encoderPinALast = LOW;
int n = LOW;
int menuPos = 0;
int menuState = STATE_MAIN_MENU;
int datetimeState = STATE_DATE_SET;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", 
  "Wednesday", "Thursday", "Friday", "Saturday"};
  
char menuList[MENU_COUNT][16] = {"Manual control", "Set open time", 
  "Set close time", "Change time" };

Button menuButton(PIN_ENC_BTN, PULLUP, INVERT, DEBOUNCE_MS);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

RTC_DS1307 rtc;

struct tm * time_from_rtc;
time_t * rtc_time;


// TODO: TimeLord - auto sunrise+sunset would be pretty good I reckon..
// * Manual control
// * check open time - later add override
// * check close time - later add override
// * set date + time
// * set timezone (by GMT) - later by country and state..

void setup(){
  
  // setup LCD
  lcd.begin(16, 2);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  // setup RTC
  if (! rtc.isrunning()){
    Serial.println("RTC is not running! PANIC!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // comment out to set time and date
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // setup piezo
  
  // setup rotary encoder
  pinMode(PIN_ENC_A, INPUT);
  pinMode(PIN_ENC_B, INPUT);  
  
  // setup serial
  Serial.begin(9600);
  
  lcd.print("Door Timer ");
  lcd.setCursor(0, 1);
  lcd.print("by Leo Febey");
  delay(100);
  lcd.clear();

  DateTime now = rtc.now();

  Serial.print("Date: ");
  Serial.print(now.day());
  Serial.print(" : ");
  Serial.print(now.month(), DEC);
  Serial.print(" : ");
  Serial.print(now.year(), DEC);
  Serial.print("\nTime: ");
  Serial.print(now.hour(), DEC);
  Serial.print(" : ");
  Serial.print(now.minute(), DEC);
  Serial.print(" : ");
  Serial.print(now.second(), DEC);
  Serial.print("\n");

  // Set location using time.h library for later sunrise/sunset calculation
  // TODO in later firmware update - set location menu
  setLocation(HOBART_TAS);

  time_from_rtc = localtime(rtc_time);
  time_from_rtc->tm_sec = now.second();
  time_from_rtc->tm_min = now.minute();
  time_from_rtc->tm_hour = now.hour();
  time_from_rtc->tm_mday = now.day();
  time_from_rtc->tm_mon = now.month() - 1; // 0-11
  time_from_rtc->tm_year = now.year() - 1900; // years since 1900

  //rtc_time = mktime(time_from_rtc);

  //time_t * tm_ptr = &rtc_time;

  double * azimuth;
  double * altitude;
  SolarPosition(rtc_time, azimuth, altitude);
  //= SolarAzimuth(rtc_time);

  Serial.print("Solar Azumith: ");
  Serial.println(*azimuth);
  Serial.print("Solar Altitude: ");
  Serial.println(*altitude);
  
  
  showMenu();
  
}
inline int encoderPosRead(){
  lastEncoderPos = encoderPos;
  n = digitalRead(PIN_ENC_A);
   if ((encoderPinALast == LOW) && (n == HIGH)) {
     if (digitalRead(PIN_ENC_B) == LOW) {
       encoderPos--;
     } else {
       encoderPos++;
     }
     Serial.print (encoderPos);
     Serial.println ("/");
   } 
   encoderPinALast = n;
    if(encoderPos == lastEncoderPos){
      return ENC_SAME;
    } else if (encoderPos > lastEncoderPos){
      Serial.println("Inc");
      
      return ENC_INC;
    } else if (encoderPos < lastEncoderPos){
      Serial.println("Dec");
      return ENC_DEC;
    }
   return ENC_SAME;
}

void showMenu(){
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuPos);
  lcd.print(">");
  
  // Show first menu item
  lcd.print(menuList[menuPos]);
  lcd.setCursor(0, 1);

  // Show second menu item - loop around if above is at last
  if (menuPos < MENU_COUNT - 1){
    lcd.print(menuPos + 1);
    lcd.print(" ");
    lcd.print(menuList[menuPos+1]);
  } else {
    lcd.print(0);
    lcd.print(" ");
    lcd.print(menuList[0]);
  }
  
}

void checkMainMenuButonPress(){
  menuButton.read();
  if(menuButton.wasReleased()){
    Serial.println("Menu button pressed.. once..?");
    switch(menuPos){
      case 0:
        Serial.println("Entering Manual Control..");
        menuState = STATE_MANUAL_CONTROL;
        break;
      case 1:
        Serial.println("Entering set open time..");
        menuState = STATE_OPEN_TIME_SET;
        break;
      case 2:
        Serial.println("Entering set close time..");
        menuState = STATE_CLOSE_TIME_SET;
        break;
       case 3:
        Serial.println("Entering set date+time..");
        showChangeTimeMenu();
        menuState = STATE_DATETIME_SET;
        break;
       default:
        Serial.print("Menu pos out of bounds: ");
        Serial.println(menuPos);
        break;
    }
  }
}

void showManualControlMenu(){
  
}

void showOpenTimeMenu(){
  
}

void showCloseTimeMenu(){
  
}

void changeTimeLoop(){
  
  menuButton.read();
  if(menuButton.wasReleased()){
    if(datetimeState == STATE_DATE_SET){
      lcd.setCursor(0,1);
      lcd.print("01/01/2015  OK");
    } else if(datetimeState == STATE_TIME_SET){
      lcd.setCursor(0, 1);
      lcd.print("00:00:00 AM OK");
    }
    // TODO: change date/time loop
  }
  int encoderPosState = encoderPosRead();
  if(encoderPosState == ENC_INC){
    datetimeState = STATE_DATE_SET;
    showChangeTimeMenu();
  } else if (encoderPosState == ENC_DEC) {
    datetimeState = STATE_TIME_SET;
    showChangeTimeMenu2();
  }
}

void showChangeTimeMenu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set [Date]|Time");
}
void showChangeTimeMenu2(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Date|[Time]");
}

void mainMenuLoop(){
  int encoderPosState = encoderPosRead();
  if(encoderPosState == ENC_INC){
    if (menuPos == 0){
      menuPos = MENU_COUNT - 1;
    } else {
      menuPos = (menuPos - 1);
    }
    Serial.print("Menu item: ");
    Serial.println(menuPos);
    showMenu();
  } else if (encoderPosState == ENC_DEC) {
    menuPos = (menuPos + 1) % (MENU_COUNT);
    Serial.print("Menu item: ");
    Serial.println(menuPos);
    showMenu();
  }
  
}
  
void loop(){

  switch (menuState){
    case STATE_MAIN_MENU:
      mainMenuLoop();
      checkMainMenuButonPress();
      break;
    case STATE_MANUAL_CONTROL:
      // do stuff
      break;
    case STATE_OPEN_TIME_SET:
      // do stuff
      break;
    case STATE_CLOSE_TIME_SET:
      // Do stuff
      break;
    case STATE_DATETIME_SET:
      changeTimeLoop();
      break;
  } 
  __asm__ volatile("nop");
}
