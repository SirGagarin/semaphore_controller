/*
 * Semaphore controller
 *
 * Pawel Adamowicz, 2018
 */

const byte greenPin = 13;
const byte yellowTopPin = 12;
const byte redPin = 11;
const byte yellowBottomPin = 10;
const byte whitePin = 9;
const byte greenAddPin = 8;
const byte yellowAddPin = 7;
const byte reservePin = 6;

// complete set of patterns for Polish railway semaphore signal
struct LightPattern {
  byte topLightPin;
  bool isTopLightBlinking;
  byte bottomLightPin;
  bool isBottomLightBlinking;
  byte additionalLightPin;
} lightPatterns[] = {
  {greenPin, false, 0, false, 0},                               // proceed max,     expect max
  {greenPin, true, 0, false, 0},                                // proceed max,     expect 100km/h
  {yellowTopPin, true, 0, false, 0},                            // proceed max,     expect 40km/h
  {yellowTopPin, false, 0, false, 0},                           // proceed max,     expect stop
  {greenPin, false, yellowBottomPin, false, 0},                 // proceed 40km/h,  expect max
  {greenPin, true, yellowBottomPin, false, 0},                  // proceed 40km/h,  expect 100km/h
  {yellowTopPin, true, yellowBottomPin, false, 0},              // proceed 40km/h,  expect 40km/h
  {yellowTopPin, false, yellowBottomPin, false, 0},             // proceed 40km/h,  expect stop
  /*
  // additional aspects if the semaphore has a light strip
  {greenPin, false, yellowBottomPin, false, yellowAddPin},      // proceed 60km/h,  expect max
  {greenPin, true, yellowBottomPin, false, yellowAddPin},       // proceed 60km/h,  expect 100km/h
  {yellowTopPin, true, yellowBottomPin, false, yellowAddPin},   // proceed 60km/h,  expect 40km/h
  {yellowTopPin, false, yellowBottomPin, false, yellowAddPin},  // proceed 60km/h,  expect stop
  {greenPin, false, yellowBottomPin, false, greenAddPin},       // proceed 100km/h, expect max
  {greenPin, true, yellowBottomPin, false, greenAddPin},        // proceed 100km/h, expect 100km/h
  {yellowTopPin, true, yellowBottomPin, false, greenAddPin},    // proceed 100km/h, expect 40km/h
  {yellowTopPin, false, yellowBottomPin, false, greenAddPin},   // proceed 100km/h, expect stop
  */
  {redPin, false, 0, false, 0},                                 // stop
  {redPin, false, whitePin, true, 0},                           // subsidiary
  {0, false, whitePin, false, 0}                                // shunting
};

const byte patternsCount = 11;
const int blinkTime = 600;
const int lightChangeTime = 10000;

bool blinkLightOn;
byte currentPattern;
long lastLightChange;
long lastBlink;

void setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(yellowTopPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowBottomPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(greenAddPin, OUTPUT);
  pinMode(yellowAddPin, OUTPUT);
  pinMode(reservePin, OUTPUT);
  // turn off extra pins
  digitalWrite(greenPin, HIGH);
  digitalWrite(yellowTopPin, HIGH);
  digitalWrite(redPin, HIGH);
  digitalWrite(yellowBottomPin, HIGH);
  digitalWrite(whitePin, HIGH);
  digitalWrite(greenAddPin, HIGH);
  digitalWrite(yellowAddPin, HIGH);
  digitalWrite(reservePin, HIGH);
  blinkLightOn = true;
  currentPattern = 0;
  lastLightChange = 0;
  lastBlink = 0;
}

void loop() {
  // pattern change timimng
  if ((millis() - lastLightChange) > lightChangeTime)
  {
    changePattern();
  }
  // blinking light timimng
  if ((millis() - lastBlink) > blinkTime)
  {
    blinkLightOn = !blinkLightOn;
    lastBlink = millis();
  }
  setPinState(lightPatterns[currentPattern].topLightPin,true & (!lightPatterns[currentPattern].isTopLightBlinking | blinkLightOn));
  setPinState(lightPatterns[currentPattern].bottomLightPin,true & (!lightPatterns[currentPattern].isBottomLightBlinking | blinkLightOn));
  setPinState(lightPatterns[currentPattern].additionalLightPin, true);
}


void changePattern() {
  setPinState(lightPatterns[currentPattern].topLightPin, false);
  setPinState(lightPatterns[currentPattern].bottomLightPin, false);
  setPinState(lightPatterns[currentPattern].additionalLightPin, false);
  currentPattern = (currentPattern + 1) % patternsCount;
  lastLightChange = millis();
  lastBlink = millis();
}


void setPinState(byte pinId, bool state) {
  if (pinId) { // treat 0 as do-nothing pin
    digitalWrite(pinId, (state ? LOW : HIGH));
  }
}

