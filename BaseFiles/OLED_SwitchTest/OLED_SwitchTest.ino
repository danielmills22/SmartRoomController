/*
 * Project: L10_01_0LEDWRITE
 * Description: LED Write Assignment
 * Author: Daniel Mills
 * Date: Mar-02-2022
 */

//image settings 128x64px

#include <SPI.h>                       //Library to use for the SPI modual
#include <Wire.h>                      //Imports the wire library for connecting 
#include <Adafruit_GFX.h>              //Ada graphics library
#include <Adafruit_SSD1306.h>          //Imports the library for the OLED display
#include "kittyHeader.h"

//Vars for OLED
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = 4;
const int SCREEN_ADDRESS = 0x3C;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LOGO_HEIGHT = 16;   //screen width
const int LOGO_WIDTH = 16;    //screen height
int selection = 0;
 
//Ints for potent
int potentPin = 14;  //set potent to port 14
int lastPotentValue;  //creates int for last potent value

void setup() {
  Serial.begin(9600);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.printf("SSD1306 allocation failed");
    for(;;);   //looks for an end to the loop
  }

  display.display();         //Shows the Adafruit splah
  delay(100);               //delay
  display.clearDisplay();    //Clears the screen
  display.display();         //Only use this command when needed instead of repeating
  delay(100);

  //Sets the pinMode for the potent values
  pinMode(potentPin, INPUT);   // set pin modes
  Serial.begin(9600);  //listen to Serial monitor
}

void loop() {

 int potentValue = analogRead(potentPin) / 255;   // read potentPin and divide by 255 to give 5 possible readings
  //sp
  if(potentValue != lastPotentValue)  //Start of Switch loop
  {
    // enter switch case
    switch(potentValue)
    {
      case 0:
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("What would you like to do: Case 0, Case 1, Case 2, Case 3, Case 4");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          display.clearDisplay();      //clears the display 
        break;
      case 1:
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 1");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          display.clearDisplay();      //clears the display 
          Serial.println("Switch Case 1");
        break;
     //Start of case 2
      case 2:
        //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 2");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          display.clearDisplay();      //clears the display 
          Serial.println("Switch Case 2");
        break;
      //Start of case 3
      case 3:
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 3");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          display.clearDisplay();      //clears the display 
          Serial.println("Switch Case 3");
        break;
      //Start of Case 4
      case 4:
         //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Switch Case 4");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          display.clearDisplay();      //clears the display 
          Serial.println("Switch Case 4");
        break;
      //Start of default
      default:
          //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(0,0);             // Start at top-left corner
          display.printf("Error");   //Outputs Switch Case

          display.display();
          delay(2000);                //delays the clear display for 2 seconds

          display.clearDisplay();      //clears the display 
          Serial.println("error!");
        break;
    }
    lastPotentValue = potentValue;
  }
}

void drawbitmapCase1(void) {
  int centerV =  (display.height()-64)/2;   //(display.height()-128)/2;
  int centerH =  (display.width()-128)/2;     //(display.width()-64)/2;

  display.clearDisplay();
  display.drawBitmap(centerH, centerV, Kitty, 128, 64, 1);
  display.display();
  delay(2000);
}
