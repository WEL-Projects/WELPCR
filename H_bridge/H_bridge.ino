// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(2, OUTPUT);

    digitalWrite(2, HIGH);
    //analogWrite(10,128);
    //analogWrite(11,128);
    
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(7, LOW);   // turn the LED on (HIGH is the voltage level)
  /*delay(2000);
  //delayMicroseconds(2000);                       // wait for a second
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);
  //delayMicroseconds(2000);                      // wait for a second
  */
}
