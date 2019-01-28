const int channel = 1;

#define led 13

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, HIGH);
  usbMIDI.sendNoteOn(69, 127, channel);
  for(int i = 0; i < 127; i++){
    usbMIDI.sendAfterTouch(i, channel);
    usbMIDI.send_now();
    d(10);
  }
  digitalWrite(led, LOW);
  usbMIDI.sendNoteOff(69, 0, channel);
  usbMIDI.send_now();
  d(2000);
}

void d(unsigned long t){
  unsigned long s = millis();
  while( millis() - s < t ){
    while(usbMIDI.read());
  }
}
