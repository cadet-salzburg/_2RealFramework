#pragma once

#include "_2realblock.h"

#include "Poco/Mutex.h"
#include "RtMidi.h"

#include <vector>
#include <string>

class MidiInOutDeviceManager : public _2Real::bundle::ContextBlock
{
public:
	MidiInOutDeviceManager();
	~MidiInOutDeviceManager();

	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	void							registerMidiInBlock();
	void							unregisterMidiInBlock();

	void							registerMidiOutBlock();
	void							unregisterMidiOutBlock();

	bool							bindMidiInDevice( const unsigned int deviceIdx );
	bool							bindMidiOutDevice( const unsigned int deviceIdx );
	void							unsbindMidiInDevice( const unsigned int deviceIdx );
	void							unsbindMidiOutDevice( const unsigned int deviceIdx );
	int								getFirstFreeMidiInDevice();
	int								getFirstFreeMidiOutDevice();
	unsigned int					getMidiInPortCount();
	unsigned int					getMidiOutPortCount();
	bool							isMidiInDeviceRunning( const unsigned int deviceIdx );
	bool							isMidiOutDeviceRunning( const unsigned int deviceIdx );
	
	std::vector<unsigned char>		getMidiInMessage( const unsigned int deviceIdx );
	void							sendMidiOutMessage( const unsigned int deviceIdx, std::vector<unsigned char> message );

private:
	void							initMidiInDeviceList();
	void							initMidiOutDeviceList();
	void							rescanMidiInPorts();
	void							rescanMidiOutPorts();
	bool							isMidiInDeviceAvailable( const unsigned int deviceIdx ) const;
	bool							isMidiOutDeviceAvailable( const unsigned int deviceIdx ) const;
	bool							isMidiInDeviceFree( const unsigned int deviceIdx ) const;
	bool							isMidiOutDeviceFree( const unsigned int deviceIdx ) const;
	void							clearMidiInList();
	void							clearMidiOutList();


	struct DeviceItem
	{
		DeviceItem::DeviceItem( std::string strDescription, bool bIsUsed ) :
			m_Midi ( 0 ),
			m_strDescription ( strDescription ),
			m_bIsUsed ( bIsUsed )
		{
			
		}
		
		RtMidi*							m_Midi;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	bool								m_bMidiInListIsUsed;
	bool								m_bMidiOutListIsUsed;

	int									m_iMidiInReferenceCount;
	int									m_iMidiOutReferenceCount;

	unsigned int						m_iNumMidiInDevices;
	unsigned int						m_iNumMidiOutDevices;

	std::vector< DeviceItem >			m_MidiInDevicesInUse;
	std::vector< DeviceItem >			m_MidiOutDevicesInUse;
	
	RtMidiIn*							m_MidiIn;
	RtMidiOut*							m_MidiOut;
	Poco::Mutex							m_Mutex;
};

