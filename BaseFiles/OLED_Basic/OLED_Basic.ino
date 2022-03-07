/*
 * Project: L10_01_0LEDWRITE
 * Description: LED Write Assignment
 * Author: Daniel Mills
 * Date: Mar-02-2022
 */

#include <SPI.h>                       //Library to use for the SPI modual
#include <Wire.h>                      //Imports the wire library for connecting 
#include <Adafruit_GFX.h>              //Ada graphics library
#include <Adafruit_SSD1306.h>          //Imports the library for the OLED display

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = 4;
const int SCREEN_ADDRESS = 0x3C;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LOGO_HEIGHT = 16;   //screen width
const int LOGO_WIDTH = 16;    //screen height
int selection = 0;
 

void setup() {
  Serial.begin(9600);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.printf("SSD1306 allocation failed");
    for(;;);   //looks for an end to the loop
  }

  display.display();         //Shows the Adafruit splah
  delay(2000);               //delay
  display.clearDisplay();    //Clears the screen
  display.display();         //Only use this command when needed instead of repeating
  delay(2000);
}

void loop() {
  
  //display.setTextSize(1);                                // Draw 2X-scale text (too large for screen)
  display.setTextColor(SSD1306_WHITE);
  display.printf("What would you like to select: \n %i", selection);   //outputs your birthday

  display.display();
  delay(2000);                //delays the clear display for 2 seconds

  display.clearDisplay();      //clears the display for rotation
  display.setRotation(3);     //displays a rotation of the display info stored
}
