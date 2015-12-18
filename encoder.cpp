#include "encoder.hpp"
#include <Arduino.h>

Encoder::Encoder(){
  
}

Encoder::Encoder(unsigned int enc_a_pin, unsigned int enc_b_pin, unsigned int enc_btn_pin){
  // setup rotary encoder
  pinMode(enc_a_pin, INPUT);
  pinMode(enc_b_pin, INPUT);
  pinMode(enc_btn_pin, INPUT);

  PIN_ENC_A = enc_a_pin;
  PIN_ENC_B = enc_b_pin;
  PIN_BTN = enc_btn_pin;

  encoderPos = 0;
  lastEncoderPos = 0;
  encoderPinALast = LOW;
  n = LOW;
}

encState Encoder::read(){
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
      
      return ENC_DEC;
    } else if (encoderPos < lastEncoderPos){
      Serial.println("Dec");
      return ENC_INC;
    }
   return ENC_SAME;
}
