#ifndef SUBMENU_H_
#define SUBMENU_H_
#include <LiquidCrystal.h>
#include "encoder.hpp"
#include "RTClib.h"
#include <Button.h>
#include "time.h"

typedef void (*callback_void)(void);

typedef enum MenuTypes{
  ITEM_MENUBAR = 0x00,
  ITEM_TEXT = 0x01, 
  ITEM_DATESET = 0x02, 
  ITEM_TIMESET = 0x03, 
  ITEM_ONOFFSWITCH = 0x04
} menuTypes;

class SubMenu{
private:
  char * menuLines[16];
  int numLines;
  int currentLine;
  LiquidCrystal * lcd;
  RTC_DS1307 * rtc;
  Encoder * encoder;
  Button * menuButton;
  struct tm ** times;
  struct tm ** dates;
  menuTypes * menuItems;
  char * title_text;
  bool title_set;
  const unsigned int blinkTime = 100000;
  unsigned int blinkCount;
  byte blinkStatus;
  void renderMenu(int deltaMove);
  char * renderItem(menuTypes mType);
  

public:
  SubMenu(LiquidCrystal * _lcd, Encoder * _encoder, Button * _menuButton);
  void addText(const char * text);
  void addTitleText(const char * text);
  void addTimeSet(struct tm * initial_time);
  void addDateSet(struct tm * initial_date);
  void addOnOffSwitch(callback_void func);
  void drawTimeSet();
  void drawDateSet();
  void drawOnOffswitch();
  void update();
  
};

#endif

