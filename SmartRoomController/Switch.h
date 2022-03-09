// pin definitions
int potPin = A0;

// declare global variables
int lastPotValue;

void setup() {
  // set pin modes
  pinMode(potPin, INPUT);
  //initialise Serial port
  Serial.begin(9600);
}

void loop() {
  // read potPin and divide by 255 to give 5 possible readings
  int potValue = analogRead(potPin) / 255;
  
  // if something has changed since last value
  if(potValue != lastPotValue)
  {
    // enter switch case
    switch(potValue)
    {
      case 0:
        fill.pixel
        Serial.println("Very Low");
        break;
      case 1:
        control wemo
        Serial.println("Low");
        break;
      case 2:
        Save to your name to the SD card
        save temp/press/rh to the sd card 
        Serial.println("Moderate");
        break;
      case 3:
        turn on the fan
        Serial.println("High");
        break;
      case 4:
        
        Serial.println("Extreme");
        break;
      default:
        Ask what do you want to do?
        Serial.println("error!");
        break;
    }
    lastPotValue = potValue;
  }
}
