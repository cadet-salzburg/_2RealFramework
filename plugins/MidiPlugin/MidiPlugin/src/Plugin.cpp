#include "Plugin.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

#include "ServiceImpl.h"

using namespace _2Real;
using namespace std;

void  MidiPlugin::getMetadata(Metadata &metadata)
{
	try
	{
		metadata.setDescription("MidiPlugin");
		metadata.setAuthor("help@cadet.at");
		metadata.setVersion(0, 0, 1);
		metadata.setContact("help@cadet.at");

		metadata.addService("Service 1");
		metadata.setDescription("Service 1", "description");
		// define the data type of the slot, the service name and the output name
	 
		metadata.addOutputSlot<  vector<int> >("Service 1", "m_ReadBytes");
		metadata.addOutputSlot< double >("Service 1", "m_TimeStamp");

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

void MidiPlugin::setup(PluginContext &context)
{
	context.registerService< Service1 >("Service 1");
}

void MidiPlugin::shutdown()
{
}

MidiPlugin::~MidiPlugin()
{
}

_2REAL_EXPORT_SINGLETON(MidiPlugin)




