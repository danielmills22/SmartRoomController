/*
 *  Project:      Hue Header
 *  Description:  Example Codefor Hue Lights in IoT Classroom using Hue IoT LIbrary
 *  Authors:      Daniel Mills
 *  Date:         03-Mar-2022
 */

//Header files that are being called
#include <SPI.h>           //includes the header for the serial bus
#include <Ethernet.h>      //includes the Ethernet header for internet
#include <mac.h>           //Includes the mac header 
#include <hue.h>           //Includes the hue header for adjusting the hue
#include <colors.h>        //includes the colors library
#include <Encoder.h>       //includes the library for the encoder

//Declaring vars for encoder
Encoder myEnc(3, 2);       //pins the encoder is connected to
int encoderOutput;         //var to hold encoder output
int encoderLastPosition;   //var to hold last encoder position
//const int SWITCHBUTTON = 23;  //pin the Switch button is connected to
//const int GREENBUTTON = 22;
//const int REDBUTTON = 21;
bool ONOFF;               //bool to turn on and off the hue


//##Start of the Void Setup fuction
void setup()
{
  Serial.begin(9600);      //starts the serial monitor

  // ensure all SPI devices start off
  pinMode(10, OUTPUT);    //sets output to pin 10
  digitalWrite(10, HIGH); //writes(turns off this port of the SPI
  pinMode(4, OUTPUT);     //set pin 4 to output
  digitalWrite(4, HIGH);

  Ethernet.begin(mac);  //starts Ethernet
  delay(200);          //ensure Serial Monitor is up and running           
  printIP();       
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());
}

void loop() {
  /* setHue function needs 5 parameters
   *  int bulb - this is the bulb number
   *  bool activated - true for bulb on, false for off
   *  int color - Hue color from hue.h
   *  int - brightness - from 0 to 255
   *  int - saturation - from 0 to 255
   */

   //##Start of the encoder selection function block
  encoderOutput = myEnc.read();                //reads the encoder output
  if (encoderOutput != encoderLastPosition) {  //compares the current encoder position to the previous position
    Serial.println(encoderOutput);             //prints the encoder position to the serial monitor
    encoderLastPosition = encoderOutput;       //sets the current encoder position to the last encoder position var 
      if (encoderOutput > 5) {                 //< fuct restricts the encoder to a max value of 5(for the 5 lightbulbs)
        myEnc.write(96);                       //writes encoder to position 5 if input value is greater than 5
      }
      if (encoderOutput < 1) {                 //<fuct that makes sure there are no negative values (no lights selected
        myEnc.write(1);                        //writes the value back to the first lightbulb
      }
  }
  setHue(3,true,HueBlue,encoderLastPosition,255);   //(5)is the lightbulb #, value used to set hue
  delay(300);   //pause 
}

//#Start of the printIP function
void printIP() {
  Serial.printf("My IP address: ");   //outputs this to the screen if the connection was successful
  for (byte thisByte = 0; thisByte < 3; thisByte++) {  
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);  //prints IP to serial monitor
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);  //prints IP to serial monitor
}
