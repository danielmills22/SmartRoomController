/*
 * Project: L04_01_oneButton
 * Description: One Button Assignment 1
 * Author: Daniel Mills
 * Date: Feb-21-2022
 */

#include <OneButton.h>

OneButton button1(23, false);
bool buttonState, blinker;
int iButton = 0;

void setup() {                         //start of void setup
  Serial.begin(9600);                  //Checks serial monitor
  button1.attachClick(click1);           //initialized button1 click1
  button1.attachDoubleClick(doubleClick1);
  button1.setClickTicks(250);
  button1.setPressTicks(2000);
  buttonState = false;
  blinker = false;
}                                      //end of setup

void loop() {                              //start of void loop    
  button1.tick();                          //looks for input from the button
  
  if (buttonState == true) {
     
    if (blinker == true) {
      digitalWrite(iButton, HIGH);
      delay(100);  
      digitalWrite(iButton, LOW);
      delay(100); 
    } 
    else { 
      digitalWrite (iButton, HIGH);
    }
  }
  else{ 
    digitalWrite(iButton, LOW); 
 } 
}

void click1() {                                     //when input is received (button is pressed)..
    buttonState = !buttonState;                     //when button is pressed it changes the bool value of the buttonState
    Serial.printf("Single button press \n");        //can do the same thing as  different data types on the same line                          
} 

void doubleClick1() {
  blinker = !blinker;
  Serial.printf("Double button press \n");  
}



//end of If loop  
