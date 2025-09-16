#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Wire.h>
// put function declarations here:
//int myFunction(int, int);
bool handleInput();


/* 0 - idle
 1 - bomb counting
 2 - successful defuse
 3 - bomb exploded
*/

int state = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS]={
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'R', '0', 'E'}
};
byte rowPins[ROWS]={9,8,7,6};
byte colPins[COLS]={10,11,12};
Keypad keypad(makeKeymap(keys),rowPins, colPins, ROWS, COLS);


  int red = 2;
  int yellow=3;
  int green=4;
  int blue = 13;
  int pipi = 5;

  int blink1 = 2000;
  int blink2=1000;
  int blink3=500;
  int blink4=250;

void setup() {
  Serial.begin(115200);
  lcd.backlight();
  lcd.init();

  // put your setup code here, to run once:
//  int result = myFunction(2, 3);



  pinMode(red, 1);
  pinMode(yellow,1);
  pinMode(green,1);
  pinMode(blue,1);
  pinMode(pipi,1); 
}


int post=0;
String code ="";
String hiddenString = "";
String newstring = "";
int cursor = 0;
int cursor2 =0;
long lastblink=0;
boolean ledState = false;
bool rightCode=false;

void loop() {
  
  Serial.println("test");

  lcd.setCursor(0,0);
// lcd.print("Hello");

//  char a =   keypad.getKey();
//  if(a){
//   lcd.clear();
//   lcd.print(a);
//  }
   delay(100);

  // put your main code here, to run repeatedly:

  switch(state){
    case 0 : {

      digitalWrite(blue,1); 
       char inputChar  =  keypad.getKey();
       if (inputChar) {
        if(inputChar=='R'){
          code = code.substring(0,cursor-1);
          cursor--;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(code);

        }
       else  if(inputChar=='E'){
          state = 1;
          lcd.clear();
          for (int i=0;i<code.length();i++){
            lcd.setCursor(i,0);
            lcd.print('*');
            hiddenString+='*';
          }
        }
        else{
          code+=inputChar;
          lcd.setCursor(cursor,0);
          lcd.print(inputChar);
          cursor++;
        }
       }
       break;
    }
    case 1:{
      
      digitalWrite(blue,0);
      int i=0;
      while(i<10){
        unsigned long now = millis();
           if (handleInput()) break;
        if (now - lastblink >= blink1) {
        lastblink = now;
   
        digitalWrite(yellow, 1);
        analogWrite(pipi, 1023);
        delay(100);
        digitalWrite(yellow,0);
         analogWrite(pipi, 0);
        i++;
    }
      }
      i=0;
      while(i<20){
        if(rightCode)break;
        unsigned long now = millis();
          if (handleInput()) break;
        if (now - lastblink >= blink2) {
        lastblink = now;
        digitalWrite(yellow, 1);
        analogWrite(pipi, 1023);
        delay(100);
        digitalWrite(yellow,0);
         analogWrite(pipi, 0);
        i++;
      }
      }
      i=0;
      while(i<20){
       if(rightCode)break;

        unsigned long now = millis();
         if (handleInput()) break;
        if (now - lastblink >= blink3) {
        lastblink = now;
        digitalWrite(yellow, 1);
        analogWrite(pipi, 1023);
        delay(100);
        digitalWrite(yellow,0);
         analogWrite(pipi, 0);
        i++;
      }
      }
      i=0;
      while(i<40){
        if(rightCode)break;

        unsigned long now = millis();
       if (handleInput()) break;
        if (now - lastblink >= blink4) {
        lastblink = now;
        digitalWrite(yellow, 1);
        digitalWrite(red, 1);
        analogWrite(pipi, 1023);
        delay(100);
        digitalWrite(yellow,0);
        digitalWrite(red, 0);
        analogWrite(pipi, 0);
        i++;
      }
      
      }
      if(rightCode){
        state=2;
        rightCode=false;
        break;
      }
      state=4;
      break;
      
        }

      case 2: {
        digitalWrite(green, 1);
        lcd.print("Bomb has been");
        lcd.setCursor(0,1);
        lcd.print("defused");
        analogWrite(pipi, 200);
        delay(50);
         analogWrite(pipi, 0);
        delay(50);
        analogWrite(pipi, 200);
        delay(50);
         analogWrite(pipi, 0);
        delay(50);
        analogWrite(pipi, 200);
        delay(50);
         analogWrite(pipi, 0);
        delay(50);

        analogWrite(pipi, 300);
        delay(50);
         analogWrite(pipi, 0);
        delay(50);
        analogWrite(pipi, 400);
        delay(50);
         analogWrite(pipi, 0);
        delay(50);
        state=3;
      }
      case 3:{
        char inputChar  =  keypad.getKey();
       if (inputChar) {
        digitalWrite(green,0);
        lcd.clear();
        lcd.setCursor(0,0);
        cursor2=0;
        code="";
        hiddenString="";
        newstring="";
        
        cursor=0;
        state=0;
        break;

       }
       break;

      }
      case 4: {
        digitalWrite(red, 1);
        analogWrite(pipi,1023);
          lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Too late...");
        delay(2000);
        analogWrite(pipi, 0);
        state =5;
        
      
      }
      case 5: {
         char inputChar  =  keypad.getKey();
       if (inputChar) {
        digitalWrite(red,0);
        lcd.clear();
        lcd.setCursor(0,0);
        cursor2=0;
        cursor=0;
        code="";
        hiddenString="";
        newstring="";
        state=0;
        break;
       }
      }

      }
    }
 

//   }
// }









bool  handleInput(){
   char inputChar  =  keypad.getKey();
       if (inputChar) {
        if(inputChar=='R'){
          newstring = newstring.substring(0,cursor2-1);
          cursor2--;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(hiddenString);
          lcd.setCursor(0,1);
          lcd.print(newstring);
          

        }
       else  if(inputChar=='E'){
        if (newstring.equals(code)){
          state =2;
          lcd.clear();
          rightCode=true;
          return true;;
          
        }
        else{
          lcd.clear();
          lcd.setCursor(0,0);
           lcd.print(hiddenString);
          lcd.setCursor(0,1);
          lcd.print("Wrong code!");
          newstring="";
          cursor2=0;
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
           lcd.print(hiddenString);

 
        }

       
          }
        
        else{
          newstring+=inputChar;
          //lcd.setCursor(cursor2,1);
          lcd.setCursor(0,1);
          lcd.print(newstring);
         // lcd.print(inputChar);
          cursor2++;
        }
        return false;
       }
       
      }

//}