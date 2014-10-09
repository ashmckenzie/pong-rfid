#include <avr/sleep.h>

int buttonPressed = 0;
int wakePin = 2;
int ledPin = 13;

void setup() {
  pinMode(wakePin, INPUT);
  pinMode(ledPin, OUTPUT);

  attachInterrupt(0, wakeUpNow, LOW);
}

void wakeUpNow() {
  buttonPressed = 1;
}

void sleepNow() {
  digitalWrite(ledPin, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, wakeUpNow, LOW);
  sleep_mode();
  sleep_disable();
  detachInterrupt(0);
}

void loop() {
  sleepNow();

  if (buttonPressed == 1) {
    digitalWrite(ledPin, HIGH);
    delay(3000);
  }
}
