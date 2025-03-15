#include "SevSeg.h" //7 digit display library
SevSeg sevseg; // initialize library 

//button set up
const int BUTTON_PIN = 2; //button pin
int buttonState = 0; // button state set up
int mode = 1; //mode set up
int press = 0; // button press set up

//joystick setup
const int xposition = A0; //initialize pin number for x and y position reading
const int yposition = A1;

int set = 0; //initialize set (changing set within mode)
int Rbrightness = 50; //initialize led brightness
int Gbrightness = 50;
int Bbrightness = 50;
const int R = 9; //initialize led pins
const int G = 10;
const int B = 11;

int time = 7; //constant time for delaying the response
int hold = 0; //initialize hold time

int brush = 0; //initialize brush


void setup(){
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);//set up button pin as input
  for(int i = 9; i < 12; i++){ //set up led pins as output
    pinMode(i, OUTPUT);
  }
  byte numDigits = 1; //use library to set pins
  byte digitPins[] = {};
  byte segmentPins[] = {12,13,3,5,6,7,8,4}; //provide the pins connected to the diplay
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE; //set common cathode mode
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments); //provide required data for library
  sevseg.setBrightness(90); //set display brightness
}

void loop(){
  //button pressed function
  buttonPress();
  //update mode based on the button press
  if (press == 1) { //set loop for button press
    if (mode == 1) { //change between mode 1 and 2 if pressed
      mode = 2;
    } else if (mode == 2) {
      mode = 1;
    } else if (mode == 0) {  // If in Mode 0, return to Mode 1 when pressed again
      mode = 1;
    }
  } 
  else if (press == 2) { //detects if button is being held
    mode = 0;  // Switch to Mode 0 if held long enough
  }
  int x = analogRead(xposition); //read joystick position (x)
  int y = analogRead(yposition); //(y)
  if (mode == 1){ //check mode
    if(x == 0){  //joystick position left
      time = time - 1; //update time count, so it doesnt respond as fast
      if(time == 0){
        set = constrain(set - 1, 0, 3); //update set, constrain set to 0-3
        time = 7; //reset time count
      }
    }else if(x == 1023){ //joystick position right, similar as above
      time = time - 1;
      if(time == 0){
        set = constrain(set + 1, 0, 3); 
        time = 7;
      }
    }else if(y == 0 && set == 0){ //joystick position up and set is in red led
      Rbrightness = constrain(Rbrightness + 5, 0, 255); //increase red led brightness
    }else if(y == 1023 && set == 0){ //joystick position down and set is in red led
      Rbrightness = constrain(Rbrightness - 5, 0, 255); //decrease red led brightness 
    }else if(y == 0 && set == 1){ //same for green
      Gbrightness = constrain(Gbrightness + 5, 0, 255);
    }else if(y == 1023 && set == 1){
      Gbrightness = constrain(Gbrightness - 5, 0, 255);
    }else if(y == 0 && set == 2){ //same for blue
      Bbrightness = constrain(Bbrightness + 5, 0, 255);
    }else if(y == 1023 && set == 2){
      Bbrightness = constrain(Bbrightness - 5, 0, 255);

    //check if it is increasing the brush size (set is 3 for brush size)
    }else if(y == 0 && set == 3){
      time --; //update time count so it doesnt respond as fast
      if(time == 0){ //check time count
        brush = constrain(brush + 1, 0, 9);  //update brush count, set constrain to 0-9
        time = 7; //reset time count
      }

    // same, but check if decreasing brush size
    }else if(y == 1023 && set == 3){
      time --;
      if(time == 0){
        brush = constrain(brush - 1, 0, 9);
        time = 7;
      }
    }

    //respond to the setting 
    //respond to red led setting
    if(set == 0){
      analogWrite(R, Rbrightness);
      digitalWrite(G, LOW);
      digitalWrite(B, LOW);
    } else if(set == 1){ //respond to green led setting
      analogWrite(G, Gbrightness);
      digitalWrite(R, LOW);
      digitalWrite(B, LOW);
    }else if (set == 2){ //respond to blue led setting
      analogWrite(B, Bbrightness);
      digitalWrite(R, LOW);
      digitalWrite(G, LOW);
    }else if (set == 3){ //respond to brush led setting, trun off all leds
      sevseg.setNumber(brush);
      sevseg.refreshDisplay();      
      digitalWrite(R, LOW);
      digitalWrite(G, LOW);
      digitalWrite(B, LOW);
    }
  }
  

  //sending the information to serial through printing with "," as sign of spliting

  Serial.print(mode);
  Serial.print(",");
  Serial.print(x); //print x position to serial (x, y)
  Serial.print(",");
  Serial.print(y); //print y position to serial in forms of (x, y)
  Serial.print(",");
  Serial.print(set);
  Serial.print(",");
  Serial.print(Rbrightness);
  Serial.print(",");
  Serial.print(brush*10);
  Serial.print(",");
  Serial.print(Gbrightness);
  Serial.print(",");
  Serial.println(Bbrightness);
  delay(50); //delay for stablization 
}

//identify if button is pressed
void buttonPress(){
  buttonState = digitalRead(BUTTON_PIN); //read button state
  if (buttonState == HIGH){
    hold++; //update hold number if button is pressed
  }else{
    if(hold > 10){ //check if button is pressed for more than 10*50ms
      press = 2; //update press to hold(2)
    } else if(hold > 0){ //check if button is pressed
      press = 1; //update press to pressed(1)
    }
    hold = 0;//reset hold
  }
}