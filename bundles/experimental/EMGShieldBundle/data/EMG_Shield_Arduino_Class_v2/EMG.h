/*
 * Copyright (c) 2012 by Dieter Steininger <dieter.steininger@aec.at>
 * EMG-Shield Class for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _EMG_H_INCLUDED
#define _EMG_H_INCLUDED




#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr\pgmspace.h>
#include <util/delay.h>
#include <Arduino.h>
#include <pins_arduino.h>


#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

#define EMG_MAX_CHANNELS 2

class EMG_Class 
{
	public:

	EMG_Class();     // Declaring a constructor
  
	// DECLARATION of METHODS
	void begin();
	void begin(int filterValue = 1);
	void end();
    void update();
	uint16_t read(int channel);
	byte readRawDataStream();
    
    void rawData(byte &LowByte, byte &HighByte, uint16_t &EMG_data, byte &channel);

    uint16_t dataMissCount;    
    
	private:
	
	class EMG_Channel
	{
		public:
		
		EMG_Channel();
        
        uint16_t moving_average(uint16_t input);
        
        uint16_t latestValue;
		int filter_value;               // filter_value (1-100)
		uint16_t index;	                // index for samples-filter-array
		uint16_t samples_EMG[101];      // samples-filter-array  
	};
	
	void setClockDivider(byte rate);
	byte master_Receive(void);
	
	// DECLARATION of VARIABLES
	EMG_Channel channels[EMG_MAX_CHANNELS];
};

extern EMG_Class EMG;

#endif
