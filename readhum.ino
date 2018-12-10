int data;
int analogPin = 3;
char userInput;

void setup() {
  Serial.begin(9600);
}

void water() {
  //do pumping code here
  Serial.println("watering 5ml!\n");
  return;
}

void readhum() {
  //do humidity sensor code here, print humidity lvl
  Serial.println("reading humidity!");
  return;
}

void loop() {
  if (Serial.available() > 0) {
    userInput = Serial.read();
    if (userInput == '1') {
      water();
    } 
    else if (userInput == '2') {
      readhum();
    }
  }
}