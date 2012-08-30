
// ####################################################################################
// EMG_Shield_v1_2_Software_v1_0.c
// ####################################################################################
//
// Author:		Dieter Steininger
// Edited:		10.06.2012 
// 
// ####################################################################################
//
// This is the first firmware for the EMG_Shield_v1_2.
// It is responsible for: 
//		- EMG-signal-calibration (via a TWI-resistor, 2 Buttons and 2 LED's)
//		- A/D-Conversion of two EMG-Signals 
//		- Offset correction
//		- Data masking
//		- EMG-Data-Transmission via SPI to a MASTER-Arduino
//
// ####################################################################################


// PREPROCESSOR DIRECTIVES
//-------------------------------------------------------------------------------------

// Constants
#define F_CPU 1000000UL     // do not forget to set the fuse-bit clock-division by 8
//#define F_SAMPLE 100 	    // sampling-rate  (for timer if used!)
#define MINIMUM_CALIBRATION_SAMPLES 100   // defines the number of measured minimum-samples
										  // maximum is 255 samples
#define CALIBRATION_TIME 3000  // defines the calibration time (3000 recommended)
#define DELAY_MIN_CALIB 1000   // at least 1000 (1000 recommended) 

// TWI-Status-Codes
#define START 0x08		 // START condition has been transmitted
#define MT_SLA_ACK 0x18  // SLA+W has been transmitted; ACK has been received
#define MT_DATA_ACK 0x28 // Data byte has been transmitted; ACK has been received

// Digital Potentiometer
#define SLAVE_ADDRESS_W 0b01011000   // addresses the digital potentiometer in write-mode

// HEADERS
//-------------------------------------------------------------------------------------
#include "EMG_Shield_v1_2_Software_v1_0.h"


// GLOBAL VARIABLES
//-------------------------------------------------------------------------------------

// EMG-data
volatile uint8_t data_lo = 0x00;				
volatile uint8_t data_hi = 0x00;	

// Calibration
volatile uint8_t invalid_calib_blink_cnt = 0;    
volatile uint16_t EMG_min_0 = 0;
volatile uint16_t EMG_min_1 = 0;
volatile uint8_t calib_status_ch0 = 0;
volatile uint8_t calib_status_ch1 = 0;	


// MAIN-PROGRAM
//-------------------------------------------------------------------------------------
int main()
{
  // internal variables
  //---------------------------------------------------------------------------------
  
  // ADC & EMG-data		
  uint8_t ADC_channel = 3;				// initial value=3, because 3 isn't used			
  uint16_t EMG_signal_raw = 0;
  uint16_t EMG_signal = 0;	
  
  
  // Initializations
  //---------------------------------------------------------------------------------
  set_IOs();
  SPI_SlaveInit();
  ADC_Init();
  program_pot_TWI(0b01011000, 0b00000000, 255); // programs to a min. between A & W
  program_pot_TWI(0b01011000, 0b10000000, 255); // programs to a min. between A & W
  //TIMER_Init();   // unused so far!
  init_blinks(); 
  
  //sei();          // activates interrupts global (if ISR is needed)
	
  
  // MAIN-Loop
  //---------------------------------------------------------------------------------
  while(1)
  {
	// CALIBRATION-Routine
	//-----------------------------------------------------------------------
	if( (~PIND & (1 << PIND6)) )      // if calibration-button 0 is pressed
	  calibration_ch0();  		
	  							
	else if( (~PIND & (1 << PIND4)) ) // if calibration-button 1 is pressed
	  calibration_ch1();				
		
	// if calibration of channel 0 was invalid --> fast blinking of LED 0
	if(calib_status_ch0 == 2)
	  blinky_invalid_calibration(0);
			
	// if calibration of channel 1 was invalid --> fast blinking of LED 1
	else if(calib_status_ch1 == 2)
	  blinky_invalid_calibration(1);
	//----------------------------------------------------------------------- 
	
	
	// Toggles the ADC-channel between ADC0/1 and returns ADC_channel
	ADC_channel = toggle_ADC();
	
	// Does one single AD-conversion
	EMG_signal_raw = ADC_conversion(); 
	
	// Does an offset correction
	EMG_signal = offset_correction(ADC_channel, EMG_signal_raw, EMG_min_0, EMG_min_1);		
	
	// Masking of the EMG-data
	data_masking(EMG_signal, ADC_channel);
	
	// Data-transmission via SPI
	SPI_SlaveTransmit(data_lo);		
	SPI_SlaveTransmit(data_hi);  
  }
}

// INTERRUPT-SERVICE-ROUTINE
//-------------------------------------------------------------------------------------

// unused !!
/*
ISR (TIMER1_COMPA_vect)   // ISR for Timer/Counter1 Compare A Match
{
  cli();	// de-activates Interrupts global
  PORTD |= (1 << PD3); // sets PB0 to 1 ... for testing
		
  PORTD &= ~(1 << PD3); // sets PB0 to 0 ... for testing
  sei();	// re-activates Interrupts global
}
*/

// FUNCTIONS
//-------------------------------------------------------------------------------------

void delay (uint16_t delay_time_ms)   // delay in ms (possible uint16_t as input!!)
{
  for (int i = 0; i < delay_time_ms; i++)
	_delay_ms(1); 
}  

void init_blinks(void)  // does 3 initial LED-blinks
{
  uint8_t i=0;
	
  while(i<6)
  {
	PORTD ^= (1 << PD5) | (1 << PD3);    // toggles LED0 & LED1
	_delay_ms(500);
	i++;			
  }
}

void blinky_invalid_calibration(uint8_t led)
{
  invalid_calib_blink_cnt++;
 
  if((invalid_calib_blink_cnt%20)==0)  
  {
	if (led == 0)
	  PORTD ^= (1 << PD5);   // toggles LED 0
	else if (led == 1)
	  PORTD ^= (1 << PD3);   // toggles LED 1
  }
}

void set_IOs(void) // setting digital I/O's
{
  // Setting Digital Inputs
  DDRD &= ~( (1 << DDD6) | (1 << DDD4));  // PD6= calib_button_0 , PD4= calib_button_1
  
  // Setting Digital Outputs
  DDRD |= (1 << DDD5) | (1 << DDD3);  // PD5= calib_LED_0 , PD3= calib_LED_1
  // DDRB |= (1 << DDB0);  // for testing!!
}

void SPI_SlaveInit(void) // SPI-Slave-Initialization
{
  DDRB |= (1<<DDB4);      // Sets MISO as an output
  DDRB &= ~((1<<DDB2)|(1<<DDB3)|(1<<DDB5));   // Set /SS, MOSI, SCK as an input
  SPCR = (1<<SPE);	// Enables SPI, all other BITS are set to zero (see SPCR-register)
}

void ADC_Init(void) // ADC-Initialization
{
  ADMUX  |= (1<<REFS0);             // Reference voltage = AVCC
  ADCSRA |= (1<<ADEN);		        // activates the ADC
  // ADC-Prescaler --> 2 (=successive rate = 500kHz @ 1MHz CPU_rate)
  ADCSRA &= ~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));  // conversion-time = 13 CLK-cycles
  ADMUX  &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0));  // Initial choice of ADC-channel-0 (ADC0)
}

uint8_t toggle_ADC(void)
{
  uint8_t ADC_channel = 3;  // initial value

  ADMUX ^= (1<<MUX0);  // toggles MUX0 (toggle between ADC0, ADC1)
  if (ADMUX & (1 << MUX0))		// if MUX0=1 --> ADC1 active
	ADC_channel = 1;
		
  else if ((~ADMUX & (1 << MUX0)))  // if MUX0=0 --> ADC0 active
	ADC_channel = 0;		
		
  return(ADC_channel);
}

uint16_t ADC_conversion(void) // does a single AD-conversion and returns ADCW
{
  ADCSRA |= (1<<ADSC);	    // starts AD-Conversion
  while(ADCSRA &(1<<ADSC));	// waits until AD-conversion done
  return(ADCW & 0b0000001111111111);  // assures a 10-bit value
}

uint16_t offset_correction(uint8_t ADC_channel, uint16_t EMG_signal_raw, uint16_t EMG_min_0, uint16_t EMG_min_1)
{
  float EMG_offset_free;
  uint16_t EMG_signal_scaled;
  uint16_t EMG_min = 0;
	
  // chooses the right Minimum-value
  if (ADC_channel == 0)
	EMG_min = EMG_min_0;
	
  else if (ADC_channel == 1)
	EMG_min = EMG_min_1;
	
  // Guaranties that the EMG-signal isn't beneath EMG_min 
  if(EMG_signal_raw < EMG_min)
	EMG_signal_raw = EMG_min;
			
  EMG_offset_free = ((float) EMG_signal_raw) - ((float) EMG_min);	
  
  // Scaling the EMG-signal between 0 and 1023
  EMG_signal_scaled = (uint16_t) roundf((EMG_offset_free / ((float) (1023 - EMG_min)))*1023);

  return(EMG_signal_scaled);
}

void data_masking(uint16_t EMG_signal, uint8_t ADC_channel)
{
  // divides 10-bit-data into 2x 5bit data-blocks and masks the 6th & 7th Bit = lo/hi-ID
  data_lo = ( ((uint8_t) EMG_signal) & 0b00011111 ) | 0b00100000;
  data_hi = ( ((uint8_t) (EMG_signal >> 5)) & 0b00011111 ) | 0b01000000; 
	
  // masks the 8th Bit= ADC0/ADC1 
  if (ADC_channel == 1)		// if ADC1 = active
  {
	//PORTB ^= (1<<PB0); // toggles PB0 for testing!!
	data_lo = data_lo | 0b10000000;	
	data_hi = data_hi | 0b10000000;				
  }
  else if (ADC_channel == 0)  // if ADC0 = active
  {
	data_lo = data_lo & 0b01111111;	
	data_hi = data_hi & 0b01111111;
  }						
}

void SPI_SlaveTransmit(uint8_t data)
{
  SPDR = data;	   // write data to the SPDR-register
  while(!(SPSR & (1<<SPIF)));  // Wait for transmission complete
}

// Calibration functions
uint8_t gain_calibration(uint8_t channel_selection, uint8_t slave_address_w, float calibration_time)
{
  uint8_t instruction_byte = 0b00000000;   // initial-value
  uint16_t EMG_calibration_signal = 0;
  uint8_t gain_value = 255;               // for minimum resistor between A & B
  uint8_t calibration_status = 0;
  uint8_t max_gain = 0;
	
  if (channel_selection == 0)
  {
	ADMUX &= ~(1<<MUX0);  // selects ADC-Channel 0	
	instruction_byte = 0b00000000;  // selects RDAC 1
  }			
  else if (channel_selection == 1)
  {	
	ADMUX |= (1<<MUX0);  // selects ADC-Channel 1
	instruction_byte = 0b10000000;  // selects RDAC 2			
  }
  		
  while(EMG_calibration_signal < 1000 && max_gain != 1)
  {
	program_pot_TWI(slave_address_w, instruction_byte, gain_value);
		
	// does one single AD-conversion of the selected ADC-Channel
	EMG_calibration_signal = ADC_conversion(); 
		
	delay((uint16_t) roundf(calibration_time/256));  // sets the calibration-time
		
	gain_value--;   // decrements gain_value --> increases the resistance between A & W
		
	if (gain_value == 255)  // if an overflow occurs
	  max_gain = 1;
  }
	
  gain_value++; // to get the last programmed gain-value
	
  delay((uint16_t) roundf((calibration_time/256)*((float) gain_value)));  // delays to full calibration-time		

  // sets calibration_status = 2, if the EMG-signal wasn't reached & sets gain to MINIMUM
  if (EMG_calibration_signal < 1000) // not total maximum, because this is not always reached
  {
	calibration_status = 2;
	program_pot_TWI(slave_address_w, instruction_byte, 255);		
  }
  // sets calibration_status = 1, if calibration was successful
  else if (EMG_calibration_signal >= 1000)
	calibration_status = 1;
	
  return(calibration_status);
}

uint16_t find_minimum(uint16_t *array)  // finds the average EMG-minimum (offset)
{
  // edited for finding average minimum!!
  
  uint16_t length = (uint16_t) (sizeof(array));   // establish size of array
  //uint16_t EMG_min = array[0];       // start with EMG_min = first element
  float average_EMG_min = 0;
      
  for(uint16_t i=0; i<length; i++)
  {
	/*
	if(array[i] < EMG_min)
      EMG_min = array[i];
	*/
	average_EMG_min = average_EMG_min + ((float) array[i]);  
  }
  
  average_EMG_min = average_EMG_min / ((float) length);
  
  return((uint16_t) roundf(average_EMG_min)); 
}

uint16_t minimum_calibration(uint8_t channel_selection, uint8_t min_calib_samples, float calib_time, float delay_min_calib)
{
  uint16_t EMG_min_samples[min_calib_samples+1]; // important for enough array-space
  uint8_t instruction_byte;
	  
  if (channel_selection == 0)
  {
	ADMUX &= ~(1<<MUX0);  // selects ADC-Channel 0	
	instruction_byte = 0b00000000;  // selects RDAC 1
  }			
  else if (channel_selection == 1)
  {	
	ADMUX |= (1<<MUX0);  // selects ADC-Channel 1
	instruction_byte = 0b10000000;  // selects RDAC 2			
  }
	  
  // storing a number of samples to EMG_min_samples[]
  for(uint16_t cnt=0; cnt<min_calib_samples; cnt++)
  {
	// does one single AD-conversion of the selected ADC-Channel
	EMG_min_samples[cnt] = ADC_conversion(); 

	delay( (uint16_t) roundf((calib_time-delay_min_calib)/((float) min_calib_samples)) );  
  }
      
  return(find_minimum(EMG_min_samples));
}

void program_pot_TWI(uint8_t slave_address_w, uint8_t instruction_byte, uint8_t value)
{
  // no special setup:
  // TWBR = 0, Prescaler = 0
  // SCL_clock = F_CPU/( 16 + ( 2*(TWBR)*PrescalerValue )) (see datasheet page 224)
  // in this case: SCL_clock = 62500 bit/s
	
  // Send START condition
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
  // Wait for TWINT Flag set. This indicates that the START 
  // condition has been transmitted
  while (!(TWCR & (1<<TWINT)));
		
  // Load slave_address_w into TWDR Register. Clear TWINT bit in 
  // TWCR to start transmission of address
  TWDR = slave_address_w;
  TWCR = (1<<TWINT) | (1<<TWEN);
	
  // Wait for TWINT Flag set. This indicates that slave_address_w 
  // has been transmitted, and ACK/NACK has been received.
  while (!(TWCR & (1<<TWINT)));
	
  // Load instr.byte into TWDR Register. Clear TWINT bit in TWCR to 
  // start transmission of data
  TWDR = instruction_byte;
  TWCR = (1<<TWINT) | (1<<TWEN);	
	
  // Wait for TWINT Flag set. This indicates that the DATA has been 
  // transmitted, and ACK/NACK has been received.
  while (!(TWCR & (1<<TWINT)));	
	
  // Load data into TWDR Register. Clear TWINT bit in TWCR to 
  // start transmission of data
  TWDR = value;
  TWCR = (1<<TWINT) | (1<<TWEN);	
	
  // Wait for TWINT Flag set. This indicates that the DATA has been 
  // transmitted, and ACK/NACK has been received.
  while (!(TWCR & (1<<TWINT)));
		
  // Transmit STOP condition
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	
}

void calibration_ch0(void)
{
  calib_status_ch0 = gain_calibration(0, SLAVE_ADDRESS_W, CALIBRATION_TIME);  // 0... for ADC-channel 0
			
  if(calib_status_ch0 != 2)
  {
	PORTD |= (1 << PD5);  // flashes up LED 0
	_delay_ms(DELAY_MIN_CALIB);    // delay before measuring the relaxed EMG-value
				
	// executes the minimum_calibration loop,... first "0" is for ADC-channel 0
	EMG_min_0 = minimum_calibration(0, MINIMUM_CALIBRATION_SAMPLES, CALIBRATION_TIME, DELAY_MIN_CALIB);
			
	PORTD &= ~(1 << PD5);  // switching off LED0 indicates that calibration is finished!
  }	
}

void calibration_ch1(void)
{
  calib_status_ch1 = gain_calibration(1, SLAVE_ADDRESS_W, CALIBRATION_TIME);  // 1... for ADC-channel 1
			
  if(calib_status_ch1 != 2)
  {
	PORTD |= (1 << PD3);  // flashes up LED 1
	_delay_ms(DELAY_MIN_CALIB);    // delay before measuring the relaxed EMG-value
				
	// executes the minimum_calibration loop,... first "1" is for ADC-channel 1
	EMG_min_1 = minimum_calibration(1, MINIMUM_CALIBRATION_SAMPLES, CALIBRATION_TIME, DELAY_MIN_CALIB);
			
	PORTD &= ~(1 << PD3);  // switching off LED1 indicates that calibration is finished!
  }				
}


// unused!!
//-----------
/* 

void TIMER_Init(void) // Timer-Initialization for 16-Bit-Timer-Counter1
{
  //Normal port operation, OC1A/OC1B disconnected.
  TCCR1A &= ~(1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0); 
  TCCR1B |= (1<<WGM12);    // Enables the CTC-Mode
  TCCR1B |= (1<<CS11);     // Prescaler = 8 
  // sets the Compare Match Register via F_ABTAST & Prescaler (see the row before)
  OCR1A = (F_CPU/(8*F_SAMPLE))-1;  
  TIMSK1 |= (1<<OCIE1A);   // activate Interrupt @ Compare-Match of 16-bit-Timer/Counter1
}


*/
