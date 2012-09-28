// dinamyxel ax-12 servo test for Arduino 0017

//#include <SoftwareSerial.h>
#include <ax12.h>

//SoftwareSerial swSerial (2, 3);

// HACK:
// function SIGNAL(USART1_RX_vect) needs to be commented out in HardwareSerial.cpp (line 127 to line 131) in [Arduino Install Folder]\hardware\arduino\cores\arduino

// Command Syntax
// Byte
// 0 1 2         3         4          5          6          7
// X X ['0'|'1'|'2'] ['S'|'P'|'B'] ['0'..'9'] ['0'..'9'] ['0'..'9'] ['0'..'9']
//
// Byte 0 and 1 are snyc bytes
// Byte 2 is the id of the motor resp. LED
// Byte 3 is the command (either Speed or Position or Brightness (LED))
// Byte 4-7 is the parameter value as ascii string for the command. needs to be a number (like '0' '1' '0' '0' for 100)
// for Position the range is 0 (0 degree) to 1023 (300 degrees)
// for Speed the range is 1 to 1023 (114 rpm). 0 means maximim speed
// for LED Brightness the range is 0 (0V) to 1023 (5V)

#define numMotors 2

AX12 motor[numMotors] = { AX12 (), AX12 () };                 // define 2 motors AX12
int pinLED = 9;	// Digital pin number where LEDs are attached
/*
int numBytes = 0;
char inChar[8];
boolean msgDetected = false;
*/

void setup()   
{                
  Serial.begin (57600);          // Baud rate for HW Serial
  Serial.setTimeout(100*1000);
  AX12::init (1000000);                             // inicializa los AX12 a 1 Mb/s
  int detect[numMotors];                                    // array para detectar automáticamente las ID de 2 motores
  AX12::autoDetect (detect, numMotors);                     // detección de IDs
  for (byte i=0; i<numMotors; i++) 
  {
    motor[i].id = detect[i];                        // asigna las ID detectadas a los motores definidos previamente
  }
  motor_init();  
  
  // LED
  pinMode(pinLED, OUTPUT);
  analogWrite(pinLED, 25);
  
}

int str2int(String str)
{
  int val = 0;
  int pos = 0;
  for (int i=str.length()-1; i>=0; i--)
  {
    val += (str.charAt(i)-'0') * int(pow10(pos));
    pos++;
  }
  return  val;
}

// arduinos pow function is so inaccurate, that we have to make this hack here - grrrr!
int pow10(int ex)
{
  int ret = 1;
  for (int i=0; i<ex; i++)
  {
    ret *= 10;
  }
  return ret;  
} 

void loop()   
{
  char start[2];
  byte byteRead = Serial.readBytes(start, 2);
  if (byteRead == 2 && start[0] == 'X' && start[1] == 'X')
  {
    //Serial.println("Sync byte detected");    
    char idStr[2] = {0};
    byteRead = Serial.readBytes(idStr, 1);
    if (byteRead == 1)
    {
      //Serial.println("ID detected");
      int id = str2int(String(idStr));
      char command;
      byteRead = Serial.readBytes(&command, 1);
      if (byteRead == 1 && command2Code(command) != 0)
      {         
        //Serial.println("Command detected");        
        char paramStr[5] = {0};
        byteRead = Serial.readBytes(paramStr, 4);
        if (byteRead == 4)
        {
          //Serial.println("Parameter detected");          
          int param = str2int(String(paramStr)); 
          
          //Serial.println(id, DEC);
          //Serial.println(command2Code(command), DEC);
          //Serial.println(param, DEC);
          
          // id for LEDs brightness adjusting is always numMotors 
          // (e.g. 2 Motors have id0/1 --> LED has id 2)
          // command has to be 'B' ( = Brightness)
          if ( id == numMotors && command == 'B' ) {
            param = map(param,0,1023,0,255);
            analogWrite(pinLED,param);
          }
          else {
            motor[id].writeInfo(command2Code(command), param);
          }
        }
      }  
    }    
  }
}

byte command2Code(char command)
{
  switch (command)
  {
    case 'S':
      return MOVING_SPEED;
    case 'P':
      return GOAL_POSITION;
    case 'B':  // LED Brightness
      return 'B';
  }
  return 0;
}


void goTo(int val)
{
  Serial.print("setting goal to ");  Serial.println(val, DEC);
  motor[0].writeInfo (GOAL_POSITION, val);
  Serial.print ("  AX12 status "); Serial.println(motor[0].status_error, DEC);
}  

void waitAndMeassure(int ms)
{
  while (int i=ms > 0)
  {
    motor[0].readInfo(PRESENT_POSITION);
    Serial.print ("  AX12 status "); Serial.print(motor[0].status_error, DEC); Serial.print (" "); Serial.println(motor[0].status_data, DEC);
    delay(200);
    ms -= 200;
  }  
}

void motor_init () 
{
  for (int i=0; i<2; i++) 
  {
    motor[i].writeInfo (TORQUE_ENABLE, 1);               // habilita el torque
    motor[i].setEndlessTurnMode(false);                   // lo pone en modo de rotación continua
    motor[i].writeInfo (CW_COMPLIANCE_MARGIN, 3);
    motor[i].writeInfo (CCW_COMPLIANCE_MARGIN, 3);
    motor[i].writeInfo (CW_COMPLIANCE_SLOPE, 250);
    motor[i].writeInfo (CCW_COMPLIANCE_SLOPE, 250);
    motor[i].writeInfo (PUNCH, 100);
    motor[i].writeInfo (MAX_TORQUE, 1023);
    motor[i].writeInfo (LIMIT_TEMPERATURE, 85);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 60);
    motor[i].writeInfo (UP_LIMIT_VOLTAGE, 190);
    motor[i].writeInfo (RETURN_DELAY_TIME, 50);
    motor[i].writeInfo (MOVING_SPEED, 0xf);
    Serial.print("AX12 "); Serial.print(i);
    Serial.print(" id "); Serial.println(motor[i].id, DEC);
  }
}

