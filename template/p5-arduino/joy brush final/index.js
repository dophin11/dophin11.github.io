
const BAUD_RATE = 9600; // This should match the baud rate in your Arduino sketch
let R = 0; //initializing the RGB led values
let G = 0;
let B = 0;

let drawingColor; //initializing the drawing color
let port, connectBtn,circleDiameter; // Declare global variables


function setup() {
  setupSerial(); // Run our serial setup function (below)

  // Create a canvas that is the size of our browser window.
  // windowWidth and windowHeight are p5 variables
  createCanvas(windowWidth, windowHeight);
  background("grey"); // Background color

}

function draw() {
    const portIsOpen = checkPort(); // Check whether the port is open (see checkPort function below)
    if (!portIsOpen) return; // If the port is not open, exit the draw loop
  
    let str = port.readUntil("\n"); // Read from the port until the newline
    if (str.length == 0) return; // If we didn't read anything, return.
    let values = str.trim().split(","); //split the reading when it has a "," (this is an array)

    //assigning the values to what is being sent from Arduino through print (in order of print)
    let colorIndex = Number(values[3]);
    let Rbrightness = Number(values[4]);
    let mode = Number(values[0]);
    let xVal = Number(values[1]);
    let yVal = Number(values[2]);
    let brush = Number(values[5]);
    let Gbrightness = Number(values[6]);
    let Bbrightness = Number(values[7]);

    //check which color the user is setting
    if(colorIndex == 0){ //if setting red
        R = Number(Rbrightness); //assign red brightness to r intensity on screen display
    }else if(colorIndex == 1){ //if setting green
        G = Number(Gbrightness); //assign red brightness to g intensity on screen display
    }else if(colorIndex == 2){ //if setting blue
        B = Number(Bbrightness); //assign red brightness to b intensity on screen display
    }

  
    if (mode === 0) { //check if it is clear canvas mode
        background("grey"); // Background color
    }
    if (mode === 1){ //check if it is setting mode
        noStroke(); //no stroke around circle
        fill(R, G, B);  // Blended RGB color
        circle(windowWidth / 2, windowHeight / 2, brush);//draw circle with the brush size and set color

    }
    if (mode === 2) { //start drawing mode
        drawingColor = color(R,G,B); //set color to what is being set by the user
        x = map(Number(xVal), 0, 1023, 0, width); //map joystick position to width
        y = map(Number(yVal), 0, 1023, 0, height); //map joystick position to height
        noStroke(); //no strock
        fill(drawingColor); // fill color for the circlue
        circle(x, y, brush); // draw circle at joystick position
    }


}

function mouseDragged() {
    // Set the color based on the mouse position, and draw a line
    // from the previous position to the current position
    strokeWeight(10);//set weight
    let lineHue = constrain(((mouseX - mouseY)+R+G+B)/6, 0, 360); //an equation to allow the change of hue with the change in position of the mouse
    //constrain the hue within 360
    stroke(lineHue, 90, 90); //set stroke
    line(pmouseX, pmouseY, mouseX, mouseY); // draw line
}


// Three helper functions for managing the serial connection.
function setupSerial() {
  port = createSerial();

  // Check to see if there are any ports we have used previously
  let usedPorts = usedSerialPorts();
  if (usedPorts.length > 0) {
    // If there are ports we've used, open the first one
    port.open(usedPorts[0], BAUD_RATE);
  }

  // create a connect button
  connectBtn = createButton("Connect to Arduino");
  connectBtn.position(5, 5); // Position the button in the top left of the screen.
  connectBtn.mouseClicked(onConnectButtonClicked); // When the button is clicked, run the onConnectButtonClicked function
}

function checkPort() {
  if (!port.opened()) {
    // If the port is not open, change button text
    connectBtn.html("Connect to Arduino");
    // Set background to gray
    background("gray");
    return false;
  } else {
    // Otherwise we are connected
    connectBtn.html("Disconnect");
    return true;
  }
}

function onConnectButtonClicked() {
  // When the connect button is clicked
  if (!port.opened()) {
    // If the port is not opened, we open it
    port.open(BAUD_RATE);
  } else {
    // Otherwise, we close it!
    port.close();
  }
}

  
