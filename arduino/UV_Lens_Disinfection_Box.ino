/*
 * Lens Disinfection Box - Hackathon Prototype
 * Aravind Centre for Eye Care Innovation (ACEi) Hackathon '25
 *
 * Controller: Arduino Uno
 *
 * Function:
 * - The box lid uses two metal contacts as a simple mechanical switch.
 * - When the box is closed, the contacts complete the circuit and the
 *   disinfection cycle starts.
 * - A blue LED is used as the demonstration light source during the timed cycle.
 * - Red LED indicates an active cycle.
 * - Green LED indicates cycle completion.
 * - Buzzer provides start and completion alerts.
 * - A 16x2 I2C LCD displays the current state.
 * - Opening the box immediately disables the blue LED output and resets the cycle.
 *
 * IMPORTANT:
 * This is a hackathon proof-of-concept. UV-C exposure can be hazardous to
 * eyes and skin. The prototype must not be treated as a validated medical
 * sterilizer/disinfection device without appropriate optical, microbiological,
 * electrical and mechanical validation.
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------------------
// LCD
// ---------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------------------
// Pin definitions - Arduino Uno
// ---------------------------
const byte CLAMP_PIN    = 5;  // Metal contacts: LOW = box closed
const byte RED_LED_PIN  = 6;  // Active-cycle indicator
const byte GREEN_LED_PIN = 2; // Cycle-complete indicator
const byte BLUE_LED_1_PIN = 4;
const byte BLUE_LED_2_PIN = 3;
const byte BUZZER_PIN   = 7;
const byte UV_PIN       = 8;  // Drives the blue demonstration LED

// ---------------------------
// Timing
// ---------------------------
const unsigned long DISINFECTION_TIME = 10000UL; // 10 s prototype cycle
const unsigned long START_BEEP_TIME   = 500UL;
const unsigned long DONE_BEEP_TIME    = 3000UL;

// ---------------------------
// State
// ---------------------------
bool cycleActive = false;
bool cycleComplete = false;
unsigned long cycleStartTime = 0;

// ---------------------------
// Helpers
// ---------------------------
void showLCD(const char* line1, const char* line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void allOutputsOff() {
  digitalWrite(UV_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_1_PIN, LOW);
  digitalWrite(BLUE_LED_2_PIN, LOW);
  noTone(BUZZER_PIN);
}

void startCycle() {
  cycleActive = true;
  cycleComplete = false;
  cycleStartTime = millis();

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(UV_PIN, HIGH);
  digitalWrite(BLUE_LED_1_PIN, HIGH);
  digitalWrite(BLUE_LED_2_PIN, HIGH);

  tone(BUZZER_PIN, 1000, START_BEEP_TIME);

  showLCD("DISINFECTION", "IN PROGRESS...");
  Serial.println(F("Box closed -> disinfection started."));
}

void finishCycle() {
  cycleActive = false;
  cycleComplete = true;

  digitalWrite(UV_PIN, LOW);
  digitalWrite(BLUE_LED_1_PIN, LOW);
  digitalWrite(BLUE_LED_2_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);

  showLCD("CYCLE COMPLETE", "SAFE TO OPEN");
  tone(BUZZER_PIN, 1500, DONE_BEEP_TIME);

  Serial.println(F("Timed cycle complete -> BLUE LED OFF."));
}

void resetAfterOpening() {
  cycleActive = false;
  cycleComplete = false;

  allOutputsOff();
  digitalWrite(GREEN_LED_PIN, LOW);

  showLCD("BOX OPEN", "READY");
  Serial.println(F("Box opened -> outputs OFF and cycle reset."));
}

// ---------------------------
// Setup
// ---------------------------
void setup() {
  pinMode(CLAMP_PIN, INPUT_PULLUP);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_1_PIN, OUTPUT);
  pinMode(BLUE_LED_2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(UV_PIN, OUTPUT);

  allOutputsOff();
  digitalWrite(GREEN_LED_PIN, LOW);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  showLCD("LENS BOX", "READY");

  Serial.println(F("Lens Disinfection Box initialized."));
}

// ---------------------------
// Main loop
// ---------------------------
void loop() {
  // Metal contacts are connected when the box is closed.
  // INPUT_PULLUP means:
  //   LOW  -> closed
  //   HIGH -> open
  const bool boxClosed = (digitalRead(CLAMP_PIN) == LOW);

  if (!boxClosed) {
    // Opening the lid is an immediate safety reset.
    if (cycleActive || cycleComplete) {
      resetAfterOpening();
    }
    return;
  }

  // Box is closed.
  if (!cycleActive && !cycleComplete) {
    startCycle();
  }

  // Timed cycle.
  if (cycleActive) {
    unsigned long elapsed = millis() - cycleStartTime;

    if (elapsed >= DISINFECTION_TIME) {
      finishCycle();
    } else {
      // Keep the display updated with remaining time.
      static unsigned long lastDisplayUpdate = 0;

      if (millis() - lastDisplayUpdate >= 250UL) {
        lastDisplayUpdate = millis();

        unsigned long remaining = (DISINFECTION_TIME - elapsed + 999UL) / 1000UL;

        lcd.setCursor(0, 0);
        lcd.print("BLUE LED ACTIVE       ");

        lcd.setCursor(0, 1);
        lcd.print("Time: ");
        lcd.print(remaining);
        lcd.print(" sec   ");
      }
    }
  }
}
