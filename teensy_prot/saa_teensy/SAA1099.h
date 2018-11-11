#include "Arduino.h"

class SoundChip {

  public:
    SoundChip() {};

    void Init() {
  
      //clock signal
      DDRB = _BV(7);

      //timer 1, clock wave generator
      TCCR1A = _BV(COM1C0);
      TCNT1 = 0xFFFF;
      TCCR1B = _BV(WGM12) | _BV(CS10);      

      //data port (C)
      DDRC = 0xFF;
      //chip control port (A0, CS, WR)
      DDRF = _BV(0) | _BV(1) | _BV(2);

      //init control signals (CS=0 all the time)
      PORTF &= ~_BV(0) & ~_BV(1); //A0=0, CS=0
      PORTF |= _BV(2); //WR=1
    }

    void Reset(){
      for (uint8_t i = 0; i < 0x20; i++)
        Write(i, 0x00);

      Write(0x1C, 0x02);//frequency reset
      Write(0x1C, 0x01);//sound enable

      }

    void Write(unsigned short int address, unsigned short int data) {
      //address
      PORTF |= _BV(0);//A0=1
      PORTC = address;
      PORTF &= ~_BV(2);//WR=0
      //wait 100 ns (62.5*2 ns)
      asm volatile ("nop\n nop\n"::);
      PORTF |= _BV(2);//WR=1

      //data
      PORTF &= ~_BV(0);//A0=0
      PORTC = data;
      PORTF &= ~_BV(2);//WR=0
      //wait 100 ns (62.5*4 ns)
      asm volatile ("nop\n nop\n"::);
      PORTF |= _BV(2);//WR=1

      //wait 2 us for the registers to be updated (62.5*32)
      //the worst case scenario (frequency registers need 16 SAA clocks)
      asm volatile ("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"::);
      asm volatile ("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"::);
      asm volatile ("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"::);
      asm volatile ("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n"::);
    }
};
