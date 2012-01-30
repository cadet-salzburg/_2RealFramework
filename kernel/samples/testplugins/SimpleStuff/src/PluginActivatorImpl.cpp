#include "PluginActivatorImpl.h"

#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

#include "ServiceImpl.h"

#include "vld.h"

using namespace _2Real;
using namespace std;

void SimpleStuff::getMetadata(Metadata &metadata)
{
	try
	{
		metadata.setDescription("simplest of testplugins");
		metadata.setAuthor("---");
		metadata.setVersion(0, 0, 0);
		metadata.setContact("---");

		metadata.addService("VecInit");
		metadata.setDescription("VecInit", "---");
		metadata.addInputSlot< unsigned int >("VecInit", "vector size", unsigned int(10));
		metadata.addInputSlot< int >("VecInit", "element value");
		metadata.addOutputSlot< vector < int > >("VecInit", "init out");

		metadata.addService("VecAdd");
		metadata.setDescription("VecAdd", "---");
		metadata.addInputSlot< vector < int > >("VecAdd", "add A");
		metadata.addInputSlot< vector < int > >("VecAdd", "add B");
		metadata.addOutputSlot< vector < int > >("VecAdd", "add out");

		metadata.addService("VecSub");
		metadata.setDescription("VecSub", "---");
		metadata.addInputSlot< vector < int > >("VecSub", "sub A");
		metadata.addInputSlot< vector < int > >("VecSub", "sub B");
		metadata.addOutputSlot< vector < int > >("VecSub", "sub out");
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

void SimpleStuff::setup(PluginContext &context)
{
	context.registerService("VecInit", &::createVecInit);
	context.registerService("VecAdd", &::createVecAdd);
	context.registerService("VecSub", &::createVecSub);
}

void SimpleStuff::shutdown()
{
}

SimpleStuff::~SimpleStuff()
{
}

_2REAL_EXPORT_PLUGIN(SimpleStuff)