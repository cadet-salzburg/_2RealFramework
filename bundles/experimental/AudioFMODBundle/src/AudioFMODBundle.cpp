#include "FMODContextManager.h"
#include "FMODSoundrenderBlock.h"
#include "_2RealBundle.h"
#include <sstream>
#include <iostream>

using namespace _2Real;

using namespace _2Real::bundle;
using _2Real::Exception;
using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName("AudioFMODBundle");
		info.setDescription( "FMOD Soundsystem handling audio in/output streams" );
		info.setAuthor( "Robert Sommeregger" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock<FMODContextManager>();
		BlockMetainfo metaInfo = info.exportBlock<FMODSoundrenderBlock, WithContext>( "FMODSoundrenderBlock" );

		metaInfo.addInlet<bool>( "Play", false );
		metaInfo.addInlet<std::string>( "FilePath", "" );
		metaInfo.addOutlet<bool>( "HasLoadedAudio" );


		metaInfo.setDescription( "FMOD Sound System" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}