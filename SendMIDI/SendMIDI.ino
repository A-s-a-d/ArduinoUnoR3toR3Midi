// Define MIDI constants
const byte MIDI_CHANNEL = 0;  // MIDI channel 0
const byte NOTE_ON = 0x90;    // MIDI command for Note On (0x90 for channel 0)
const byte NOTE_OFF = 0x80;   // MIDI command for Note Off (0x80 for channel 0)
const byte VELOCITY = 100;    // Velocity (volume) for Note On

// Array of notes to play (e.g., C4, D4, E4, F4, G4, A4, B4, C5 in MIDI note numbers)
byte melody[] = { 60, 62, 64, 65, 67, 69, 71, 72 };
int noteDuration = 500;  // Duration for each note in milliseconds

void setup() {
  Serial.begin(31250);  // MIDI baud rate
}

void loop() {
  for (int i = 0; i < sizeof(melody); i++) {
    sendNoteOn(melody[i]);   // Send Note On message for the current note
    delay(noteDuration);     // Wait for note duration
    sendNoteOff(melody[i]);  // Send Note Off message
    delay(100);              // Short delay between notes
  }
  delay(100);  // Short delay between notes
}

// Function to send a "Note On" message
void sendNoteOn(byte note) {
  Serial.write(NOTE_ON | MIDI_CHANNEL);  // Send Note On command with channel
  Serial.write(note);                    // Send the note to play
  Serial.write(VELOCITY);                // Send velocity
}

// Function to send a "Note Off" message
void sendNoteOff(byte note) {
  Serial.write(NOTE_OFF | MIDI_CHANNEL);  // Send Note Off command with channel
  Serial.write(note);                     // Send the note to stop
  Serial.write(VELOCITY);                 // Send velocity (usually ignored for Note Off)
}
