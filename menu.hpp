#ifndef MENU_H_
#define MENU_H_

#include <LiquidCrystal.h>
#include "encoder.hpp"
#include "RTClib.h"
#include <Button.h>
#include "submenu.hpp"

#define MAX_MENUS 12

typedef enum MenuState {
  STATE_IDLE = 0x00,
  STATE_MAIN = 0x01,
  STATE_SUBMENU = 0x02
} menuState;

class Menu{
  
private:
  menuState mState;
  LiquidCrystal * lcd;
  RTC_DS1307 * rtc;
  Encoder * encoder;
  Button * menuButton;
  int menuCount;
  int menuPos;

  char menuItems[MAX_MENUS][16];
  SubMenu * subMenus;
  //callback_void menu_callbacks[12];

  void updateIdleScreen();
  void updateMainScreen();
  void updateRegisteredScreen();
  
  void drawIdleScreen();
  void drawMainScreen();
  void drawRegisteredScreen();

  DateTime rtcTime;
  
public:

  Menu(LiquidCrystal * _lcd, Encoder * _encoder, Button * _menuButton);

  void registerMenu(char * title, SubMenu * subMenu);

  void update();

  void draw();
  
};

#endif

