#include <Arduino.h>

#define RELAY_PIN 4
#define BAUDRATE 115200

#define TURNED_ON_FOR_MS 1500
#define TURNED_OFF_FOR_MS 6000

#define RELAY_ON LOW
#define RELAY_OFF HIGH

hw_timer_t *timer = nullptr;
volatile uint32_t elapsedMs = 0;
volatile bool shouldTurnOn = false;
volatile bool shouldTurnOff = false;
volatile bool relayIsOn = false;

void IRAM_ATTR onTimer() {
  elapsedMs++;

  if (relayIsOn && elapsedMs >= TURNED_ON_FOR_MS) {
    shouldTurnOff = true;
  }

  else if (!relayIsOn && elapsedMs >= TURNED_OFF_FOR_MS) {
    shouldTurnOn = true;
  }
}

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
}

void loop() {
  if (shouldTurnOff) {
    shouldTurnOff = false;
    digitalWrite(RELAY_PIN, RELAY_OFF);
    relayIsOn = false;
    elapsedMs = 0;

    Serial.println("Relay OFF");
  }

  if (shouldTurnOn) {
    shouldTurnOn = false;
    digitalWrite(RELAY_PIN, RELAY_ON);
    relayIsOn = true;
    elapsedMs = 0;

    Serial.println("Relay ON");
  }
  
}

