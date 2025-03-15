const BAUD_RATE = 9600; // Match baud rate in Arduino sketch
let port, connectBtn;
let xposition = 200, yposition = 200;
let currentColor = [255, 0, 0];
let lineWidth = 5;
let drawing = false;
let clearCanvas = false;

function setup() {
  setupSerial();
  createCanvas(windowWidth, windowHeight);
  background(255); // Ensure canvas is visible before connection
}

function draw() {
  const portIsOpen = checkPort();
  if (!portIsOpen) return;

  let str = port.readUntil("\n");
  if (str.length == 0) return;

  console.log("Received:", str); // Debug incoming data

  let values = str.trim().split(",");
  if (values[0] === "CLEAR") {
    clearCanvas = true;
  } else if (values[0] === "DRAW") {
    drawing = true;
  } else {
    drawing = false;
  }

  if (clearCanvas) {
    background(255);
    clearCanvas = false;
  }

  if (values.length >= 5) {
    xposition = map(Number(values[0]), 0, 1023, 0, width);
    yposition = map(Number(values[1]), 0, 1023, height, 0);
    currentColor = [Number(values[2]), Number(values[3]), Number(values[4])];
    lineWidth = Number(values[5]) || 5;
  }

  if (drawing) {
    fill(currentColor);
    stroke(currentColor);
    strokeWeight(lineWidth);
    ellipse(xposition, yposition, lineWidth, lineWidth);
  }
}

function setupSerial() {
  port = createSerial();
  let usedPorts = usedSerialPorts();
  if (usedPorts.length > 0) {
    port.open(usedPorts[0], BAUD_RATE);
  }

  connectBtn = createButton("Connect to Arduino");
  connectBtn.position(5, 5);
  connectBtn.mouseClicked(onConnectButtonClicked);
}


function checkPort() {
  if (!port.opened()) {
    connectBtn.html("Connect to Arduino");
    background(255); // Ensure a white screen when disconnected
    return false;
  } else {
    connectBtn.html("Disconnect");
    return true;
  }
}

function onConnectButtonClicked() {
  if (!port.opened()) {
    port.open(BAUD_RATE);
  } else {
    port.close();
  }
}
