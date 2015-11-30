
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
#include "RTClib.h"

#define PIN_ENC_A 7
#define PIN_ENC_B 8
#define PIN_ENC_BTN 6 
#define PIN_PIEZO 9

#define STATE_MENU_MAIN 0x01
#define STATE_MENU_MANUAL_CONTROL 0x02
#define STATE_MENU_TIME_SET 0x03
#define STATE_MENU_OPEN_TIME_SET 0x05
#define STATE_MENU_CLOSE_TIME_SET 0x0
#define MENU_COUNT 4

int encoderPos = 0;
int lastEncoderPos = 0;
int encoderPinALast = LOW;
int n = LOW;
int menuPos = 0;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", 
  "Wednesday", "Thursday", "Friday", "Saturday"};
  
char menuList[MENU_COUNT][16] = {"Manual control", "Set open time", 
  "Set close time", "Change time" };

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

RTC_DS1307 rtc;

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
  
  // setup piezo
  
  // setup rotary encoder
  pinMode(PIN_ENC_A, INPUT);
  pinMode(PIN_ENC_B, INPUT);  
  
  // setup serial
  Serial.begin(9600);
  
  lcd.print("Door Timer ");
  lcd.setCursor(0, 1);
  lcd.print("by Leo Febey");
  delay(1000);
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
  showMenu();
  
}
inline void readEncoderPos(){
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
  
void loop(){
  readEncoderPos();
  if (lastEncoderPos > encoderPos){
    menuPos = (menuPos + 1) % (MENU_COUNT);
    Serial.print("Menu item: ");
    Serial.println(menuPos);
    showMenu();
  } else if (lastEncoderPos < encoderPos){
    if (menuPos == 0){
      menuPos = MENU_COUNT - 1;
    } else {
      menuPos = (menuPos - 1);
    }
    Serial.print("Menu item: ");
    Serial.println(menuPos);
    showMenu();
  }
  __asm__ volatile("nop");
}
