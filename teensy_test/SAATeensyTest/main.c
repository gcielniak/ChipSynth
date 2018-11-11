/*
 * SAATeensyTest.c
 *
 * Created: 28/10/2018 17:27:34
 * Author : gcielniak
 */ 

#include <avr/io.h>
#include "MCU.h"

void ChipWrite(uint8_t chip, uint16_t address, uint8_t data);

int main(void) {

	MCUInit();
	
	//clock signal
	DDRB = _BV(7);

	if (0) {

	//clock timer
	//timer 0, clock wave generator
	TCCR0A = _BV(COM0A0) | _BV(WGM01);
	TCCR0B = _BV(CS00);
	OCR0A = 0;//output freq. of 8 MHz
	TCNT0 = 0;//output freq. of 8 MHz

} else {


	//timer 0, clock wave generator
	TCCR1A = _BV(COM1C0);
	TCNT1 = 0xFFFF;
	TCCR1B = _BV(WGM12) | _BV(CS10);
}

	//data port (C)
	DDRC = 0xFF;
	//chip control port (A0, CS, WR)
	DDRF = _BV(0)|_BV(1)|_BV(2);

	//init control signals (CS=0 all the time)	
	PORTF &= ~_BV(0)&~_BV(1); //A0=0, CS=0
	PORTF |= _BV(2); //WR=1
	
	for (uint8_t i = 0; i < 0x20; i++)
		ChipWrite(0, i, 0x00);
	
	ChipWrite(0, 0x1C, 0x02);//frequency reset
	ChipWrite(0, 0x1C, 0x01);//sound enable

	ChipWrite(0, 0x00, 0xFF);//volume0 left F right F
	ChipWrite(0, 0x10, 0x03);//octave0 6
	ChipWrite(0, 0x14, 0x01);//all freq enabled

	while (1) {
    }
}

void ChipWrite(uint8_t chip, uint16_t address, uint8_t data) {
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


