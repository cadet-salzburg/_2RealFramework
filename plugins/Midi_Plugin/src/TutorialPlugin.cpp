#include "TutorialPlugin.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

#include "ServiceImpl.h"

using namespace _2Real;
using namespace std;

void TutorialPlugin::getMetadata(Metadata &metadata)
{
	try
	{
		metadata.setDescription("Plugin 1");
		metadata.setAuthor("help@cadet.at");
		metadata.setVersion(0, 0, 0);
		metadata.setContact("help@cadet.at");

		metadata.addService("Service1");
		metadata.setDescription("Service1", "count up starting with 1");
		metadata.addOutputSlot< int >("Service1", "counter outlet");
		metadata.addOutputSlot< std::vector< unsigned char > >("Service1", "test");

	
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

void TutorialPlugin::setup(PluginContext &context)
{
	context.registerService< Service1 >("Service1");
	 
}

void TutorialPlugin::shutdown()
{
}

TutorialPlugin::~TutorialPlugin()
{
}

_2REAL_EXPORT_PLUGIN(TutorialPlugin)