#include <LiquidCrystal.h>
#include <Wire.h>
#include <MPU6050_light.h>

// ---- LCD ----
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// ---- MPU6050 ----
MPU6050 mpu(Wire);
unsigned long lcdTimer = 0;


int IN1 = A1; 
int IN2 = A2;
int IN3 = 13; 
int IN4 = 12;
int ENA = 3;
int ENB = 11;


int speed = 255; 


int currentStep = 0;          
unsigned long timerStart = 0; 
float targetYaw = 0;          


float triggerVal = 0.0;      
float displayVal = 0.0;      
float maxSlope = 0.0;         


void Run();
void Stop();
void TurnLeft(); 
void updateLCD();

void setup() {
  Serial.begin(9600);
  
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Init MPU...");
  
  Wire.begin();
  byte status = mpu.begin();
  lcd.setCursor(0, 1);
  if(status != 0){
    lcd.print("MPU Error!");
    while(1);
  }
  
  lcd.print("Calibrating...");
  mpu.calcOffsets(); 
  
  lcd.clear();
  lcd.print("Ready to Go!");
  delay(1000);
  lcd.clear();
}

void loop() {
  mpu.update();

  triggerVal = abs(mpu.getAngleX()); 
  displayVal = abs(mpu.getAngleY()); 
  float y = mpu.getAngleZ();

  if (currentStep < 2) {
    if (displayVal > maxSlope) {
      maxSlope = displayVal; 
    }
  }

  switch (currentStep) {
    case 0:
      Run();
      if (triggerVal > 8.0) { 
        currentStep = 1; 
        timerStart = millis(); 
      }
      break;
      
    case 1:
      Run();
      if (millis() - timerStart > 1200) {
        currentStep = 2; 
      }
      break;

    case 2:
      Stop();
      delay(1000); 
      targetYaw = y + 345; 
      currentStep = 3;
      break;

    case 3:
      TurnLeft(); 
      if (y >= targetYaw) {
        Stop();
        delay(500); 
        currentStep = 4;
      }
      break;

    case 4:
      speed = 120; 
      Run();
      break;
  }

  if(millis() - lcdTimer > 200){
    updateLCD();
    lcdTimer = millis();
  }
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("S:"); lcd.print(currentStep);
  lcd.print(" T:"); lcd.print((int)triggerVal); 
  
  lcd.setCursor(0, 1);
  lcd.print("Max:"); 
  lcd.print(maxSlope, 1); 
  
  if (currentStep >= 2) {
    lcd.print((char)223); 
    lcd.print("*"); 
  } else {
    lcd.print((char)223); 
  }
}

void Run() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void TurnLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 255); 
  analogWrite(ENB, 255);
}

void Stop() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}