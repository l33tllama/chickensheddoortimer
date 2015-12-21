#include "menu.hpp"
#include <Arduino.h>

Menu::Menu(LiquidCrystal * _lcd, Encoder * _encoder){
  lcd = _lcd;
  mState = STATE_IDLE;
  encoder = _encoder;
  lcd->clear();
  menuCount = 0;
}

// TODO: change to submenu class/object or functions defined in here
void Menu::registerMenu(char * title, callback_void func){
  memcpy(menuItems[menuCount], title, strlen(title));
  menu_callbacks[menuCount] = func;
  menuCount++;
} 

void Menu::update(){
  switch(mState){
    case STATE_IDLE:
      //Serial.println("Idle screen update");
      updateIdleScreen();
      break;
    case STATE_MAIN:
      //Serial.println("Main screen update");
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
  //Serial.println("Reading encoder..");
  encState eState = encoder->read();
  if(eState == ENC_DEC || eState == ENC_INC){
    Serial.println("Going to main menu");
    lcd->clear();
    lcd->setCursor(0,0);
    mState = STATE_MAIN;
  } 
  //Serial.println("No change, updating idle");
  drawIdleScreen();
}

void Menu::drawIdleScreen(){
  lcd->setCursor(0,0);
  lcd->print("Door controller");
  lcd->setCursor(0,1);
  lcd->print("The time");
  
}

void Menu::updateMainScreen(){
  encState eState = encoder->read();
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

void Menu::drawMainScreen(){
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print(menuPos);
  lcd->print(">");
  
  // Show first menu item
  lcd->print(menuItems[menuPos]);
  lcd->setCursor(0, 1);

  // Show second menu item - loop around if above is at last
  if (menuPos < menuCount - 1){
    lcd->print(menuPos + 1);
    lcd->print(" ");
    lcd->print(menuItems[menuPos+1]);
  } else {
    lcd->print(0);
    lcd->print(" ");
    lcd->print(menuItems[0]);
  }
}

void Menu::updateRegisteredScreen(){

}

void Menu::drawRegisteredScreen(){
  
}

