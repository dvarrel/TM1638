#include <TM1638.h>

//choose digital pins compatibles with your board
#define STB A0 // Strobe digital pin
#define CLK A1 // clock digital pin
#define DIO A2 // data digital pin

TM1638 tm(CLK,DIO,STB);

void setup() {
  Serial.begin(115200);
  tm.reset();
  tm.test();
}

uint8_t pulse=PULSE1_16;

void loop() {
  const uint8_t text[]={0x7c,0x1c,0x78,0x78,0x5c,0x54};
  for (uint8_t i=0;i<sizeof(text);i++)
    tm.displayDig(7-i, text[i]);

  uint8_t buttons = tm.getButtons();
  tm.writeLeds(buttons);

  static uint32_t timer = millis();
  
  if ( millis()-timer > 1000){
    timer=millis();
    tm.displaySetBrightness(pulse);
    pulse = (pulse==PULSE1_16) ? PULSE14_16 : PULSE1_16;
  }
  
}