
int IN1 = A4; 
int IN2 = A5;  
int IN3 = 13;
int IN4 = 12;  
int ENA = 3;  
int ENB = 11;


int L_Speed = 250;
int R_Speed = 250;


char command; 


void Run();
void Stop();
void Left();
void Right();

void setup()
{

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
}

void loop() {


  if (Serial.available()) {
    command = Serial.read();
    Serial.println(command);

    if (command == 'F') {
      Run();
    }
    else if (command == 'B') {
      analogWrite(ENA, L_Speed);
      analogWrite(ENB, R_Speed);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    else if (command == 'L') {
      Left();
    }
    else if (command == 'R') {
      Right();
    }
    else if (command == 'S') {
      Stop();
    }
  }
}

void Run()
{
  analogWrite(ENA, L_Speed);
  analogWrite(ENB, R_Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Stop()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Left() 
{
  analogWrite(ENA, 250);
  analogWrite(ENB, 250);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Right() 
{
  analogWrite(ENA, 250);
  analogWrite(ENB, 250);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
