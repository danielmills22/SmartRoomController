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


const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = 4;
const int SCREEN_ADDRESS = 0x3C;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LOGO_HEIGHT = 16;   //screen width
const int LOGO_WIDTH = 16;    //screen height
int selection = 0;

#include <OneButton.h>
OneButton button1(23, false);
bool buttonState, blinker;

void setup() {
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.printf("SSD1306 allocation failed");
    for(;;);   //looks for an end to the loop
  }

  delay(2000);
}

void loop() {}


void click1() {                                     //when input is received (button is pressed)..
    buttonState = !buttonState;                     //when button is pressed it changes the bool value of the buttonState
    Serial.printf("Single button press \n");        //can do the same thing as  different data types on the same line                          
} 

void doubleClick1() {
  blinker = !blinker;
  Serial.printf("Double button press \n");  
}
