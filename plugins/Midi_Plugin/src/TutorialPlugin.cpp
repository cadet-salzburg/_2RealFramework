
#include "ServiceImpl.h"

#include "_2RealBundle.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

#include <vector>

using _2Real::BundleMetainfo;
using _2Real::BlockMetainfo;
using _2Real::Enumeration;
using _2Real::Enums;
using _2Real::Exception;

using std::vector;
using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo(BundleMetainfo info )
{
	try
	{
		info.setDescription("Midi Plugin");
		info.setAuthor("help@cadet.at");
		info.setVersion(0, 0, 0);
		info.setContact("help@cadet.at");

		BlockMetainfo midiInputService = info.exportBlock< MidiInputService>("MidiInputService");
		midiInputService.setDescription( "midi reading input" );
		midiInputService.addOutlet<vector< unsigned char > >( "inputmidi", vector<unsigned char>() );

		BlockMetainfo midiOutputService = info.exportBlock< MidiOutputService >( "MidiOutputService" );
		midiOutputService.setDescription( "midi reading output" );
		midiOutputService.addOutlet< vector< unsigned char > >( "outputmidi", vector<unsigned char>()  );
		 
	
	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during setup: " << e.what();
		 
	}
}
 