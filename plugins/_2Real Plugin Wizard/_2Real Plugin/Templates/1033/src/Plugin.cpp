#include "Plugin.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

#include "ServiceImpl.h"

using namespace _2Real;
using namespace std;

void  [!output SAFE_PROJECT_NAME]::getMetadata(Metadata &metadata)
{
	try
	{
		metadata.setDescription("[!output SAFE_PROJECT_NAME]");
		metadata.setAuthor("help@cadet.at");
		metadata.setVersion(0, 0, 1);
		metadata.setContact("help@cadet.at");

		metadata.addService("Service 1");
		metadata.setDescription("Service 1", "description");
		// define the data type of the slot, the service name and the output name
		metadata.addInputSlot< int >("Service 1", "s1 inlet");
		metadata.addOutputSlot< int >("Service 1", "s1 outlet");

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

void [!output SAFE_PROJECT_NAME]::setup(PluginContext &context)
{
	context.registerService("Service 1", &::createService1);
}

void [!output SAFE_PROJECT_NAME]::shutdown()
{
}

[!output SAFE_PROJECT_NAME]::~[!output SAFE_PROJECT_NAME]()
{
}

[!if SINGLETON_CHECKBOX]
_2REAL_EXPORT_SINGLETON([!output SAFE_PROJECT_NAME])
[!else]
_2REAL_EXPORT_PLUGIN([!output SAFE_PROJECT_NAME])
[!endif]




