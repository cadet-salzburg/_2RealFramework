
// ####################################################################################
// EMG_Shield_v1_2_Software_v1_0.h
// ####################################################################################
//
// Author:		Dieter Steininger
// Edited:		10.06.2012 
//
// ####################################################################################
//
// This is the header-file for the first firmware for the EMG-Shield_v1_2.
// It is responsible for: 
//		- including all header-files
//		- including all function-prototypes with description
//
// ####################################################################################


// HEADERS
//-------------------------------------------------------------------------------------
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <math.h>



// FUNCTION-PROTOTYPES
//-------------------------------------------------------------------------------------

void delay (uint16_t delay_time_ms);   // delay in ms (possible uint16_t as input!!)

void init_blinks(void);  // does 3 initial LED-blinks

void blinky_invalid_calibration(uint8_t led); // fast blinking if calibration is invalid

void set_IOs(void);  // sets all inputs and outputs

void SPI_SlaveInit(void); // Initializes the µC as a slave for SPI-communication

void ADC_Init(void); // ADC-Initialization

// does the gain calibration of the selected
uint8_t gain_calibration(uint8_t channel_selection, uint8_t slave_address_w, float calibration_time);
			// channel (channel_selection = 0 or 1)
			// as output the calibration-status is returned
			// 0 ... not calibrated
			// 1 ... calibrated
			// 2 ... invalid calibration
					
uint16_t find_minimum(uint16_t *array);  // finds the average EMG-minimum (offset)
			
// does the whole minimum calibration
uint16_t minimum_calibration(uint8_t channel_selection, uint8_t min_calib_samples, float calib_time, float delay_min_calib);

void program_pot_TWI(uint8_t slave_address_w, uint8_t instruction_byte, uint8_t value);
	 // slave_address_w:	7bit + Write-Bit(Low) = (0 1 0 1 1 AD1 AD0 R/W)
     //						AD0/1: set to HIGH via hardware
	 // instruction_byte:	8bit = (A/B RS SD O1 O2 X X X) 
	 //						A/B: RDAC1(LOW), RDAC2(HIGH)
	 //						RS: midscale reset (active HIGH)
	 //						SD: shutdown (active HIGH)
	 //						O1/2: programmable Outputs
	 
void calibration_ch0(void);

void calibration_ch1(void);	

uint8_t toggle_ADC(void); // this function toggles between ADC0 and ADC1
					      // and returns the particular ADC_channel

uint16_t ADC_conversion(void); // does a single AD-conversion and returns digital-value "ADCW"

// does an offset correction and maps the signal again between 0 and 1023
uint16_t offset_correction(uint8_t ADC_channel, uint16_t EMG_signal_raw, uint16_t EMG_min_0, uint16_t EMG_min_1);

void data_masking(uint16_t EMG_signal, uint8_t ADC_channel);
		// divides 10-bit-data into 2x 5bit data-blocks
		// masks the 6th & 7th Bit: 01=data_lo-ID; 10=data_hi-ID 
		// masks the 8th Bit: 0=ADC0; 1=ADC1

void SPI_SlaveTransmit(uint8_t data); // writes 8-bit-data into the SPI-data-register (SPDR)
									  // and waits till the transmission is done!
									 

// unused prototypes
/*

void TIMER_Init(void); // Timer-Initialization for 16-Bit-Timer-Counter1
					   // It is used for creating interrupts to sample the signals
*/			
	   