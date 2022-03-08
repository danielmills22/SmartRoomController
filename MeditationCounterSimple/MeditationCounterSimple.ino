#include <Adafruit_NeoPixel.h>
#include <colors.h>

const int buttonPin = 20;         // the number of the pushbutton pin
const int PIXELPIN = 17;
const int PIXELCOUNT = 16;
int currentTime = millis();
int i;

int buttonState = 0;         // variable for reading the pushbutton status

//Time Var 
unsigned long workTimerStart;
unsigned long workTimereInterval;

Adafruit_NeoPixel pixel(PIXELCOUNT,PIXELPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixel.setBrightness(150);
  pixel.begin();
  pinMode(buttonPin, INPUT);

  workTimerStart = millis();
  workTimereInterval = 10000;
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  Serial.printf("Beginning of Meditation Counter");

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
        pixel.clear();
        pixel.show();
        Serial.printf("Button press was registered"); 
        delay(60000*10);
    } 
    else {
        pixel.fill(blue, i, 16);
        pixel.show();
    }
}

    
