/*
 * Project: EthernetTest
 * Description: Ensure Ethernet Port wired correctly
 * Author: Daniel Mills
 * Date: Feb-28-2022
 */

 /*
  * Ethernet Port Wiring
  * 3.3V to Teensy 3.3V
  * GND to Teensy GND
  * MISO to Teensy DI (Pin 12)
  * MOSI to Teensy DO (Pin 11)
  * SCLK to Teensy SCK (Pin 13)
  * SCNn to Teensy Pin 10 (for Chip Select)
  * RSTn to Teensy Pin 9
  * INTn and NC (No Connection) not connected
  */

#include <SPI.h>
#include <Ethernet.h>
#include "mac.h"
#include "wemo.h"
#include <OneButton.h>
#include <Encoder.h>

Encoder myEnc(3, 2);
int encoderOutput;
int encoderLastPosition;
OneButton button1(23, false);
bool ONOFF;
bool buttonState = false, blinker = false;
int wemoPorts = 0;
int i;

EthernetClient client;
bool status;   //user to ensure port openned correctly
byte thisbyte; //used to get IP address

void setup() {
  //initialize ethernet port and uSD module to off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  //Open Serial Communication and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  Serial.printf("Starting Program:\n");

  //Start ethernet connection
  status = Ethernet.begin(mac);
  if (!status) {
    Serial.printf("failed to configure Ethernet using DHCP \n");
    //no point in continueing 
    while(1);
  }
    
  //print your local IP address
  Serial.print("My IP address: ");
  for (thisbyte = 0; thisbyte < 3; thisbyte++) {
    //print value of each byte of the IP address
    Serial.printf("%i.",Ethernet.localIP()[thisbyte]);
    }
  Serial.printf("%i\n",Ethernet.localIP()[thisbyte]);
  switchOFF(0);
  
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleClick1);
  button1.setClickTicks(1000);
  button1.setPressTicks(2000);
  buttonState = false;
  blinker = false;
}

void loop() {
   button1.tick();                          //looks for input from the button
}

void click1() {                                     //when input is received (button is pressed)..
    buttonState = !buttonState;                     //when button is pressed it changes the bool value of the buttonState
    Serial.printf("Single button press %i \n", buttonState);        //can do the same thing as  different data types on the same line 
    if (buttonState) { 
      switchON(wemoPorts);
      Serial.printf("Single button press \n");
    }  
    else{
      switchOFF(wemoPorts);
    }                         
} 

void doubleClick1() {
  blinker = !blinker;
  if (blinker, wemoPorts = i, i++) {
    wemoPorts = i;
  } 
  else {
    wemoPorts = 0;
  }
  Serial.printf("Double button press \n");  
}
