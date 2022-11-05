#include "Arduino.h"
#include "TM1638.h"

bool TM1638::getButton(button_t s){
  _buttons = getButtons();
  return bitRead(_buttons, s);
}

// buttons K3/KS1-8
uint8_t TM1638::getButtons(){
  digitalWrite(_stb_pin, LOW);
  writeData(INSTRUCTION_READ_KEY);
  //Twait 1µs
  pinMode(_dio_pin, INPUT);
  digitalWrite(_clk_pin, LOW);
  uint8_t data[4];
  for (uint8_t i=0; i<sizeof(data);i++){
    data[i] = shiftIn(_dio_pin, _clk_pin, LSBFIRST);
    delayMicroseconds(1);
  }
  pinMode(_dio_pin, OUTPUT);
  digitalWrite(_stb_pin, HIGH);
  _buttons=0;
  for (uint8_t i=0; i<4;i++){
    _buttons |= data[i]<<i;
  }
  return _buttons;
}

void TM1638::reset(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_AUTO);
  digitalWrite(_stb_pin, LOW);
  writeData(FIRST_DISPLAY_ADDRESS);
  for(uint8_t i=0;i<16;i++)
    writeData(0);
  digitalWrite(_stb_pin, HIGH);
}

void TM1638::displayVal(uint8_t digitId, uint8_t val){
  if (digitId>7 | val>15 | val<0) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS+14-(digitId*2), _digits[val]);
}

void TM1638::displayDig(uint8_t digitId, uint8_t pgfedcba){
  if (digitId>7) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS+14-(digitId*2), pgfedcba);
}

void TM1638::displayClear(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  for (uint8_t i=0;i<15;i+=2){
    writeDataAt(FIRST_DISPLAY_ADDRESS+i,0x00);
  }
}

void TM1638::writeLed(uint8_t num,bool state){
  if (num<1 |num>8) return;
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  writeDataAt(FIRST_DISPLAY_ADDRESS + (num*2-1), state);
}

void TM1638::writeLeds(uint8_t val){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  setDataInstruction(INSTRUCTION_WRITE_DATA | INSTRUCTION_ADDRESS_FIXED);
  for(uint8_t i=1;i<9;i++){
    writeDataAt(FIRST_DISPLAY_ADDRESS + (i*2-1), val & 0x01);
    val >>= 1; 
  }
}

void TM1638::displayTurnOn(){
  setDisplayMode(DISPLAY_TURN_ON | _pulse);
  _isOn = true;
}

void TM1638::displayTurnOff(){
  setDisplayMode(DISPLAY_TURN_OFF | _pulse);
  _isOn = false;
}

void TM1638::displaySetBrightness(pulse_t newpulse){
  if (newpulse<PULSE1_16 | newpulse>PULSE14_16) return;
  _pulse = newpulse;
  uint8_t data = (_isOn) ? DISPLAY_TURN_ON : DISPLAY_TURN_OFF;
  data |= _pulse;
  setDisplayMode(data);
}

void TM1638::writeData(uint8_t data){
  shiftOut(_dio_pin,_clk_pin,LSBFIRST,data);
} 

void TM1638::writeDataAt(uint8_t displayAddress, uint8_t data){
    digitalWrite(_stb_pin, LOW);
    writeData(displayAddress);
    writeData(data);
    digitalWrite(_stb_pin, HIGH);
    delayMicroseconds(1);
}

void TM1638::setDisplayMode(uint8_t displayMode){
  digitalWrite(_stb_pin, LOW);
  writeData(displayMode);
  digitalWrite(_stb_pin, HIGH);
  delayMicroseconds(1);
}
void TM1638::setDataInstruction(uint8_t dataInstruction){
  digitalWrite(_stb_pin, LOW);
  writeData(dataInstruction);
  digitalWrite(_stb_pin, HIGH);
  delayMicroseconds(1);  
}

void TM1638::test(){
  uint8_t val=0;
  for(uint8_t i=0;i<5;i++){
    setDisplayMode(DISPLAY_TURN_ON | _pulse);
    setDataInstruction(INSTRUCTION_WRITE_DATA| INSTRUCTION_ADDRESS_AUTO);
    digitalWrite(_stb_pin, LOW);
    writeData(FIRST_DISPLAY_ADDRESS);
    for(uint8_t i=0;i<16;i++)
      writeData(val);
    digitalWrite(_stb_pin, HIGH);
    delay(1000);
    val = ~val;
  }

}

