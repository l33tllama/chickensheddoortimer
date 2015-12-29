#include "submenu.hpp"

SubMenu::SubMenu(LiquidCrystal * _lcd, Encoder * _encoder, Button * _menuButton){
  lcd = _lcd;
  encoder = _encoder;
  menuButton = _menuButton;
  title_set = false;
  menuItems[0] = ITEM_MENUBAR;
  numLines = 1;
  blinkCount = 0;
}

void SubMenu::addText(const char * text){
  menuItems[numLines] = ITEM_TEXT;
  memcpy(menuLines[numLines], text, 16);  
  numLines++;
}

void SubMenu::addTitleText(const char * text){
  memcpy(title_text, text, 15);
  title_set = true;
}

void SubMenu::addTimeSet(struct tm * intitial_time){
  menuItems[numLines] = ITEM_TIMESET;

  numLines++;
}

void SubMenu::addDateSet(struct tm * initial_date){
  menuItems[numLines] = ITEM_DATESET;

  numLines++; 
}

void SubMenu::addOnOffSwitch(callback_void func){
  menuItems[numLines] = ITEM_ONOFFSWITCH;

  numLines++;
}
/*
char * renderBlink(char * in, char pos ){
  
}

char * renderMenuBar(){
  
  char * out = "<";
} */

char * SubMenu::renderItem(menuTypes mType){
   switch(mType){
      case ITEM_MENUBAR:
        
      break;
      case ITEM_TEXT:
      break;
      case ITEM_TIMESET:
      break;
      case ITEM_DATESET:
      break;
      case ITEM_ONOFFSWITCH:
      break;
      default:
      break;
    }
}

void SubMenu::renderMenu(int deltaMove){
  currentLine = currentLine + deltaMove;
  
  char * topLine;
  char * bottomLine;
  
  for(int i = 0; i < numLines; i++){
    if(i == currentLine){
      menuTypes mType = menuItems[i];
      topLine = renderItem(mType);
      if (i + 1 < numLines){
        menuTypes mType = menuItems[i+1];
        bottomLine = renderItem(mType);
      } else {
        menuTypes mType = menuItems[0];
        bottomLine = renderItem(mType);
      }
    }  
  }
  lcd->setCursor(0, 0);
  lcd->print(topLine);
  if(numLines > 1){
   lcd->setCursor(0, 1);
   lcd->print(bottomLine);
  }
  
}

void SubMenu::update(){

  encState eState = encoder->read();
  
  if(eState == ENC_INC) {
    // move up/down menu
    renderMenu(1);
  } else if (eState == ENC_DEC){
    renderMenu(-1);
  } else {
    if(blinkCount > blinkTime){
     blinkStatus = !blinkStatus;
     renderMenu(0);
     blinkCount = 0;
    }
  }
  
  blinkCount++;
 
}

