#ifndef MENU_H_
#define MENU_H_

#include <LiquidCrystal.h>
#include "encoder.hpp"
#include "RTClib.h"
typedef enum MenuState {
  STATE_IDLE = 0x00,
  STATE_MAIN = 0x01,
  STATE_SUBMENU = 0x02
} menuState;

typedef void (*callback_void)(void);

class Menu{
  
private:
  menuState mState;
  LiquidCrystal * lcd;
  RTC_DS1307 * rtc;
  Encoder * encoder;
  int menuCount;
  int menuPos;

  char menuItems[12][16];
  callback_void menu_callbacks[12];

  void updateIdleScreen();
  void updateMainScreen();
  void updateRegisteredScreen();
  
  void drawIdleScreen();
  void drawMainScreen();
  void drawRegisteredScreen();

  DateTime rtcTime;
  
public:

  Menu(LiquidCrystal * _lcd, Encoder * encoder);

  void registerMenu(char * title, callback_void func);

  void update();

  void draw();
  
};

#endif

