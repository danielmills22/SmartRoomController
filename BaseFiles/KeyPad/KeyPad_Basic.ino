#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char customKey;
int i;
char savedKeyInput[4];
char passwordArray[4] = {'7','7','7','7'};
bool codeCorrect;


char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  //keypad leads 8,7,6,5
byte colPins[COLS] = {5, 4, 3, 2};  //keypad leads 4,3,2,1 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  i = 0;
  customKey = customKeypad.getKey();
  
  if(customKey){
    Serial.printf("Key Pressed: %c \n", customKey);
    Serial.printf("Key Pressed (Hex Code) 0x%02X \n", customKey);
  }
  //savedKeyInput[i] = customKeypad.getKey();
 
  while (i < 4){
    customKey = customKeypad.getKey();
      if (customKey) {
        savedKeyInput[i] = customKey;
        i++;
        Serial.printf("%c, %i, \n", customKey, i);
      }
  }
  codeCorrect = isMatched(savedKeyInput, passwordArray);
  Serial.printf("%i", codeCorrect);
  if (codeCorrect == true) {
     Serial.printf("This is correct \n"); 
  
  }
  else{
    Serial.printf("Incorrect \n"); 
  }
}
  
//########

bool isMatched (char savedKeyInput[4], char passwordArray[4]) {
  int i;
  for(i=0; i < 4; i++){
    Serial.printf("%c", savedKeyInput[i]);
      if (savedKeyInput[i] != passwordArray[i]) {
        Serial.printf("false \n");
        return false;
      }
   }
   Serial.printf("true");
   return true;
}
