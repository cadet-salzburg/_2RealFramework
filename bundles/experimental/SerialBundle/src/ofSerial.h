#pragma once

// serial error codes
#define OF_SERIAL_NO_DATA 	-2
#define OF_SERIAL_ERROR		-1

// find the system
#if defined( __WIN32__ ) || defined( _WIN32 )
	#define TARGET_WIN32
#elif defined( __APPLE_CC__)
	#include <TargetConditionals.h>

	#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define TARGET_OF_IPHONE
		#define TARGET_OPENGLES
	#else
		#define TARGET_OSX
	#endif
#elif defined (ANDROID)
	#define TARGET_ANDROID
	#define TARGET_OPENGLES
#else
	#define TARGET_LINUX
#endif

#if defined( TARGET_OSX ) || defined( TARGET_LINUX ) || defined (TARGET_ANDROID)
	#include <termios.h>
#else
	#include <windows.h>
	#include <tchar.h>
	#include <iostream>
	#include <string.h>
	#include <setupapi.h>
	#include <regstr.h>
	#define MAX_SERIAL_PORTS 256
	 #include <winioctl.h>
	#ifdef __MINGW32__
			#define INITGUID
			#include <initguid.h> // needed for dev-c++ & DEFINE_GUID
	#endif
#endif

#include <vector>
using namespace std;

class ofSerialDeviceInfo{
	friend class ofSerial;

	public:

		ofSerialDeviceInfo(string devicePathIn, string deviceNameIn, int deviceIDIn){
			devicePath			= devicePathIn;
			deviceName			= deviceNameIn;
			deviceID			= deviceIDIn;
		}

		ofSerialDeviceInfo(){
			deviceName = "device undefined";
			deviceID   = -1;
		}

		string getDevicePath(){
			return devicePath;
		}

		string getDeviceName(){
			return deviceName;
		}

		int getDeviceID(){
			return deviceID;
		}

	protected:
		string devicePath;			//eg: /dev/tty.cu/usbdevice-a440
		string deviceName;			//eg: usbdevice-a440 / COM4
		int deviceID;				//eg: 0,1,2,3 etc

		//TODO: other stuff for serial ?
};

//----------------------------------------------------------------------
class ofSerial {

	public:
			ofSerial();
			virtual ~ofSerial();

			void			listDevices();
							
			vector <ofSerialDeviceInfo> getDeviceList();

			void 			close();
			bool			setup();	// use default port, baud (0,9600)
			bool			setup(string portName, int baudrate);
			bool			setup(int deviceNumber, int baudrate);


			int 			readBytes(unsigned char * buffer, int length);
			int 			writeBytes(unsigned char * buffer, int length);
			bool			writeByte(unsigned char singleByte);
			int             readByte();  // returns -1 on no read or error...
			void			flush(bool flushIn = true, bool flushOut = true);
			int				available();

			void            drain();
	
			bool 			bVerbose;
			void 			setVerbose(bool bLoudmouth) { bVerbose = bLoudmouth; };

	protected:
			void			buildDeviceList();
						
			string			deviceType;
			std::vector <ofSerialDeviceInfo> devices;

			bool bHaveEnumeratedDevices;

			bool 	bInited;

			#ifdef TARGET_WIN32

				char 		** portNamesShort;//[MAX_SERIAL_PORTS];
				char 		** portNamesFriendly; ///[MAX_SERIAL_PORTS];
				HANDLE  	hComm;		// the handle to the serial port pc
				int	 		nPorts;
				bool 		bPortsEnumerated;
				void 		enumerateWin32Ports();
				COMMTIMEOUTS 	oldTimeout;	// we alter this, so keep a record

			#else
				int 		fd;			// the handle to the serial port mac
				struct 		termios oldoptions;
			#endif

};

//----------------------------------------------------------------------




// this serial code contains small portions of the following code-examples:
// ---------------------------------------------------
// http://todbot.com/arduino/host/arduino-serial/arduino-serial.c
// web.mac.com/miked13/iWeb/Arduino/Serial%20Write_files/main.cpp
// www.racer.nl/docs/libraries/qlib/qserial.htm
// ---------------------------------------------------

// notes:
// ----------------------------
// when calling setup("....") you need to pass in
// the name of the com port the device is attached to
// for example, on a mac, it might look like:
//
// 		setup("/dev/tty.usbserial-3B1", 9600)
//
// and on a pc, it might look like:
//
// 		setup("COM4", 9600)
//
