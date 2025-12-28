#include <LiquidCrystal.h>
#include <PinChangeInterrupt.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
unsigned long previousLCD = 0;
const long lcdInterval = 200; 


int IN1 = 1; 
int IN2 = A5;
int IN3 = 13; 
int IN4 = 12;
int ENA = 3;  
int ENB = 11; 


int Run_Speed = 150;   
int Turn_Speed = 170;  


int L_IR = A3;  
int M_IR = A1; 
int R_IR = A2;  

int L = 0;
int M = 0;
int R = 0;


String state = "STOP";


int L_Enc = 2;
int R_Enc = A4;

volatile long leftPulses = 0;
volatile long rightPulses = 0;

volatile unsigned long lastUsL = 0;
volatile unsigned long lastUsR = 0;
const unsigned long MIN_EDGE_US = 300;

float Wheel = 6;     // wheel diameter in cm
int PPR =20;
float Distance = 0;

// Timer
unsigned long T_Start = 0;
unsigned long Time = 0;

// Encoder ISR
void leftISR()
{
    unsigned long t = micros();
    if (t - lastUsL > MIN_EDGE_US) { 
        leftPulses++;
        lastUsL = t;
    }
}

void rightISR()
{
    unsigned long t = micros();
    if (t - lastUsR > MIN_EDGE_US) { 
        rightPulses++; 
        lastUsR = t;
    }
}

bool stoppedOnce = false;

void setup() {

  lcd.begin(16, 2);
  lcd.print("Line Follower");
  delay(1000);
  lcd.clear();


  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);


  pinMode(L_IR, INPUT);
  pinMode(M_IR, INPUT);
  pinMode(R_IR, INPUT);
  

  pinMode(L_Enc, INPUT_PULLUP);
  pinMode(R_Enc, INPUT_PULLUP);
  

  attachInterrupt(digitalPinToInterrupt(L_Enc), leftISR, RISING);
  attachPCINT(digitalPinToPCINT(R_Enc), rightISR, RISING);

  T_Start = millis();
}

void loop() {

  float C = 3.14 * Wheel;
  float dist_L = (leftPulses / (float)PPR) * C;
  float dist_R = (rightPulses / (float)PPR) * C;
  Distance = (dist_L + dist_R) / 2.0;
  

  if (!stoppedOnce && Distance >= 60)
  {
    stopMoving();

    lcd.setCursor(0,1);
    lcd.print("Dist:");
    lcd.print(Distance, 1);
    lcd.print("cm  ");
    delay(3000);
    stoppedOnce = true;   
  }
  readSensors(); 
  updateLCD();  


  


  if (L == 1 && M == 1 && R == 1) {
    moveForward();
    state = "Forward";
  }

  else if (L == 0 && M == 0 && R == 1 ) {
    turnLeft();
    state = "Left";
  }

  else if (L == 1 && M == 0 && R == 0 ) {
    turnRight();
    state = "Right";
  }
  

  else if (L == 0 && M == 1 && R == 0){
    stopMoving();
    state = "Stop";
    while(1);
  }
  else if (L == 1 && M == 1 && R == 0)
  {
  analogWrite(ENA, Turn_Speed);
  analogWrite(ENB, Turn_Speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(300);
 
  }
  else if (L == 0 && M == 1 && R == 1)
  {
    analogWrite(ENA, Turn_Speed);
    analogWrite(ENB, Turn_Speed);
    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    delay(300);
  }
}



void readSensors() {

  L = digitalRead(L_IR);
  M = digitalRead(M_IR);
  R = digitalRead(R_IR);
}

void moveForward() {

  analogWrite(ENA, Run_Speed);
  analogWrite(ENB, Run_Speed);


  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {

  analogWrite(ENA, Turn_Speed);
  analogWrite(ENB, Turn_Speed);


  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {

  analogWrite(ENA, Turn_Speed);
  analogWrite(ENB, Turn_Speed);


  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopMoving() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void updateLCD() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousLCD >= lcdInterval) {
    previousLCD = currentMillis;
    

    float seconds = (currentMillis - T_Start) / 1000.0;

    lcd.setCursor(0, 0);
    lcd.print("T:"); lcd.print(seconds, 1); lcd.print("s ");
    lcd.print(state); 
    
    lcd.setCursor(0, 1);
    lcd.print("Dist:");
    lcd.print(Distance, 1);
    lcd.print("cm   ");
  }
}

 