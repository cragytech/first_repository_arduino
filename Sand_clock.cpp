// This project is about to build an digital verson of real sand_clock by using some led displays, inertial mesuremet and arduino uno 

#include <Arduino.h>
#include <Wire.h>
#include <LedControl.h>

#define MPU_ADDR 0x68
#define No_display 2
#define delayTime 700    // 700-> 1 min, 1650 -> 2 min
#define delayDropTime 30

LedControl lc = LedControl(12, 11, 10, No_display); // DataIn, CLK, CS, number of devices

int displayX[9];
int displayY[9];
int displayX_1[8];
int displayY_1[8];
int fix_dropX[7] = {0,1,2,3,4,5,6};
int fix_dropY[7] = {7,6,5,4,3,2,1};
int16_t ax, ay, az;
int16_t gx, gy, gz;
int clockDir = 1;
int angle_y;
int countX = 1;
int countY = 7;
int index = 0;
int secondHalf = 0;
int no_of_rows = 0;
int hight = 7;
int hightChange = 0;
int display_1 = 1;
int display_2 = 0;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min)*(out_max - out_min)/(in_max - in_min) + out_min;
}
void led_position();
void fill_display(int displayNo);
void clear_array();
void turn_off_leds(int displayNo, int k);
void drop_leds(int displayNo, int k);
void clockOrentation();
void initialize_clock();
void counters();
bool check_orentation();
void hold_clock();

void setup() {
  Serial.begin(9600);
  Wire.begin();

  //Wake up MX7219
  for(int i = 0; i<No_display; i++) lc.shutdown(i,false);
  for(int i = 0; i<No_display; i++) lc.setIntensity(i,1);
  for(int i = 0; i<No_display; i++) lc.clearDisplay(i);

  // Wake up MPU
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);      // PWR_MGMT_1
  Wire.write(0x00);      // wake up
  Wire.endTransmission();
  Serial.println("MPU ready");
  delay(1000);
}

void loop() {
  clockOrentation();
  initialize_clock();
  delay(50);
  for(int i = 0; i < 15; i++){
    if(check_orentation()) return;
    clear_array(); 
    led_position();
    for(int k = 0; k < countX; k++){
      if(check_orentation()) return;
      turn_off_leds(display_1,k);
      drop_leds(display_2,k);
      delay(delayTime);
    }
    counters();
  }
  hold_clock();
}

void hold_clock(){
   int lastDir_2 = -999;
  clockOrentation();
  if(lastDir_2 == -999){
    lastDir_2 = clockDir;
  }
  while(lastDir_2 == clockDir){
    clockOrentation();
  }
  lastDir_2 = clockDir;
  // return;
}

bool check_orentation(){
  static int lastDir = -999;
  clockOrentation();
  if(lastDir == -999){
    lastDir = clockDir;
    return false;
  }
  if(clockDir != lastDir){
    lastDir = clockDir;
    return true;
  }
  return false;
}

void counters(){
  hightChange++;
  if(hightChange == 2){
    hightChange = 0;
    hight--;
  }
  if(no_of_rows <  8){
    countX++;
    countY--;
  }
  if(no_of_rows >= 8){
    countX--;
    countY++;
  }
  no_of_rows++;
}

void initialize_clock(){
  lc.clearDisplay(display_1);
  lc.clearDisplay(display_2);

  fill_display(display_1);
  hight = 7;
  no_of_rows = 0;
  countX = 1;
  countY = 7;
}

void drop_leds(int displayNo, int k){
   if(displayX[0] < 0 || displayY[0] < 0) return;
  if(displayNo == 1){
    for(int i = 0; i < hight; i++){
      lc.setLed(1, fix_dropX[i], fix_dropY[i], 1);
      delay(delayDropTime);
      lc.setLed(1, fix_dropX[i], fix_dropY[i], 0);
      delay(delayDropTime);
   }
   delay(20);
   lc.setLed(1, displayY[k], displayX[k],1);
  }
  if(displayNo == 0){
    for(int i = 0; i < hight; i++){
      lc.setLed(0, fix_dropY[i], fix_dropX[i], 1);
      delay(delayDropTime);
      lc.setLed(0, fix_dropY[i], fix_dropX[i], 0);
      delay(delayDropTime);
    }
    delay(20);
    lc.setLed(0, displayX[k], displayY[k],1);
  }
}

void turn_off_leds(int displayNo,int k){
  if(displayNo == 0){
    lc.setLed(0,displayX[k],displayY[k],0);
  }
  if(displayNo == 1){
    lc.setLed(1,displayY[k],displayX[k],0);
  }
}

void fill_display(int displayNo){
  for(int i = 0; i<8; i++){
    for(int j = 0; j<8; j++){
      lc.setLed(displayNo,i,j,1);
    }
  }
}

void clear_array(){
  for(int i = 0; i<9; i++){
    displayX[i] = -1;
    displayY[i] = -1;
  }
}

void led_position(){
  index = 0;
  if(no_of_rows <  8){
    for(int i = 0; i < countX; i++){
      displayX[index] = i;
      displayY[index] = index + countY;
      index++;
    }
  }
 
  if(no_of_rows == 8){
    countX = 7;
    countY = 1;
  }
  index = 0;

  if(no_of_rows >=8){
    for(int i = countY; i < 8; i++){
     displayX[index] = i;
     displayY[index] = index;
     index++;
    }
    index = 0;
  }  
}

void clockOrentation(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // ACCEL_XOUT_H   this tell from which regjister we want to start reading or from wher we are pointing
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);

  ax = Wire.read() << 8 | Wire.read();  //this will read 8 bits first then it will shift it to left by 8 bits and add the nest 8 bits to it
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read(); // temp
  gx = Wire.read() << 8 | Wire.read();
  gy = Wire.read() << 8 | Wire.read();
  gz = Wire.read() << 8 | Wire.read();

  angle_y = mapFloat(ay,-16400,16400,-90,90);
  if(angle_y >= 80 && angle_y <=90) clockDir = 1;
  else if(angle_y < -80 && angle_y >= -90)clockDir = 0;

  if(clockDir == 1){         //up
    display_1 = 0;
    display_2 = 1;
  }
  else if(clockDir == 0){    //down
    display_1 = 1;
    display_2 = 0;
  }
}
