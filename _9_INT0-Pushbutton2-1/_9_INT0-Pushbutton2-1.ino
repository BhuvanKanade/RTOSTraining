#define LED8 8
#define LED9 9
#define LED10 10
#define LED11 11
//const int buttonPin = 2;     // the number of the pushbutton pin
//const int ledPin =  13;      // the number of the LED pin
const int switchPin = 2; //Switch Connected to PIN 2
// variables will change:
volatile int switchState = 1; //Variable for reading switch status
void setup()
{
  pinMode(switchPin, INPUT_PULLUP); //Switch PIN is Input
  // initialize the LED pin as an output:
  pinMode(LED8,OUTPUT);
  pinMode(LED9,OUTPUT);
  pinMode(LED10,OUTPUT);
  pinMode(LED11,OUTPUT);
  // initialize the pushbutton pin as an input:
  digitalWrite(LED8,HIGH);
  digitalWrite(LED9,HIGH); 
  digitalWrite(LED10,HIGH);
  digitalWrite(LED11,HIGH);
  //pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  switchState = digitalRead(switchPin); //Read the status of the Switch
  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(2), pin_ISR, CHANGE);
}

void loop() {
  // Nothing here!
  /*digitalWrite(LED8,LOW);
  digitalWrite(LED9,LOW); 
  digitalWrite(LED10,LOW);
  digitalWrite(LED11,LOW);
  delay(1000);
  digitalWrite(LED8,HIGH);
  digitalWrite(LED9,HIGH); 
  digitalWrite(LED10,HIGH);
  digitalWrite(LED11,HIGH);
  delay(3000);*/

  //Comment the above after experimenting i.e., make the loop nothing.
}

void pin_ISR()
{
  switchState = digitalRead(switchPin); //Read the status of the Switch //COMMENT digital read for TESTING
  digitalWrite(LED8,switchState);
  digitalWrite(LED9,switchState); 
  digitalWrite(LED10,switchState);
  digitalWrite(LED11,switchState);
  
  /*int i;
  for(i=0;i<10;i++)
  {
  digitalWrite(LED8,LOW);
  digitalWrite(LED9,LOW); 
  digitalWrite(LED10,LOW);
  digitalWrite(LED11,LOW);
  delay(200);
  digitalWrite(LED8,HIGH);
  digitalWrite(LED9,HIGH); 
  digitalWrite(LED10,HIGH);
  digitalWrite(LED11,HIGH);
  delay(500);
  }*/
}

/* ORIGINAL PROGRAM
    * const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
volatile int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // Attach an interrupt to the ISR vector
  attachInterrupt(0, pin_ISR, CHANGE);
}

void loop() {
  // Nothing here!
}

void pin_ISR() {
  buttonState = digitalRead(buttonPin);
  digitalWrite(ledPin, buttonState);
}
*/
                  
