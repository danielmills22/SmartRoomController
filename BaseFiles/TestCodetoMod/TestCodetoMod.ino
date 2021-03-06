/*
 * Project: L04_01_oneButton
 * Description: One Button Assignment 1
 * Author: Daniel Mills
 * Date: Feb-21-2022
 */

#include <OneButton.h>  //includes the OneButton library

OneButton button1(20, false);  //OneButton to pin 23 and sets value to false
bool buttonState, blinker;     //Creates a bool var for buttonState and for blinker
int iButton = 0;               //Creates var to hold the button press outcome

void setup() {                         //start of void setup
  Serial.begin(9600);                  //Checks serial monitor
  button1.attachClick(click1);           //initialized button1 click1
  button1.attachDoubleClick(doubleClick1);  //initializes the DoubleClick function
  button1.setClickTicks(250);               //Sets the click rate for viewing outcome to 250
  button1.setPressTicks(2000);           //Looks for a listening time of 2000 for double click
  buttonState = false;                   //sets the button state to false
  blinker = false;                      //sets the blinker state to false
}                                      //end of setup

void loop() {                              //start of void loop    
  button1.tick();                          //looks for input from the button
  if (buttonState == true) {
   click1();
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
