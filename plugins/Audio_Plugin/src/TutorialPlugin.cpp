 
 
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

#include "ServiceImpl.h"

using namespace _2Real;
using namespace std;

void getBundleMetainfo( BundleMetainfo info )
{
	try
	{
		info.setDescription("Audio Plugin");
		info.setAuthor("help@cadet.at");
		info.setVersion(0, 0, 0);
		info.setContact("help@cadet.at");

		BlockMetainfo audioplugin = info.exportBlock< AudioOutputService >( "AudioOutputService" );
		audioplugin.setDescription( "audio plugin" );
		audioplugin.addOutlet< std::vector< unsigned char >  >( "test", std::vector< unsigned char >() );
	 
		 
		 

	
	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during setup: " << e.what();
		throw PluginException(msg.str());
	}
}

  