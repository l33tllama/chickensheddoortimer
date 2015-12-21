#include "encoder.hpp"
#include <Arduino.h>

Encoder::Encoder(){
  
}

Encoder::Encoder(unsigned int enc_a_pin, unsigned int enc_b_pin, unsigned int enc_btn_pin){
  // setup rotary encoder
  pinMode(enc_a_pin, INPUT);
  pinMode(enc_b_pin, INPUT);
  pinMode(enc_btn_pin, INPUT);

  _enc_a_pin = enc_a_pin;
  _enc_b_pin = enc_b_pin;
  _enc_btn_pin = enc_btn_pin;

  encoderPos = 0;
  lastEncoderPos = 0;
  encoderPinALast = LOW;
  n = LOW;
}

encState Encoder::read(){
  lastEncoderPos = encoderPos;
  //Serial.println("Reading encoder..");
  n = digitalRead(_enc_a_pin);
  if ((encoderPinALast == LOW) && (n == HIGH)) {
    if (digitalRead(_enc_b_pin) == LOW) {
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
