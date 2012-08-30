/*
 * Copyright (c) 2012 by Dieter Steininger <dieter.steininger@aec.at>
 * EMG-Shield Class for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

// HEADERS
//---------------------------------------------------------------------------------------------------
#include "EMG.h"

// CLASS DECLARATIOMS
//---------------------------------------------------------------------------------------------------
EMG_Class EMG;

// Nested class EMG_Channel
//---------------------------------------------------------------------------------------------------
EMG_Class::EMG_Channel::EMG_Channel()
{
	// For Initialization
	filter_value = 1;  			  // sets the filter_value to a default value of 1
	index = 0;					  // initial value
    latestValue = 0;
}

uint16_t EMG_Class::EMG_Channel::moving_average(uint16_t input)  // moving-average-lowpass-filter
{
    samples_EMG[index++] = input;
    index %= filter_value;
    
    float average_EMG = 0;
    
    for (uint16_t i = 0; i < filter_value; i++) 
    {
        average_EMG += samples_EMG[i];
    }
    average_EMG /= filter_value;
    
    latestValue = ((uint16_t) round(average_EMG));
    
    return latestValue;
}

// CONSTRUCTOR
//---------------------------------------------------------------------------------------------------
EMG_Class::EMG_Class()
{
    dataMissCount = 0;
}

// EMG_Class METHODS
//---------------------------------------------------------------------------------------------------

void EMG_Class::setClockDivider(byte rate)
{
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

byte EMG_Class::master_Receive(void)
{
	digitalWrite(SS, LOW);		// selects the slave
    _delay_us(100);
	//delay(1);				    // important!
	
	SPDR = 1;
    while(!(SPSR & (1<<SPIF)));  // Wait for transmission complete
	
	digitalWrite(SS, HIGH);		// de-selects the slave

    return(SPDR);
}

void EMG_Class::begin(int filtervalue) 
{
	// SETTING everything for SPI-communication!
	//------------------------------------------------------------------
	// Set direction register for SCK and MOSI pin.
	// MISO pin automatically overrides to INPUT.
	// When the SS pin is set as OUTPUT, it can be used as
	// a general purpose output port (it doesn't influence
	// SPI operations).

	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(SS, OUTPUT);
  
	digitalWrite(SCK, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(SS, HIGH);

	// Warning: if the SS pin ever becomes a LOW INPUT then SPI 
	// automatically switches to Slave, so the data direction of 
	// the SS pin MUST be kept as OUTPUT.
	SPCR |= (1<<MSTR);
	SPCR |= (1<<SPE);

	// Set clock-divider
	setClockDivider(SPI_CLOCK_DIV64);   // sets the SPI-clock to 250kHz @ 16MHz

	if (filtervalue <= 0)     // sets filtervalue to a default of 1
        filtervalue = 1;
    
    if (filtervalue > 100)
		filtervalue = 100;    // limits the filter-value to 100 !!
	
	for (int i=0; i<EMG_MAX_CHANNELS; i++)
	{
		channels[i].filter_value = filtervalue;	
	}
}

void EMG_Class::end() 
{
	// terminates the SPI-communication
    SPCR &= ~_BV(SPE);
}

void EMG_Class::rawData(byte &LowByte, byte &HighByte, uint16_t &EMG_data, byte &channel)
{
	byte getByte = 0;
	
	do
	{
		// wait for a low_byte
		do
		{
			getByte = master_Receive();
		}
		while((getByte & (1<<5)) != (1<<5));
        
		LowByte = getByte;
		HighByte = master_Receive();
	} 
	while((LowByte & (1<<7)) != (HighByte & (1<<7)));
    
    channel = (LowByte>>7)&1;
    
    // de-masking of the data-bytes:
    LowByte = LowByte & 0b00011111;
    HighByte = HighByte & 0b00011111;
    
    // merge the 2 bytes to one 10-bit (16-bit) data-byte
    EMG_data = ((uint16_t) (HighByte << 5)) | ((uint16_t)LowByte); 
}

void EMG_Class::update()
{
	byte LowByte = 0;
	byte HighByte = 0;
	byte getByte = 0;
	uint16_t EMG_data = 0;
    
    dataMissCount = 0;
	
	do
	{
		// wait for a low_byte
		do
		{
			getByte = master_Receive();
            dataMissCount++;
		}
		while((getByte & (1<<5)) != (1<<5));
        dataMissCount--;
        
		LowByte = getByte;
		HighByte = master_Receive();
        dataMissCount++;
	} 
	while((LowByte & (1<<7)) != (HighByte & (1<<7)));
    dataMissCount--;
    
    byte channel = (LowByte>>7) & 1;
        
    // de-masking of the data-bytes:
    LowByte = LowByte & 0b00011111;
    HighByte = HighByte & 0b00011111;
    
    // merge the 2 bytes to one 10-bit (16-bit) data-byte
    EMG_data = ((uint16_t) (HighByte << 5)) | ((uint16_t)LowByte); 
        
    channels[channel].moving_average(EMG_data);
}

uint16_t EMG_Class::read(int channel)
{
	if (channel >= EMG_MAX_CHANNELS)	// chooses channel MAX_CHANNELS-1 if value is equal or higher MAX_CHANNELS
		channel = EMG_MAX_CHANNELS-1;
    else if (channel < 0)
        channel = 0;    // chooses channel 0 if value is lower 0
    
    return channels[channel].latestValue;
}

byte EMG_Class::readRawDataStream()
{
	return( master_Receive() );
}
