// for short press we will use '0'
// for long press we will use '-'

#include <Arduino.h>
#include <string.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,2);

#define buzzer 2
#define button 3
#define dot 125
#define space 400

long pressTime,releaseTime;
char signal[100];
int timeDiff;
int i = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  pressTime = millis();
}

void loop() {
  if(digitalRead(button) == 0){          //  this line of code will detect that when we start to press the button and when we release the button
    pressTime = millis();
    digitalWrite(buzzer,HIGH);
    while(digitalRead(button) == 0){}
    releaseTime = millis();
    digitalWrite(buzzer,LOW);

    timeDiff = releaseTime - pressTime;  //here we will canculate the difference between the pressTime and releaseTime 
 
    if(timeDiff < space){               // this will compare the timeDifference and add a 0 or - to the signal array  
      if(timeDiff < dot && timeDiff > 10){               // if timeDifference is less then dot(100mS) the it will be treated as '0' otherwise '-'
        signal[i] = '0';
        i++;
      }
      else{
        signal[i] = '-';
        i++;
      }
    } 

    if(timeDiff > space){     
      Serial.print(signal);             // by comparing signal it will decide that acording to signal which char to print on serial 
      if(strcmp(signal,"0-") == 0){
        Serial.print("a");
        lcd.write('a');
      }else if(strcmp(signal,"-000") == 0){
        Serial.print("b");
        lcd.write('b');
      }else if(strcmp(signal,"-0-0") == 0){
        Serial.print("c");
        lcd.write('c');
      }else if(strcmp(signal,"-00") == 0){
        Serial.print("d");
        lcd.write('d');

      }else if(strcmp(signal,"0") == 0){
        Serial.print("e");
        lcd.write('e');
      }else if(strcmp(signal,"00-0") == 0){
        Serial.print("f");
        lcd.write('f');
      }else if(strcmp(signal,"--0") == 0){
        Serial.print("g");
        lcd.write('g');
      }else if(strcmp(signal,"0000") == 0){
        Serial.print("h");
        lcd.write('h');
      }else if(strcmp(signal,"00") == 0){
        Serial.print("i");
        lcd.write('i');
      }else if(strcmp(signal,"0---") == 0){
        Serial.print("j");
        lcd.write('j');
      }else if(strcmp(signal,"-0-") == 0){
        Serial.print("k");
        lcd.write('k');
      }else if(strcmp(signal,"0-00") == 0){
        Serial.print("l");
        lcd.write('l');
      }else if(strcmp(signal,"--") == 0){
        Serial.print("m");
        lcd.write('m');
      }else if(strcmp(signal,"-0") == 0){
        Serial.print("n");
        lcd.write('n');
      }else if(strcmp(signal,"---") == 0){
        Serial.print("o");
        lcd.write('o');
      }else if(strcmp(signal,"0--0") == 0){
        Serial.print("p");
        lcd.write('p');
      }else if(strcmp(signal,"--0-") == 0){
        Serial.print("q");
        lcd.write('q');
      }else if(strcmp(signal,"0-0") == 0){
        Serial.print("r");
        lcd.write('r');
      }else if(strcmp(signal,"000") == 0){
        Serial.print("s");
        lcd.write('s');
      }else if(strcmp(signal,"-") == 0){
        Serial.print("t");
        lcd.write('t');
      }else if(strcmp(signal,"00-") == 0){
        Serial.print("u");
        lcd.write('a');
      }else if(strcmp(signal,"000-") == 0){
        Serial.print("v");
        lcd.write('v');
      }else if(strcmp(signal,"0--") == 0){
        Serial.print("w");
        lcd.write('w');
      }else if(strcmp(signal,"-00-") == 0){
        Serial.print("x");
        lcd.write('x');
      }else if(strcmp(signal,"-0--") == 0){
        Serial.print("y");
        lcd.write('y');
      }else if(strcmp(signal,"--00") == 0){
        Serial.print("z");
        lcd.write('z');
      }
      else if(strcmp(signal,"000000") == 0){
        Serial.print(" ");
        lcd.write(' ');
      }else if(strcmp(signal,"-00000") == 0){
        lcd.clear();
        Serial.println("clear");
      }

      for(int k = 0; k< 10; k++){            //this will reset the array by assignig null vlue to it
        signal[k] = '\0';
      }
      i = 0;
    }
  }
}
