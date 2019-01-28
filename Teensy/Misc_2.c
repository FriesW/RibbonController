const int channel = 1;

#define led 13
#define msbr 16
#define lsbr 48

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led, HIGH);
  usbMIDI.sendControlChange(msbr, 0, channel);
  usbMIDI.sendControlChange(lsbr, 0, channel);
  
  unsigned int msb_last = 0;
  
  for(unsigned int i = 0; i < 16384; i+=8){
    byte msb = i >> 7;
    byte lsb = i & 0b1111111;
    if(msb_last != msb){
        usbMIDI.sendControlChange(msbr, msb, channel);
        usbMIDI.send_now();
    }
    msb_last = msb;
    usbMIDI.sendControlChange(lsbr, lsb, channel);
    d(5);
  }
  
  digitalWrite(led, LOW);
  d(1000);
}

void d(unsigned long t){
  unsigned long s = millis();
  while( millis() - s < t ){
    while(usbMIDI.read());
  }
}
