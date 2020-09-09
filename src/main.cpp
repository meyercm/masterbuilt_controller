#include <Arduino.h>
#include <stdio.h>

const int PERIOD = 10000;

int setpoint = 225;
int thermocouple = -1;
float temperature = -1;
float integral_decay = 0.9;
int last = -1;
int bias = 15;

float k_p = 1.5;
float k_i = 0.02;
float k_d = 10;
float e_p = -1;
float e_i = 0;
float e_d = 0;
float output = 0;
int pwm = 0;

void writeStatus() {
  Serial.print(millis());Serial.print("\t");
  Serial.print(setpoint);Serial.print("\t");
  Serial.print(temperature);Serial.print("\t");
  Serial.print(pwm);Serial.print("\t");
  Serial.print(e_p);Serial.print("\t");
  Serial.print(e_i);Serial.print("\t");
  Serial.print(e_d);Serial.print("\t");
  Serial.print(output);Serial.print("\t");

  Serial.print(thermocouple);Serial.print("\t");
  Serial.print(integral_decay);Serial.print("\t");
  Serial.print(bias);Serial.print("\t");
  Serial.print(k_p);Serial.print("\t");
  Serial.print(k_i);Serial.print("\t");
  Serial.print(k_d);Serial.print("\t");
  Serial.println("");
} 

void readThermocouple() {
  thermocouple = analogRead(A0);
  temperature = 41 + thermocouple * 0.3134;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  readThermocouple();
}

void recompute() {
  last = e_p;
  e_p = setpoint + bias - temperature;
  e_i = e_i * integral_decay + e_p;
  e_d = e_p - last;
  output = e_p * k_p + e_i * k_i + e_d * k_d;
  pwm = (output <=0 ? 0 : output >= 100 ? 1 : output / 100) * PERIOD;
}

void readCommand() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); 
    if (input) {
      if (sscanf(input.c_str(), "sp%i", &setpoint)) {
        return;
      }
      if (sscanf(input.c_str(), "b%i", &bias)) {
        return;
      }
      if (sscanf(input.c_str(), "id%f", &integral_decay)) {
        return;
      }
      if (sscanf(input.c_str(), "kp%f", &k_p)) {
        return;
      }
      if (sscanf(input.c_str(), "ki%f", &k_i)) {
        return;
      }
      if (sscanf(input.c_str(), "kd%f", &k_d)) {
        return;
      }
    }

  }
}

void loop() {
  readCommand();
  readThermocouple();
  recompute();
  writeStatus();
  if (pwm <= 0) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(PERIOD);
  } else if (pwm >= PERIOD) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(PERIOD);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(pwm);
    digitalWrite(LED_BUILTIN, LOW);
    delay(PERIOD - pwm);
  }
}


