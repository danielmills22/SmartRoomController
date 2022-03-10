/*
 *  Project:      Smart Room Controller 
 *  Description:  Uses keypad/breadboard components to control different objects in the room.  
 *             
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
#include "wemo.h"                      //includes the wemo library
#include <Adafruit_NeoPixel.h>         //includes the NeoPixel library
#include <Adafruit_BME280.h>           //Calls library for Adafruit display

//Image Header Files
#include "kittyHeader.h"               //Includes the kitty image        
#include "Cat1.h"
#include "Cat2.h"

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
OneButton button1(20, false);   //OneButton to pin 23 and sets value to false
bool buttonState = 0; 
bool blinker = 0;      //Creates a bool var for buttonState and for blinker
int iButton = 0;                //Creates var to hold the button press outcome

//Creates the SD card object
File dataFile;
//Creates the vars for the SD Card
const int chipSelect = 4;     //Registers the SD card to pin 4
int object;                   //Creates an int var for saving data from the SD Card
bool status;

//Wemo Vars
int wemoPorts = 1;  //starts the wemo ports on zero
byte thisbyte; //used to get IP address
int i;

//NeoPixel Var
const int PIXELPIN = 17;
const int PIXELCOUNT = 16;
int currentNeoPixel;

//Vars for BME - Temp Sensor 
Adafruit_BME280 bme;                                                        //Defines BME sensor data 
int temp;
int humid;
float tempF;   //set temperature variable
float pressPA; //sets pressure var
float humidRH; //sets RH var


Adafruit_NeoPixel pixel(PIXELCOUNT,PIXELPIN, NEO_GRB + NEO_KHZ800);  //declares the NeoPixel Object
 
//Ints for potent
int potentPin = 14;  //set potent to port 14
int lastPotentValue;  //creates int for last potent value
int potentMap;

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

  //  drawbitmap();             //draws the map
  // delay(2000);

  //##Setup Block for the SD Card
  Serial.printf("Initializing SD card...");

  // initialize uSD card module CS to off
  pinMode(chipSelect,OUTPUT); 
  digitalWrite(chipSelect,HIGH);
  
  // see if the card is present and can be initialized:
  status = SD.begin(chipSelect);
  if (!status) {  // if status is false
    Serial.printf("Card failed, or not present\n");
//    while(true);  // pause the code indefinately
  }
  else {
    Serial.printf("card initialized.\n");
  }

  //##Set Up block for Wemo Ports - Print your local IP address
  Serial.print("My IP address: ");                   //Lets user know that the ip is being printed to the screen
  for (thisbyte = 0; thisbyte < 3; thisbyte++) {     //Start of loop for identifing the IP
    //print value of each byte of the IP address
    Serial.printf("%i.",Ethernet.localIP()[thisbyte]);      //Command for printing the Ethernet local IP
    }
  Serial.printf("%i\n",Ethernet.localIP()[thisbyte]);        //Prints the Final ethernet local ip to the monitor
  switchOFF(0);                                              //Turns off the wemo port 0

  //Setup Block for NeoPixels
  pixel.setBrightness(150);    //sets the brightness of the NeoPixels to 150
   pixel.begin();               //Sets the Neopixel and starts listening for commands

  //Sets the pinMode for the potent values
  pinMode(potentPin, INPUT);   // set pin modes
  Serial.begin(9600);  //listen to Serial monitor

  status = bme.begin(0x76); //looks for the temp sensor

  //NeoPixel Setup Block
  pixel.setBrightness(150);  //brightness value for the Neopixels display
  pixel.begin();
  pixel.show();
}



//////////////////////////////
/////////////////////////////
void loop() {
  
  button1.tick();
  Serial.printf("Button State %i \n", buttonState); 
  encoderOutput = myEnc.read();
//  if (encoderOutput != encoderLastPosition) {
//    Serial.println(encoderOutput);
//    encoderLastPosition = encoderOutput;
//      if (encoderOutput > 255) {
//        myEnc.write(255);
//      }
//      if (encoderOutput < 0) {
//        myEnc.write(0);
//      }
//  }
  
 int potentValue = analogRead(potentPin);   // read potentPin and divide by 255 to give 5 possible readings
 potentMap = map(potentValue, 0, 1023, 0, 5);
 //Serial.printf("Potent Map %i \n, Potent Values %i \n", potentMap, potentValue);
 Serial.printf("Potent Map %i \n", potentMap);
  //sp
  if(potentMap != lastPotentValue){  //Start of Switch loop
    buttonState = false;
    blinker = false;
    lastPotentValue = potentMap;
  }
    // enter switch case
    switch(potentMap)
    {
      case 0:
           display.clearDisplay();      //clears the display 
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          //display.setTextColor(SSD1306_WHITE);
          //display.setCursor(0,0);             // Start at top-left corner
          //display.printf("Switch Case 0");   //Outputs Switch Case
          //display.display();
         
         
         if (buttonState) {
            currentNeoPixel = map(encoderOutput,0,96,0,15);
            //pixel.setPixelColor(currentNeoPixel,yellow);
            pixel.fill(blue, i, 16);
            pixel.show();
            Serial.printf("Case 0 Button Check %i \n", buttonState); 
         }
         else {
            Serial.printf("Case 0 Button Check Else Stat %i \n", buttonState);
            pixel.clear(); 
            pixel.show();
         }
        
          if(blinker){
            Cat1display();
          }
          else {
           //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
            //display.clearDisplay();      //clears the display 
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0,0);             // Start at top-left corner
            display.printf("Squire: You are in Case0<, Case1, Case2, Case3, Case4 ");   //Outputs Switch Case
            display.display();
            pixel.clear();
            pixel.show();
          }
        
        //display.clearDisplay();      //clears the display 
        break;
      case 1:
           display.clearDisplay();      //clears the display 
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          //display.setTextColor(SSD1306_WHITE);
          //display.setCursor(0,0);             // Start at top-left corner
          //display.printf("Switch Case 1");   //Outputs Switch Case
          //display.display();
          encoderOutput = myEnc.read();
          if (encoderOutput != encoderLastPosition) {
                Serial.println(encoderOutput);
                buttonState = false;
                encoderLastPosition = encoderOutput;
                  if (encoderOutput > 4) {
                    myEnc.write(4);
                  }
                  if (encoderOutput < 0) {
                    myEnc.write(0);
                  }
           }
          if (buttonState) {
             setHue(encoderOutput,true,HueBlue,255,255);  //lights the bulb the color blue
              Serial.printf("Case 1 Button Check %i \n", buttonState);    
          }  
          else {
            Serial.printf("Case 1 Button Check Else Stat %i \n", buttonState);
            setHue(encoderOutput,false,HueBlue,0,0);  //lights the bulb the color blue
          }

          if(blinker){
            catDisplay2();
          }
          else {
            //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
            //display.clearDisplay();      //clears the display 
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0,0);             // Start at top-left corner
            display.printf("Squire: You are in Case0, Case1<, Case2, Case3, Case4 ");   //Outputs Switch Case
            display.display();
            pixel.clear();
            pixel.show();
          }

          Serial.printf("Encoder Value %i \n", encoderOutput);

        break;
     //Start of case 2
      case 2:
        //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.clearDisplay();      //clears the display 
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 2");   //Outputs Switch Case

          display.display();
          //delay(2000);                //delays the clear display for 2 seconds

          if (buttonState) {
            currentNeoPixel = map(encoderOutput,0,96,0,15);
            //pixel.setPixelColor(currentNeoPixel,yellow);
            pixel.fill(blue, i, 16);
            pixel.show();
            Serial.printf("Case 2 Button Check %i \n", buttonState); 
          }
          else {
            Serial.printf("Case 2 Button Check Else Stat %i \n", buttonState);
            pixel.clear(); 
            pixel.show();
          }
          
          if(blinker){
            pixel.clear();
            pixel.fill(green, i, 16);
            pixel.show();
          }
          else {
            //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
            //display.clearDisplay();      //clears the display 
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0,0);             // Start at top-left corner
            display.printf("Squire: You are in Case0, Case1, Case2<, Case3, Case4 ");   //Outputs Switch Case
            display.display();
            pixel.clear();
            pixel.show();
          }
          
         
          Serial.println("Switch Case 2");
        break;
      //Start of case 3
      case 3:
          display.clearDisplay();      //clears the display 
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          //display.clearDisplay();      //clears the display 
          //display.setTextColor(SSD1306_WHITE);
          //display.setCursor(0,0);             // Start at top-left corner
          //display.printf("Switch Case 3");   //Outputs Switch Case
          //display.display();
          //delay(2000);                //delays the clear display for 2 seconds

           if (buttonState) {
            //display.clearDisplay();   //clear display
            tempF = (bme.readTemperature()*1.8)+32;         //converting F to C
            pressPA = bme.readPressure()*0.00030;           //converting pascal pressure to inches of mercury
            humidRH = bme.readHumidity();                   //read humidity

            Serial.printf("temp %.02f \n", tempF);          //Serial.print temp 
            Serial.printf("pressPA %.02f \n", pressPA);     //serial.print pressure
            Serial.printf("humid %.02f \n", humidRH);       //Serial print humidity

            display.setTextSize(1);                                    // Sets 1:1 pixel scale
            display.setTextColor(SSD1306_WHITE);       // Draws black text on white (inverse text)
            display.setCursor(0,0);                                    // Start at top-left corner
            display.printf("Temp: %.02f%cF  \n Press: %.02f \n RH: %.02f%cF \n", tempF, 248, pressPA, humidRH, 248);   //outputs sensor values
            display.display();
            //delay(2000);                //delays the clear display for 2 seconds
            //display.clearDisplay();   //clear display
  
             //#Calls the write to datafile and read from file
              //writeToSD(tempF, pressPA, humidRH);    //Calls the writeToSD function and writes these vars to the file
                 /*if(millis() - lastTime > 5000) {
                  *   writeToSD(tempF, pressPA, humidRH);
                  *   lastTime = millis();
                  * }  
                  * 
                  */
              //delay(2000);  //delays for 2 seconds 
              //readFromSD(); 
              Serial.printf("Case 3 Button Check %i \n", buttonState); 
              
              if (tempF > 50){
               switchON(1);
               Serial.printf("Wemo is on \n");
              }  
              else{
                switchOFF(wemoPorts);
              }                         
              }
               else {
                  Serial.printf("Case 3 Button Check Else Stat %i \n", buttonState);
              }
           

          Serial.println("Switch Case 3");
        break;
      //Start of Case 4
      case 4:
         //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.clearDisplay();      //clears the display 
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 4");   //Outputs Switch Case

          display.display();
          //delay(2000);                //delays the clear display for 2 seconds

           if(blinker){
            pixel.clear();
            pixel.fill(white, i, 16);
            pixel.show();
           }
           else {
            //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
            //display.clearDisplay();      //clears the display 
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0,0);             // Start at top-left corner
            display.printf("Squire: You are in Case0, Case1, Case2, Case3, Case4< ");   //Outputs Switch Case
            display.display();
            pixel.clear();
            pixel.show();
            pixel.clear();
            pixel.show();
           }


          Serial.println("Switch Case 4");
        break;
      //Start of Case 5
      case 5:
         //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.clearDisplay();      //clears the display 
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 4");   //Outputs Switch Case

          display.display();
          //delay(2000);                //delays the clear display for 2 seconds

           if(blinker){
            pixel.clear();
            pixel.fill(yellow, i, 16);
            pixel.show();
           }
           else {
            //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
  
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0,0);             // Start at top-left corner
            display.printf("Squire: You are in Case0, Case1, Case2, Case3, Case4, Case5< ");   //Outputs Switch Case
            display.display();
            pixel.clear();
            pixel.show();
           }


          
          Serial.println("Switch Case 4");
        break;
      //Start of default
      default:
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.clearDisplay();      //clears the display 
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Error");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          
          Serial.println("error!");
        break;
    //}
    lastPotentValue = potentMap;
  }
}

/////////////////////////////////////////////
////////////////////////////////////////////
//void drawbitmapCase1(void) {
//  int centerV =  (display.height()-64)/2;   //(display.height()-128)/2;
//  int centerH =  (display.width()-128)/2;     //(display.width()-64)/2;
//
//  display.clearDisplay();
//  display.drawBitmap(centerH, centerV, Kitty, 128, 64, 1);
//  display.display();
//  delay(2000);
//}

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
//void drawbitmap(void) {
//  int centerV =  (display.height()-64)/2;   //(display.height()-128)/2;
//  int centerH =  (display.width()-128)/2;     //(display.width()-64)/2;
//
//  display.clearDisplay();
//  display.drawBitmap(centerH, centerV, Kitty, 128, 64, 1);
//  display.display();
//  delay(2000);
//}
 
//#Void Function for Starting of the printIP function
void printIP() {
  Serial.printf("My IP address: ");   //outputs this to the screen if the connection was successful
  for (byte thisByte = 0; thisByte < 3; thisByte++) {  
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);  //prints IP to serial monitor
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);  //prints IP to serial monitor
}

void writeToSD(float tempF, float pressPA, float humidRH ) {     //looks to write these variable to the SD card
  dataFile = SD.open("datalog.csv", FILE_WRITE);     //file the information was written to
  // if the file is available, write to it:
  if (dataFile) {         
    dataFile.printf("Temp: %.02f%cF  \n Press: %.02f \n RH: %.02f%c \n", tempF, 248, pressPA, humidRH, 248);    //writes these var to the SD card
    dataFile.close();   //closes the SD when done
    Serial.printf("Temp: %.02f%cF  \n Press: %.02f \n RH: %.02f%c \n", tempF, 248, pressPA, humidRH, 248);  //prints these values to the screen
  }  
  else {
    Serial.printf("error opening datalog.csv \n");    //returns this message if information couldn't be read from the card
  }
  return;  //exits function
} 


//##Start of the read from SD card function
void readFromSD(){
  // re-open the file for reading:
  dataFile = SD.open("datalog.csv"); //opens the datalog.csv file
  if (dataFile) {
    Serial.printf("datalog.csv: \n");  //if it was able to open that file it will output those values to the serial monitor
  // read from the file until there's nothing else in it:
  while (dataFile.available()) {
      Serial.write(dataFile.read());   //will continue to read from card as long as data is present
  } 
  dataFile.close();   //closes the data file when it is finished reading 
  } 
  else { 
    Serial.printf("error opening datalog.csv \n");   //will output this message if there was a problem reading from the file.
  }
  return;  //exits function
}




//////////////////////////////////////////////
/////////////////////////////////////////////
//Images Block
void Cat1display(void) {
  int centerV =  (display.height()-64)/2;   //(display.height()-128)/2;
  int centerH =  (display.width()-128)/2;     //(display.width()-64)/2;

  display.clearDisplay();
  display.drawBitmap(centerH, centerV, Cat1, 128, 64, 1);
  display.display();
  delay(2000);
}

void catDisplay2(void) {
  int centerV =  (display.height()-64)/2;   //(display.height()-128)/2;
  int centerH =  (display.width()-128)/2;     //(display.width()-64)/2;

  display.clearDisplay();
  display.drawBitmap(centerH, centerV, Cat2, 128, 64, 1);
  display.display();
  delay(2000);
}
