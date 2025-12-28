  int IN1 = A4; 
  int IN2 = A5; 
  int IN3 = 13; 
  int IN4 = 12; 
  int ENA = 3; 
  int ENB = 11;

  int L_Speed = 120;
  int R_Speed = 120;


  void Run();
  void Stop();
  void Left();

  int TRIG = A1;
  int ECHO = A2;

  long duration;
  float distance;

  void setup()
  {

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    Serial.begin(9600);
  }

  void loop() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);

    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(60);

    if(distance< 30)
    {
      Stop();
      delay(500);
      Left();
      delay(2800);
    }
    else
    {
      Run();
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
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
  }

  void Left() 
  {
    analogWrite(ENA,0);
    analogWrite(ENB, 120);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }

