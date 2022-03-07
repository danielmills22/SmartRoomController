/*
 *  Project:      Smart Room Controller Game
 *  Description:  Secret Cat smart room controller shows funny images and animation for controlling the room. 
 *                Uses keypad/breadboard components to control different objects in the room. 
 *  Authors:      Daniel Mills
 *  Date:         03-Mar-2022
 */


//Header Files
#include <OneButton.h>   //includes the OneButton library

//OneButton Vars
OneButton button1(23, false);   //OneButton to pin 23 and sets value to false
bool buttonState, blinker;      //Creates a bool var for buttonState and for blinker
int iButton = 0;                //Creates var to hold the button press outcome


void setup() {
//##SetUp block for OneButton
  Serial.begin(9600);                  //Checks serial monitor
  button1.attachClick(click1);           //initialized button1 click1
  button1.attachDoubleClick(doubleClick1);
  button1.setClickTicks(250);
  button1.setPressTicks(2000);
  buttonState = false;
  blinker = false;

  
}

void loop() {
//##Loop Block for OneButton
 button1.tick();                          //looks for input from the button
 

//

}

//Creates the void functions for OneButton
void click1() {                                     //when input is received (button is pressed)..
    buttonState = !buttonState;                     //when button is pressed it changes the bool value of the buttonState
    Serial.printf("Single button press \n");        //can do the same thing as  different data types on the same line                          
} 
void doubleClick1() {    //looks for the doubleclick function
  blinker = !blinker;    //if current state is not equal to previous state
  Serial.printf("Double button press \n");  //outputs that a doublepress occured to the screen
}
