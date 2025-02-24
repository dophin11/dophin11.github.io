const BAUD_RATE = 9600; // match baud rate in my Arduino sketch

let port, connectBtn; // Declare global variables
let xposition = 200, yposition = 200; // initialize circle position

function setup() {
  setupSerial(); // Run serial setup function
  createCanvas(windowWidth, windowHeight);  // Create a canvas that is the size of browser window.

  textFont("system-ui", 50); //set font, style, and align (based on BOLD, CENTER provided by p5)
  textStyle(BOLD);
  textAlign(CENTER, CENTER);
}

function draw() {
  const portIsOpen = checkPort(); // Check whether the port is open (using funtion below)
  if (!portIsOpen) return; // If the port is not open, exit the draw loop

  let str = port.readUntil("\n"); // Read from the port until the newline
  if (str.length == 0) return; // If didn't read anything, return.

  let values = str.trim().split(","); //the position is given in (x, y) so split into two numbers
  if (values.length === 2) {
    xposition = map(Number(values[0]), 0, 1023, 0, width);  // Scale X to canvas width (x got from first number after split)
    yposition = map(Number(values[1]), 0, 1023, height, 0); // Scale Y to canvas height (y got from second number after split)
  }
  fill(255, 0, 0); // Red circle
  ellipse(xposition, yposition, 50, 50); // Draw circle at joystick position
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

function keyPressed() { //check which key is pressed and send to arduino
  if (port.opened()) { //check if port is open
      if (key === '1') {
          port.write('1\n'); //if key pressed is 1, write 1 to arduino
      } else if (key === '2') {
          port.write('2\n'); //same as above for key 2
      } else if (key === '3') {
          port.write('3\n'); //same as above for key 3
      }
  }
}