const int r = 9; //initialize pin number for RGB led
const int g = 10;
const int b = 11;

const int xposition = A0; //initialize pin number for x and y position reading
const int yposition = A1;

void setup(){
  Serial.begin(9600); //initialize serial
  for (int i = 9; i <12; i++){  //set pinMode to output for RGB led
    pinMode(i, OUTPUT);
  }
  for (int i = 9; i <12; i++){ //close all light
    analogWrite(i, 0);
  }
}

void loop(){
    if (Serial.available() > 0) { //check if any input from keyboard
        char received = Serial.read(); //read the input and store in received 
        if (received == '1') { //check if received is 1
            analogWrite(r, 100); //light led to white 
            analogWrite(g, 100); 
            analogWrite(b, 100); 
        } else if (received == '2') { //check if received is 2
            analogWrite(r, 20); //light led to blue
            analogWrite(g, 100); 
            analogWrite(b, 20); 
        } else if (received == '3') { //check if received is 3
            analogWrite(r, 10);  //light led to light blue
            analogWrite(g, 10); 
            analogWrite(b, 100); 
        }

    }
    int x = analogRead(xposition); //read x position from pin
    int y = analogRead(yposition); //read y position from pin (joy stick)
    Serial.print(x); //print x position to serial (x, y)
    Serial.print(",");
    Serial.println(y); //print y position to serial in forms of (x, y)

    delay(50);//short delay for stability
}