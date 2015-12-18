#include "menu.hpp"
#include <Arduino.h>

Menu::Menu(LiquidCrystal * _lcd, Encoder * encoder){

  lcd = _lcd;
  mState = STATE_IDLE;
  lcd->clear();
  menuCount = 0;
  
}

// TODO: change to submenu class/object or functions defined in here
void Menu::registerMenu(char * title, callback_void func){
  memcpy(menuItems[menuCount], title, strlen(title));
  menu_callbacks[menuCount] = func;
  menuCount++;
} 
void Menu::readInput(){
  switch(mState){
    case STATE_IDLE:
      updateIdleScreen();
      break;
    case STATE_MAIN:
      updateMainScreen();
      break;
    case STATE_SUBMENU:
      updateRegisteredScreen();
      break;
    default:
      updateIdleScreen();
      break;
  }
}

/* Not needed? */
void Menu::draw(){
  
}

void Menu::updateIdleScreen(){
  encState eState = encoder.read();
  if(eState == ENC_DEC){
    if (menuPos == 0){
      menuPos = menuCount - 1;
    } else {
      menuPos = (menuPos - 1);
    }
    Serial.print("Menu item: ");
    Serial.println(menuPos);
    
    drawMainScreen();
  } else if (eState == ENC_INC) {
    menuPos = (menuPos + 1) % (menuCount);
    Serial.print("Menu item: ");
    Serial.println(menuPos);
    
    drawMainScreen();
  }
}

void Menu::drawIdleScreen(){
  
}

