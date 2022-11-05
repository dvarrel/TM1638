/*!
 * @file TM1638.h
 * @brief Arduino library for interface with TM1638 chip. 
 * @n read buttons, switch leds, display on 7segment.
 * @author [Damien](web@varrel.fr)
 * @version  V1.0
 * @date  2022-11-01
 * @url https://github.com/dvarrel/TM1638.git
 * @module https://fr.aliexpress.com/item/32832772646.html
 */

#ifndef _TM1638_H
#define _TM1638_H
#include "Arduino.h"

#ifndef ON
#define ON 1
#endif
#ifndef OFF
#define OFF 0
#endif

    typedef enum{
      PULSE1_16,
      PULSE2_16,
      PULSE4_16,
      PULSE10_16,
      PULSE11_16,
      PULSE12_16,
      PULSE13_16,
      PULSE14_16
    } pulse_t;

    typedef enum{
      S1,S2,S3,S4,
      S5,S6,S7,S8
    } button_t;

class TM1638{
  private:
    #define INSTRUCTION_WRITE_DATA 0x40
    #define INSTRUCTION_READ_KEY 0x42
    #define INSTRUCTION_ADDRESS_AUTO 0x40
    #define INSTRUCTION_ADDRESS_FIXED 0x44
    #define INSTRUCTION_NORMAL_MODE 0x40
    #define INSTRUCTION_TEST_MODE 0x48

    #define FIRST_DISPLAY_ADDRESS 0xC0

    #define DISPLAY_TURN_OFF 0x80
    #define DISPLAY_TURN_ON 0x88

    uint8_t _digits[16]={
      0b00111111,0b00000110,0b01011011,0b01001111,
      0b01100110,0b01101101,0b01111101,0b00000111,
      0b01111111,0b01101111,0b01110111,0b01111100,
      0b00111001,0b01011110,0b01111001,0b01110001
    };
        
    uint8_t _clk_pin;
    uint8_t _stb_pin;
    uint8_t _dio_pin;
    uint8_t _buttons;
    uint8_t _pulse;
    bool _isOn;

  public:
    TM1638(uint8_t clk_pin, uint8_t dio_pin, uint8_t stb_pin){
      _clk_pin = clk_pin;
      _stb_pin = stb_pin;
      _dio_pin = dio_pin;
      _pulse = PULSE1_16;
      _isOn = false;
      
      pinMode(stb_pin, OUTPUT);
      pinMode(clk_pin, OUTPUT);
      pinMode(dio_pin, OUTPUT);
      digitalWrite(stb_pin, HIGH);
      digitalWrite(clk_pin, HIGH);
      digitalWrite(dio_pin, HIGH);    
    }

    bool getButton(button_t s);
    uint8_t getButtons();

    /**
    * @fn writeLed
    * @brief put led ON or OFF
    * @param num num of led(1-8)
    * @param state (true or false)
    */
    void writeLed(uint8_t num, bool state);
    void writeLeds(uint8_t val);

    /**
    * @fn displayVal
    * @brief put value on 7 segment display
    * @param digitId num of digit(0-7)
    * @param val value(0->F)
    * @return Altitude in meter
    */
    void displayVal(uint8_t digitId, uint8_t val);
    void displayDig(uint8_t digitId, uint8_t pgfedcba);

    /**
    * @fn displayClear
    * @brief switch off all leds and segment display
    */
    void displayClear();

    /**
    * @fn displayTurnOff
    * @brief turn on lights
    */
    void displayTurnOff();

    /**
    * @fn displayTurnOn
    * @brief turn off lights
    */
    void displayTurnOn();

    void displaySetPulse(pulse_t pulse);
    void reset();
    void test();
  private:
    void writeData(uint8_t data);
    void writeDataAt(uint8_t displayAddress, uint8_t data);
    void setDisplayMode(uint8_t displayMode);
    void setDataInstruction(uint8_t dataInstruction);
};
#endif