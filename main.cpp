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

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "nokia5110.h"
#include "i2c.h"

NOKIA5110 nokia5110;

#define PCF8575_SLA_W				0x40
#define PCF8575_SLA_R				PCF8575_SLA_W | 0x01

#define TRUE						1
#define FALSE						0				

#define PCF8575_PIN_0				0
#define PCF8575_PIN_1				1
#define PCF8575_PIN_2				2
#define PCF8575_PIN_3				3
#define PCF8575_PIN_4				4
#define PCF8575_PIN_5				5
#define PCF8575_PIN_6				6
#define PCF8575_PIN_7				7

void PCF8575_write(char p0_p7, char p10_p17)
{
	i2c_start();
	i2c_write(PCF8575_SLA_W);
	i2c_write(p0_p7);
	i2c_write(p10_p17);
	i2c_stop();
}

void PCF8575_read(char buffer[])
{
	i2c_start();
	i2c_write(PCF8575_SLA_R);
	buffer[0]	= i2c_ack();
	buffer[1]	= i2c_nack();
	i2c_stop();
}

void PCF8575_clear()
{
	PCF8575_write(0x00, 0x00);
};

void PCF8575_pin(char pin, char state)
{
	char buffer[2];
	
	PCF8575_read(buffer);
	
	char p0_p7	= buffer[0];
	char p10_17 = buffer[1];
	
	if (pin < 8)
	{
		if (!state)
		p0_p7 &=~(1<<pin);
		else
		p0_p7 |=(1<<pin);
	}
	else
	{
		pin = pin % 10;
		
		if (!state)
		p10_17 &=~(1<<pin);
		else
		p10_17 |=(1<<pin);
	}
	
	PCF8575_write(p0_p7, p10_17);

}

int main()
{
	i2c_init( F_CPU );
	
	nokia5110.Init();
	
	nokia5110.WriteString("PCF8575",0,0,Font_Arial_12x12);
	
	PCF8575_clear();
	
	PCF8575_pin(PCF8575_PIN_7, TRUE);
	
	while(1);
}