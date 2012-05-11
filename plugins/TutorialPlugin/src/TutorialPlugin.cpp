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

		metadata.addService("Counter");
		metadata.setDescription("Counter", "count up starting with 1");
		metadata.addOutputSlot< int >("Counter", "counter outlet");
		metadata.addOutputSlot< std::vector< int > >("Counter", "test");

		metadata.addService("Doubler");
		metadata.setDescription("Doubler", "multiply number by 2");
		metadata.addInputSlot< int >("Doubler", "number inlet");
		metadata.addOutputSlot< int >("Doubler", "doubler outlet");

		metadata.addService("PrintOut");
		metadata.setDescription("PrintOut", "print out a number to the console");
		metadata.addInputSlot< int >("PrintOut", "input number", int(0));
		metadata.addSetupParameter< Enumeration >("PrintOut", "test enum", Enumeration(Enums("option 1", "hello")("option 2", "have a nice day")("option 3", "goodbye"), "NO VALUE AVAILABLE"));
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