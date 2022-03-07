/*
 *  Project:      Smart Room Controller Game
 *  Description:  Secret Cat smart room controller shows funny images and animation for controlling the room. 
 *                Uses keypad/breadboard components to control different objects in the room. 
 *  Authors:      Daniel Mills
 *  Date:         03-Mar-2022
 */

#include button
#include keypad
#include OLED
#include WEMO
#include Ethernet
#include SD Card
#include HUE
#include SPI 
#include Colors

//Var Block, create var block
create button vars
create keypad vars
create OLED vars
create WEMO vars
create Ethernet vars
create SD vars
create HUE vars
create SPI vars 

//Call Definitions
button def
keypad def
OLED def
WEMO def
Ethernet def
SD def
HUE def
SPI def

void setup() {
  set pins for button 
  set pins for keypad 
  set pins for OLED 
  Call WEMO 
  Call Ethernet 
  Call SD 
  Call HUE 
  Call SPI 
}

void loop() {
  OLED display.printf("What would you like to control")
  print options ("HUE, WEMO, NeoPixels")

  If hue is true
    ask which light to control
    Use encoder to to increase/decrease light bightness
    use zero to go back to menu
    write selection to SD card
  If Wemo is true
    ask which one to turn on/off
    use button to turn on/off wemo
    use zero to go back to menu
    write selection to sd card file
  If fan is true 
    Read temperature
    if temp is greater than 50F, print "Would you like to turn on fan"
    write selection to sd card

  If D is selected 
    ask would you like to turn all off
    if button is pushed, kill all
    
  If A is selected
    Ask if you would you like to turn all on
    if button is pushed, kill all
}
