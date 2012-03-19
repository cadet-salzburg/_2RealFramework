#include "TutorialPlugin.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

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

		metadata.addService("Counter");
		metadata.setDescription("Counter", "count up starting with 1");
		// define the data type of the slot, the service name and the output name
		metadata.addOutputSlot< int >("Counter", "counter outlet");

		metadata.addService("Doubler");
		metadata.setDescription("Doubler", "multiply number by 2");
		metadata.addInputSlot< int >("Doubler", "number inlet");
		metadata.addOutputSlot< int >("Doubler", "doubler outlet");

		metadata.addService("PrintOut");
		metadata.setDescription("PrintOut", "print out a number to the console");
		 
		// the default value is optional, but if there is no default value and no 
		// input value, the update() function will not be executed. 
		// metadata.addInputSlot< int >("PrintOut", "input number");
		metadata.addInputSlot< int >("PrintOut", "input number", int(0));
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
	context.registerService< Counter >("Counter");
	context.registerService< Doubler >("Doubler");
	context.registerService< PrintOut >("PrintOut");
}

void TutorialPlugin::shutdown()
{
}

TutorialPlugin::~TutorialPlugin()
{
}

_2REAL_EXPORT_PLUGIN(TutorialPlugin)