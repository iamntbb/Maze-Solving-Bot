// IR Sensors
int sensor1 = 5;      // Left most sensor
int sensor2 = 2;
int sensor3 = 3;
int sensor4 = 4;      // Right most sensor

// Initial Values of Sensors
int sensors[4] = {0, 0, 0, 0};

// Motor Variables
int ENA = 6;
int motorInput1 = 7;
int motorInput2 = 8;
int motorInput3 = 9;
int motorInput4 = 10;
int ENB = 11;

int reachedEnd = 0;

int left_speed = 130, right_speed = 130;

void setup() {
  Serial.begin(9600);
  Serial.println("Activating Sensors");
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  Serial.println("Activating Motors");
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  //Forward: 1
  if(readSensors(0, 1, 1, 0) == 1) {
    Serial.println("Forward");
    motor1(1);
    motor2(1);
  }

    
  //Only left: 2
  if(readSensors(1, 1, 1, 0) == 1) {
    Serial.println("Left");
    // delay(50);
    while(readSensors(0, 1, 1, 0) == -1) {
      motor1(0);
      motor2(1);
      delay(100);
    }
  }
  
  //All black => T Junction, Croos Roads or End Reached: 3
  if(readSensors(1, 1, 1, 1) == 1) {
    Serial.println("T Junction, Croos Roads or End Reached");
    motor1(1);
    motor2(1);
    delay(500);
    motor1(0);
    motor2(0);
    if(readSensors(1, 1, 1, 1) == 1) {
      Serial.println("Reached End");
      reachedEnd = 1;
      left_speed = 0;
      right_speed = 0;
      Serial.end();
    } else {
      Serial.println("Taking left at Junction");
      motor1(-1);
      motor2(-1);
      delay(500);
      // motor1(1);
      // motor2(1);
      // delay(50);
      while(readSensors(0, 1, 1, 0) == -1) {
        motor1(0);
        motor2(1);
      }
      delay(100);
    }
  }
  
  //4
  if(readSensors(0, 1, 1, 1) == 1) {
    Serial.println("Straight / Right");
    motor1(1);
    motor2(1);
    delay(500);
    motor1(0);
    motor2(0);
    if(readSensors(0, 1, 1, 0) == 1) {
      Serial.println("Straight");
      //continue on the straight path
    } else {
      //only right
      Serial.println("Right");
      motor1(-1);
      motor2(-1);
      delay(500);
      // motor1(1);
      // motor2(1);
      // delay(50);
      while(readSensors(0, 1, 1, 0) == -1) {
        motor1(1);
        motor2(0);
      }
      delay(100);
    }
  }
  
  //5
  if(readSensors(0, 0, 0, 0) == 1) {
    while(readSensors(0, 1, 1, 0) == -1) {
      motor1(1);
      motor2(-1);
    }
  }
  
  // //6, 7
  // if(readSensors(1, 1, 0, 0) == 1 || readSensors(1, 0, 0, 0) == 1) {
  //   while(readSensors(0, 1, 1, 0) == -1 || readSensors(1, 1, 1, 0) == - 1 || readSensors(0, 1, 1, 1) == -1) {
  //     motor1(left_speed-20);
  //     motor2(right_speed);
  //   }
  // }
  
  // //8, 9
  // if(readSensors(0, 0, 1, 1) == 1 || readSensors(0, 0, 0, 1) == 1) {
  //   while(readSensors(0, 1, 1, 0) == -1 || readSensors(1, 1, 1, 0) == - 1 || readSensors(0, 1, 1, 1) == -1) {
  //     motor1(left_speed);
  //     motor2(right_speed-20);
  //   }
  // }
}


void motor1(int dir) {
  if(dir == 1) {
    analogWrite(ENA, left_speed);
    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
  } else if(dir == -1) {
    analogWrite(ENA, left_speed);
    digitalWrite(motorInput1, LOW);
    digitalWrite(motorInput2, HIGH);
  } else if(dir == 0) {
    analogWrite(ENA, left_speed);
    digitalWrite(motorInput1, LOW);
    digitalWrite(motorInput2, LOW);
  } else {
    if(dir > 0) {
      analogWrite(ENA, dir);
      digitalWrite(motorInput1, HIGH);
      digitalWrite(motorInput2, LOW);
    } else if(dir < 0) {
      analogWrite(ENA, -1 * dir);
      digitalWrite(motorInput1, LOW);
      digitalWrite(motorInput2, HIGH);
    }
  }
}


void motor2(int dir) {
  if(dir == 1) {
    analogWrite(ENB, right_speed);
    digitalWrite(motorInput3, HIGH);
    digitalWrite(motorInput4, LOW);
  } else if(dir == -1) {
    analogWrite(ENB, right_speed);
    digitalWrite(motorInput3, LOW);
    digitalWrite(motorInput4, HIGH);
  } else if(dir == 0) { 
    analogWrite(ENB, right_speed);
    digitalWrite(motorInput3, LOW);
    digitalWrite(motorInput4, LOW);
  } else {
    if(dir > 0) {
      analogWrite(ENB, dir);
      digitalWrite(motorInput3, HIGH);
      digitalWrite(motorInput4, LOW);
    } else if(dir < 0) {
      analogWrite(ENB, -1 * dir);
      digitalWrite(motorInput3, LOW);
      digitalWrite(motorInput4, HIGH);
    }
  }
}

int readSensors(int a, int b, int c, int d) {
  // Serial.println("ReadSensors");
  if(digitalRead(sensor1) == a && digitalRead(sensor2) == b && digitalRead(sensor3) == c &&  digitalRead(sensor4) == d) {
    return 1;
  } else {
    return -1;
  }
}
