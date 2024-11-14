byte commandByte;
byte noteByte;
byte velocityByte;
bool noteOn = false;

const int audioOutPin = 9; // Set the output pin for PWM (e.g., pin 9 on Arduino Uno)

void setup() {
  Serial.begin(31250);
  
  pinMode(audioOutPin, OUTPUT);

  cli(); // Stop interrupts

  // Set timer2 interrupt for ~8kHz (128us intervals)
  TCCR2A = 0; // set entire TCCR2A register to 0
  TCCR2B = 0; // same for TCCR2B
  TCNT2  = 0; // initialize counter value to 0
  OCR2A = 255; // Set compare match register
  TCCR2A |= (1 << WGM21); // turn on CTC mode
  TCCR2B |= (1 << CS11);  // Set CS11 bit for 8 prescaler
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt
  
  sei(); // Allow interrupts
}

ISR(TIMER2_COMPA_vect) { // Timer interrupt for reading MIDI data
  if (Serial.available() >= 3) { // Check if at least three bytes are available
    commandByte = Serial.read();   // Read command byte
    noteByte = Serial.read();      // Read note byte
    velocityByte = Serial.read();  // Read velocity byte

    // Determine if the command is a "Note On" or "Note Off" message
    if ((commandByte & 0xF0) == 0x90 && velocityByte > 0) {
      noteOn = true;
      setFrequency(noteByte); // Set frequency based on the MIDI note
    } else if ((commandByte & 0xF0) == 0x80 || velocityByte == 0) {
      noteOn = false;
      analogWrite(audioOutPin, 0); // Turn off the output when note is off
    }
  }
}

void setFrequency(byte note) {
  // Map MIDI note (0-127) to frequency (in Hz)
  float frequency = 440.0 * pow(2.0, (note - 69) / 12.0); // A4 (69) = 440 Hz
  
  // Calculate the corresponding delay for the square wave
  int delayTime = 1000000 / (2 * frequency); // Period in microseconds

  // Output square wave using delay timing for analog audio signal
  if (noteOn) {
    analogWrite(audioOutPin, 128); // Set PWM duty cycle to 50% for square wave
    delayMicroseconds(delayTime);
    analogWrite(audioOutPin, 0);
    delayMicroseconds(delayTime);
  }
}

void loop() {
  // Main loop remains empty since timer interrupt handles the MIDI processing
}
