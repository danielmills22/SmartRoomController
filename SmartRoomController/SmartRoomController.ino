/*
 *  Project:      Smart Room Controller Game
 *  Description:  Secret Cat smart room controller shows funny images and animation for controlling the room. 
 *                Uses keypad/breadboard components to control different objects in the room. 
 *  Authors:      Daniel Mills
 *  Date:         03-Mar-2022
 */


//Header Files
#include <SPI.h>                       //Library to use for the SPI modual
#include <Wire.h>                      //Imports the wire library for connecting 
#include <Adafruit_GFX.h>              //Ada graphics library
#include <Adafruit_SSD1306.h>          //Imports the library for the OLED display
#include <OneButton.h>                 //includes the OneButton library
#include <SPI.h>                       //includes the header for the serial bus
#include <Ethernet.h>                  //includes the Ethernet header for internet
#include <mac.h>                       //Includes the mac header 
#include <hue.h>                       //Includes the hue header for adjusting the hue
#include <colors.h>                    //includes the colors library
#include <Encoder.h>                   //includes the library for the encoder
#include <SD.h>                        //includes the SD card library

//Image Header Files
#include "kittyHeader.h"               //Includes the kitty image        

//Var for OLED Screen
const int SCREEN_WIDTH = 128;         //sets the screen width for the OLED
const int SCREEN_HEIGHT = 64;         //sets the screen height
const int OLED_RESET = 4;             
const int SCREEN_ADDRESS = 0x3C;      //Registeres the screen address to the SPI
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //defines the Adafruit screen

//Declaring vars for encoder
Encoder myEnc(3, 2);       //pins the encoder is connected to
int encoderOutput;         //var to hold encoder output
int encoderLastPosition;   //var to hold last encoder position
bool ONOFF;               //bool to turn on and off the hue

//Splash Screen Image Size for reference
//const int LOGO_HEIGHT = 16;   //screen width
//const int LOGO_WIDTH = 16;    //screen height
//int selection = 0;

//OneButton Vars
OneButton button1(23, false);   //OneButton to pin 23 and sets value to false
bool buttonState, blinker;      //Creates a bool var for buttonState and for blinker
int iButton = 0;                //Creates var to hold the button press outcome

//Creates the SD card object
File dataFile;
//Creates the vars for the SD Card
const int chipSelect = 4;
int object;
bool status;


void setup() {
//##SetUp block for OneButton
  Serial.begin(9600);                  //Checks serial monitor
  button1.attachClick(click1);           //initialized button1 click1
  button1.attachDoubleClick(doubleClick1);
  button1.setClickTicks(250);
  button1.setPressTicks(2000);
  buttonState = false;
  blinker = false;

//##Ethernet Start Up Block
  Ethernet.begin(mac);  //starts Ethernet
  delay(200);          //ensure Serial Monitor is up and running           
  printIP();       
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());


//##Setup Information for the OLED
   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.printf("SSD1306 allocation failed");
    for(;;);   //looks for an end to the loop
  }

  display.display();         //Shows the Adafruit splah
  delay(100);               //delay
  display.clearDisplay();    //Clears the screen
  display.display();         //Only use this command when needed instead of repeating
  delay(100);

  drawbitmap();             //draws the map
  delay(2000);

//##Setup Block for the SD Card
Serial.printf("Initializing SD card...");

  // initialize uSD card module CS to off
  pinMode(chipSelect,OUTPUT); 
  digitalWrite(chipSelect,HIGH);
  
  // see if the card is present and can be initialized:
  status = SD.begin(chipSelect);
  if (!status) {  // if status is false
    Serial.printf("Card failed, or not present\n");
    while(true);  // pause the code indefinately
  }
  else {
    Serial.printf("card initialized.\n");
  }

}

void loop() {
//##Loop Block for OneButton
 button1.tick();                          //looks for input from the button
 
//##Encoder Block for controlling HUE Lights
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

//Loop Settings for Writing Data to Card
Serial.printf("Daniel turned on \n");  //output to let me know that the card started
  delay(2000);   //pausese for 2 seconds
  object = 0;    //sets start of info to write to card to be 0

  writeToSD(object);  //writes object data(currently 0) to the SD Card
  delay(2000);        //Pauses for 2 seconds after the information is written to card
  readFromSD();       //Reads the object information from the SD Card
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

//Void Function for showing Images
void drawbitmap(void) {
  int centerV =  (display.height()-64)/2;   //(display.height()-128)/2;
  int centerH =  (display.width()-128)/2;     //(display.width()-64)/2;

  display.clearDisplay();
  display.drawBitmap(centerH, centerV, Kitty, 128, 64, 1);
  display.display();
  delay(2000);
}
 
//#Void Function for Starting of the printIP function
void printIP() {
  Serial.printf("My IP address: ");   //outputs this to the screen if the connection was successful
  for (byte thisByte = 0; thisByte < 3; thisByte++) {  
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);  //prints IP to serial monitor
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);  //prints IP to serial monitor
}

//#Void function for writing to SD Card
void writeToSD(int object) {                             //Start of function used to write info to card
  dataFile = SD.open("datalog.csv", FILE_WRITE);         //Opens the datalog.csv for writing to file
  // if the file is available, write to it:
  if (dataFile) {                                       //asks if data file is available 
    dataFile.printf("%i, \n", object);                  //Writes the data to the card 
    dataFile.close();                                   //Closes the data file after the information is written to it
    Serial.printf("%i, %i \n", object);                 //prints the information stored in the data file to the serial monitor                
  }  
  else {                                               //Looks for if there were problems reading from the file
    Serial.printf("error opening datalog.csv \n");     //Prints the error informaion to the serial monitor
  }
  return;                                              //Ends the function
} 

//#Void Function to read from SD Card
void readFromSD(){                                    //start of loop needed to read from the SD card
    // re-open the file for reading:
  dataFile = SD.open("datalog.csv");                   //Opens the datalog.csv file and saves that info into the datafile var
  if (dataFile) {                                      //if this file is availbale
    Serial.printf("datalog.csv: \n");                  //Prints that information to the Serial monitor   
    // read from the file until there's nothing else in it:
    while (dataFile.available()) {                     //While that datafile is avaialble 
      Serial.write(dataFile.read());                   //continue to read from the file as long as info ia aviable 
    }
    dataFile.close();                                  //Closes the datafile after the information is finished being read
  } 
  else {                                               //What to look for if there was an error opening the file
    Serial.printf("error opening datalog.csv \n");     //Prints error to the monitor
  }
  return;                                              //Ends the loop
}
