
/*
This Arduino example code reads the raw high-low-byte data stream 
from an EMG-Shield and sends it via the Serial Port to the PC.
*/

// HEADERS
//--------------------------------------------------------------------

#include <EMG.h>      // Includes the EMG-Shield-Class


// DECLARATIONS
//--------------------------------------------------------------------
byte data;


// SETUP
//--------------------------------------------------------------------
void setup()
{

  EMG.begin();          // initialization
  Serial.begin(9600);
  
}

// MAIN-LOOP
//--------------------------------------------------------------------
void loop()
{
  
  data = EMG.readRawDataStream();  
  Serial.write(data);    // sending the raw hi-lo-data-bytes via serial port

}
