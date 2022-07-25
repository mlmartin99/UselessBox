#include <Servo.h>
#include <LedControl.h>

Servo armServo;  // create servo object to control a servo

int pos1Arm = 45;    // variable to store the servo position for arm lowered 
int pos2Arm = -35;   // variable to store the servo position for arm raised 
int toggle = 2 ;  // analog pin used to connect the toggle switch
int switchVal ;

//LED matrix settings

const int DIN_PIN = 10;
const int CS_PIN = 12;
const int CLK_PIN = 11;

const uint64_t SMILE = 0x00204e40404e2000;

const int SMILE_LEN = sizeof(SMILE)/8;

const uint64_t FROWN = 0x00402e20202e4000;

const int FROWN_LEN = sizeof(FROWN)/8;

LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN);


void setup() {

  //set up LED display
  display.clearDisplay(0);
  display.shutdown(0, false);
  display.setIntensity(0, 10);
  
  armServo.attach(9);  // attaches the servo on pin 9 to the servo object for the arm moving servo
  pinMode(toggle, INPUT_PULLUP); //switch pin in input mode
  Serial.begin(115200); //start serial writer (debugging)
  armServo.write(pos1Arm); //put arm in starting position
  Serial.println("Set up complete");
  attachInterrupt(digitalPinToInterrupt(2), switchChanged, RISING); //attach interrupt to switch
  Serial.println("Interrupt attached.");

  displayImage(SMILE);
}

void loop(){
}

//ISR for when arm pushes switch from ON to OFF, move arm back and change face to smiley
void switchOff(){
  detachInterrupt(digitalPinToInterrupt(2));
  armServo.write(pos1Arm);
  displayImage(SMILE);
  attachInterrupt(digitalPinToInterrupt(2), switchChanged, RISING);
}

//ISR for when user toggles switch, move change face to frown and move arm to toggle switch
void switchChanged(){
  detachInterrupt(digitalPinToInterrupt(2));
  int i = 0;
  displayImage(FROWN);
  attachInterrupt(digitalPinToInterrupt(2), switchOff, FALLING); //attach interrupt to switch
  armServo.write(pos2Arm);
}


//displays image using nested for loop
void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
