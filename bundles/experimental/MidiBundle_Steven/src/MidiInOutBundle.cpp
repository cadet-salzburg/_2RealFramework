#include "_2RealBundle.h"

#include "MidiInputBlock.h"
#include "MidiOutputBlock.h"

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "MidiInOutBundle" );
		info.setDescription( "Midi In and Out" );
		info.setAuthor( "Steven Stojanovic" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo midiIn = info.exportBlock< MidiInputBlock, WithContext >( "MidiInBlock" );
		midiIn.addInlet<unsigned int>( "MidiInPort", 0 );
		midiIn.addOutlet<unsigned char>( "MidiInMessage0" );
		midiIn.addOutlet<unsigned char>( "MidiInMessage1" );
		midiIn.addOutlet<unsigned char>( "MidiInMessage2" );
		midiIn.setDescription( "Midi In" );

		BlockMetainfo midiOut = info.exportBlock< MidiOutputBlock, WithContext >( "MidiOutBlock" );
		midiOut.addInlet<unsigned int>( "MidiOutPort", 0 );
		midiOut.addInlet<unsigned char>( "MidiOutMessage0", 'a' );
		midiOut.addInlet<unsigned char>( "MidiOutMessage1", 'a' );
		midiOut.addInlet<unsigned char>( "MidiOutMessage2", 'a' );
		midiOut.setDescription( "Midi Out" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}