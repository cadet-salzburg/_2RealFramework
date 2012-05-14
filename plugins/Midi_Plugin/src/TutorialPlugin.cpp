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

		metadata.addService("MidiInputService");
		metadata.setDescription("MidiInputService", "midi input");
		
		metadata.addService("MidiOutputService");
		metadata.setDescription("MidiOutputService", "midi output");



		metadata.addOutputSlot< int >("MidiInputService", "counter outlet");
		metadata.addOutputSlot< std::vector< unsigned char > >("MidiInputService", "test");
		metadata.addOutputSlot< std::vector< unsigned char > >("MidiOutputService", "outputmidi");

	
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
	context.registerService< MidiInputService >("MidiInputService");
	context.registerService< MidiOutputService >("MidiOutputService");
}

void TutorialPlugin::shutdown()
{
}

TutorialPlugin::~TutorialPlugin()
{
}

_2REAL_EXPORT_PLUGIN(TutorialPlugin)