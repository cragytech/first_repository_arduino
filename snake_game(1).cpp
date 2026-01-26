#include <Arduino.h>
#include <LedControl.h>

#define up 3
#define right 5
#define left 2
#define down 4
#define buzzer 8

LedControl lc = LedControl(11, 13, 10, 1);  //ledControl(din,clk,cs,numberofDevices);
void snakeMove();
void printSnake();
void snakeHed();
int buttonPress();
void snakeDir();
bool foodCheck();
void snakeFood();
void printSnakeFood();
void snakeLengthIncrese();
bool snakeBite();
void gameOver();
void gameReset();
int snakeHedX = 0;
int snakeHedY = 4;
int snakeDirX = 1;
int snakeDirY = 0;
int snakeFoodX = 0;
int snakeFoodY = 0;
int snakeLength = 4;
int snakeX[64];
int snakeY[64];

void setup() {
  pinMode(up,INPUT_PULLUP);
  pinMode(down,INPUT_PULLUP);
  pinMode(right,INPUT_PULLUP);
  pinMode(left,INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  Serial.begin(9600);
  for(int i = 0; i<64; i++){
    snakeX[i] = -1;
    snakeY[i] = -1;
  }
  randomSeed(analogRead(A0)); //this will create more randomnes to randon function 
}

void loop() {
  lc.clearDisplay(0);
  snakeFood();
  printSnakeFood();
  gameOver();
  snakeMove();
  printSnake();
  snakeLengthIncrese();
  snakeDir();
  snakeHed();
  delay(400);
  Serial.print(snakeDirX);
  Serial.print(", ");
  Serial.println(snakeDirY);
}
void gameReset(){ //It will reset the whole game when the player got out
  lc.clearDisplay(0);
  for(int i = 0; i<64; i++){
    snakeX[i] = -1;
    snakeY[i] = -1;
  }
  snakeLength = 4;
  snakeHedX = 0;
  snakeHedY = 4;
  snakeFoodX = 0;
  snakeFoodY = 0;
  snakeDirX = 1;
  snakeDirY = 0;
}
bool snakeBite(){  //It will check wether if snake has biten itself or not 
  for(int i = 1; i <snakeLength -1; i++){
    if(snakeHedX == snakeX[i] && snakeHedY == snakeY[i]) return 1;
  }
  return 0;
}
void gameOver(){  //This decide that how the game will over 
  if(snakeBite() == 1){
    digitalWrite(buzzer,1);
    delay(200);
    lc.setIntensity(0,0);
    digitalWrite(buzzer,0);
    delay(400);
    lc.setIntensity(0,15);
    digitalWrite(buzzer,1);
    delay(200);
    digitalWrite(buzzer,0);
    delay(200);
    lc.setIntensity(0,0);             //It will trigir the buzzer on and off & the display on and off
    digitalWrite(buzzer,1);
    delay(200);
    digitalWrite(buzzer,0);
    delay(200);
    lc.setIntensity(0,15);
    digitalWrite(buzzer,1);
    delay(200);
    digitalWrite(buzzer,0);
    delay(200);
    lc.setIntensity(0,0);
    digitalWrite(buzzer,1);
    delay(200);
    digitalWrite(buzzer,0);
    delay(200);
    lc.setIntensity(0,8);

    gameReset();
  }
}
void snakeLengthIncrese(){ //Thsi will increase the length of the snake when it eat the food
  if(foodCheck() == true) snakeLength++;
  if(snakeLength >30) snakeLength = 4;
}

bool foodCheck(){ // this will check wether the generated food is on the snake body or not
  for(int i = 0; i <4; i++){
    if(snakeFoodX == snakeX[i] && snakeFoodY == snakeY[i]) return true;  
  }  
  return false;
}
void snakeFood(){ //This generate random food cordinate
  while(foodCheck()){
   snakeFoodX = random(0,8);
   snakeFoodY = random(0,8);
  }
}
void printSnakeFood(){ //This print's the food on display
  lc.setLed(0,snakeFoodY,snakeFoodX,true);
}
void snakeDir(){   // It stores that in which direction the snake is moving
  if(snakeDirY == -1 || snakeDirY == 1){
    if(digitalRead(up) == 0 ){
      snakeDirX = 1;
      snakeDirY = 0;
    }
    else if(digitalRead(down) == 0){
      snakeDirX = -1;
      snakeDirY = 0;
    }
  }
  else if(snakeDirX == 1 || snakeDirX == -1){
    if(digitalRead(right) == 0){
      snakeDirX = 0;
      snakeDirY = 1;
    }
    else if(digitalRead(left) == 0){
      snakeDirX = 0;
      snakeDirY = -1;
    }
  }
}
void snakeHed(){    // controls the forward movement of the snake hed 
  if(snakeDirY == 1){
    snakeHedY++;
    if(snakeHedY>7) snakeHedY=0;
  }
  else if(snakeDirY == -1){
    snakeHedY--;
    if(snakeHedY <0) snakeHedY = 7; 
  }
  else if(snakeDirX == 1){ 
    snakeHedX++;
    if(snakeHedX>7) snakeHedX = 0;
  }
  else if(snakeDirX == -1){
    snakeHedX--;
    if(snakeHedX<0) snakeHedX = 7;
  }
}
int buttonPress(){   // check if any button is pressed or not
  if(digitalRead(up) == 0) return 0;
  else if(digitalRead(down) == 0) return 0;
  else if(digitalRead(left) == 0) return 0;
  else if(digitalRead(right) == 0) return 0;
  else {return 1;}
}
void snakeMove(){     // This will the snake body by following hed
  for(int i = snakeLength-1; i >0; i--){
    snakeX[i] = snakeX[i-1]; 
    snakeY[i] = snakeY[i-1];
  }
  snakeX[0] = snakeHedX;
  snakeY[0] = snakeHedY;
}
void printSnake(){  //This will turn on those light where snake is present
  // lc.clearDisplay(0);
  for(int i = 0; i < snakeLength; i++){
    lc.setLed(0,snakeY[i],snakeX[i],true);
  }
}



