/*
	@File 		nokia5110.cpp
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		07/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/ 

#include "nokia5110.h"

#include <avr/io.h>

void NOKIA5110::spi_write(const char data)
{
	int i = 0;
		
	for (i=7; i>=0; i--)
	{
		NOKIA5110_SCK_CLEAR;
			
		if(data & (1 << i))
		NOKIA5110_SDI_SET;
		else
		NOKIA5110_SDI_CLEAR;
			
		NOKIA5110_SCK_SET;
	}
}

void NOKIA5110::Cmd(unsigned char cmd)
{
	NOKIA5110_DC_CLEAR;
	NOKIA5110_SCE_CLEAR;
	
	NOKIA5110::spi_write(cmd);

	NOKIA5110_DC_SET;
	NOKIA5110_SCE_SET;
}

void NOKIA5110::Char(unsigned char data)
{
	NOKIA5110_DC_SET;
	NOKIA5110_SCE_CLEAR;
	
	NOKIA5110::spi_write(data);

	NOKIA5110_SCE_SET;
}

void NOKIA5110::Set_Cursor(unsigned char x, unsigned char y)
{
	NOKIA5110::Cmd(0x40 | y);
	NOKIA5110::Cmd(0x80 | x);
}

void NOKIA5110::WriteChar(unsigned char data, const unsigned char Font[])
{
	unsigned char wd				= pgm_read_byte( &Font[0] );	// font width
	unsigned char pg				= pgm_read_byte( &Font[2] );	// font pages
	unsigned char st				= pgm_read_byte( &Font[3] );	// font start
	unsigned char ch				= 0;							// character
	unsigned char fh				= 5;							// font header size
	unsigned char cl				= 0;							// col
	unsigned int i					= 0;							// width * width ...
	unsigned int rw					= 0;							// rows
	
	int id = ( ( ( data - st) * ( (wd+1) * 2) + fh ) );
	
	unsigned char character_width = pgm_read_byte( &Font[ id ] );
	
	Set_Cursor(X, Y);
	
	for (cl=0; cl<pg; cl++)
	{
		Set_Cursor(X, Y + cl);
		
		for (; rw < (wd + i); rw++)
		{
			ch = pgm_read_byte( &Font[ (id + rw) + 2 ] );
			
			Char( ch );
		}
		
		i += wd;
	}
	
	X += character_width + 1;
}

void NOKIA5110::WriteString(const char * data, unsigned char x, unsigned char y, const unsigned char Font[])
{
	NOKIA5110::X = x;
	NOKIA5110::Y = y;
	
	while(*data)
	{
		NOKIA5110::WriteChar(*data++, Font);
	}
}

void NOKIA5110::Init()
{
	DDRB |= (1<<NOKIA5110_MOSI);
	DDRB |= (1<<NOKIA5110_SCK);
	DDRB |= (1<<NOKIA5110_SCE);
	DDRB |= (1<<NOKIA5110_DC);
	DDRB |= (1<<NOKIA5110_RS);
	
	NOKIA5110_SCE_SET;
	
	NOKIA5110::Cmd(0x21);  	// command set in addition mode
	NOKIA5110::Cmd(0xC0);  	// set the voltage by sending C0 means VOP = 5V
	NOKIA5110::Cmd(0x07);  	// set the temp. coefficient to 3
	NOKIA5110::Cmd(0x13);  	// set value of Voltage Bias System
	NOKIA5110::Cmd(0x20);  	// command set in basic mode
	NOKIA5110::Cmd(0x0C);  	// display result in normal mode
}
