/*
This Arduino example code reads the data from an EMG-Shield
and sends it via the Serial Port to the PC.
*/

// HEADERS
//--------------------------------------------------------------------

// IMPORTANT define the speed of your arduino in hertz here
#define F_CPU 16000000UL

#include <EMG.h>      // Includes the EMG-Shield-Class

// SETUP
//--------------------------------------------------------------------
void setup()
{

  EMG.begin(100);     // 1-100 is a moving-average filter_value
  Serial.begin(9600); 
  
}

// MAIN-LOOP
//--------------------------------------------------------------------
void loop()
{
  /*
  byte LowByte;
  byte HighByte;
  uint16_t EMG_data;
  byte channel;
  EMG.rawData(LowByte, HighByte, EMG_data, channel);

  Serial.print(LowByte, DEC); Serial.print(" ");
  Serial.print(HighByte, DEC); Serial.print(" ");
  Serial.print(EMG_data, DEC); Serial.print(" ");  
  Serial.println(channel, DEC);    // sending the data via serial port 
  */
    
  unsigned short data;
  EMG.update();

 /*
  Serial.println("---");    // sending the data via serial port 
 
  data = EMG.dataMissCount;
  Serial.println(data, DEC);    // sending the data via serial port 
*/

  data = EMG.read(0);           // 0 or 1 for channel-selection
  Serial.println(data, DEC);    // sending the data via serial port 

  data = EMG.read(1);           // 0 or 1 for channel-selection
  Serial.println(data, DEC);    // sending the data via serial port 
}
