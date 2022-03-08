/*
 * Project: L05_NeoPixels
 * Description: L05 NeoPixels
 * Author: Daniel Mills
 * Date: Feb-22-2022
 */

#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <OneButton.h>  

OneButton button1(22, false);  
bool buttonState, blinker;

const int PIXELPIN = 17;
const int PIXELCOUNT = 16;
int i;


//Timer Var
unsigned long blinkTimerStart;
unsigned long blinkTimereInterval;
unsigned long workTimerStart;
unsigned long workTimereInterval;
unsigned long currentTime;


Adafruit_NeoPixel pixel(PIXELCOUNT,PIXELPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixel.setBrightness(150);
  pixel.begin();
  pixel.show();
  i = 0;
  
  button1.attachClick(click1);
  button1.setClickTicks(250);
  button1.setPressTicks(2000);
  buttonState = false;
  blinker = false;

//Initialize timer for 
  blinkTimerStart = millis();
  blinkTimereInterval = 200;

  workTimerStart = millis();
  workTimereInterval = 2000;
}

void loop() {
 
 currentTime = millis();
 button1.tick();
 if (buttonState == true) {
  if((currentTime-workTimerStart) > workTimereInterval) {
    digitalWrite(PIXELPIN, LOW);
  }
 } 
  else { 
      if((currentTime-blinkTimerStart) > blinkTimereInterval) {
        pixel.fill(blue, i, 16);
        pixel.show();
        pixel.clear();
      }
  }
}
 
 
void click1() {                                     //when input is received (button is pressed)..
    buttonState = !buttonState;                     //when button is pressed it changes the bool value of the buttonState
    Serial.printf("Single button press \n");        //can do the same thing as  different data types on the same line                          
} 
