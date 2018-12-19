int data;
int analogPin = 3;
char userInput;
#define RELAY1  7 

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);
}

void water() {
  //do pumping code here
  Serial.println("watering 5ml!\n");
  digitalWrite(RELAY1,0);
  delay(2000);
  digitalWrite(RELAY1,1);
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