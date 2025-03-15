#include <Wire.h>

// Joystick Pins
const int joystickX = A0;
const int joystickY = A1;
const int buttonPin = 2;

// RGB LED Pins
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

// 7-Segment Display Pins (A-G, DP)
const int segPins[8] = {13, 12, 8, 7, 6, 5, 4, 0}; // A, B, C, D, E, F, G, DP

// Segment encoding for digits 0-9
const byte digitMap[10] = {
  0b11111100, // 0 -> A, B, C, D, E, F ON
  0b01100000, // 1 -> B, C ON
  0b11011010, // 2 -> A, B, G, E, D ON
  0b11110010, // 3 -> A, B, C, D, G ON
  0b01100110, // 4 -> B, C, F, G ON
  0b10110110, // 5 -> A, C, D, F, G ON
  0b10111110, // 6 -> A, C, D, E, F, G ON
  0b11100000, // 7 -> A, B, C ON
  0b11111110, // 8 -> ALL ON
  0b11110110  // 9 -> A, B, C, D, F, G ON
};

// Variables
int mode = 0; // 0: Color Selection, 1: Brightness Adjustment, 2: Line Width, 3: Drawing
int colorIndex = 0; // 0: Red, 1: Green, 2: Blue
int brightness[3] = {255, 255, 255}; // RGB brightness
int lineWidth = 5; // Default line width
bool buttonState = false;
bool lastButtonState = false;
unsigned long lastPressTime = 0;
bool drawing = false;

void setup() {
    pinMode(joystickX, INPUT);
    pinMode(joystickY, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    // Set 7-Segment Display Pins as Output
    for (int i = 0; i < 8; i++) {
        pinMode(segPins[i], OUTPUT);
    }

    Serial.begin(9600);
    updateSegmentDisplay(lineWidth); // Show default line width
}

void loop() {
    int xValue = analogRead(joystickX);
    int yValue = analogRead(joystickY);
    bool buttonPressed = digitalRead(buttonPin) == LOW;

    // Detect button press (Debouncing)
    if (buttonPressed && !lastButtonState) {
        unsigned long currentTime = millis();
        if (currentTime - lastPressTime < 500) { // Double press detection
            if (mode == 2) {
                mode = 3; // Start drawing mode
                drawing = true;
            } else if (mode == 3) {
                drawing = false; // Stop drawing
            }
        } else {
            mode = (mode + 1) % 4; // Cycle through modes
        }
        lastPressTime = currentTime;
    }
    lastButtonState = buttonPressed;

    if (mode == 0) { // Color Selection Mode
        if (xValue > 800) {
            colorIndex = (colorIndex + 1) % 3; // Right
        } else if (xValue < 200) {
            colorIndex = (colorIndex - 1 + 3) % 3; // Left
        }
    } else if (mode == 1) { // Brightness Adjustment Mode
        if (yValue > 800) {
            brightness[colorIndex] = constrain(brightness[colorIndex] + 10, 0, 255);
        } else if (yValue < 200) {
            brightness[colorIndex] = constrain(brightness[colorIndex] - 10, 0, 255);
        }
    } else if (mode == 2) { // Line Width Mode (Joystick controls line width, display updates)
        if (yValue > 800) {
            lineWidth = constrain(lineWidth + 1, 1, 9);
            updateSegmentDisplay(lineWidth);
            delay(200); // Prevent rapid increase
        } else if (yValue < 200) {
            lineWidth = constrain(lineWidth - 1, 1, 9);
            updateSegmentDisplay(lineWidth);
            delay(200); // Prevent rapid decrease
        }
    } else if (mode == 3) { // Drawing Mode
        if (buttonPressed) {
            Serial.println("CLEAR"); // Clear canvas when button is held
        } else if (drawing) {
            Serial.println("DRAW"); // Send drawing command
        }
    }

    // Update RGB LED
    analogWrite(redPin, brightness[0]);
    analogWrite(greenPin, brightness[1]);
    analogWrite(bluePin, brightness[2]);

    // Send data to PC
    Serial.print("MODE: "); Serial.print(mode);
    Serial.print(" | Color: "); Serial.print(colorIndex);
    Serial.print(" | Brightness: "); Serial.print(brightness[colorIndex]);
    Serial.print(" | Line Width: "); Serial.print(lineWidth);
    Serial.print(" | Drawing: "); Serial.println(drawing);

    int xposition = analogRead(joystickX); //read x position from pin
    int yposition = analogRead(joystickY); //read y position from pin (joy stick)
    Serial.print(xposition); //print x position to serial (x, y)
    Serial.print(",");
    Serial.println(yposition); //print y position to serial in forms of (x, y)


    delay(100);
}

// Function to Update 7-Segment Display
void updateSegmentDisplay(int num) {
    byte segments = digitMap[num]; // Get segment encoding for the number
    for (int i = 0; i < 8; i++) {
        digitalWrite(segPins[i], (segments >> i) & 0x01);
    }
}