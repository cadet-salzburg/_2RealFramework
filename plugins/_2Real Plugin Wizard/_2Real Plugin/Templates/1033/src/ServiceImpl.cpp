#include "ServiceImpl.h"
#include "_2RealException.h"

using namespace _2Real;
using namespace std;

_2Real::IService *const createService1(void)
{
	_2Real::IService *service = new Service1();
	return service;
}


// Implement the Interface of the first Service ("Counter")
void Service1::setup(_2Real::ServiceContext &context)
{
	try
	{
		// names of slot have to match the names defined in metadata.addOutputSlot 
		m_InValue = context.getInputHandle("s1 inlet");
		m_OutValue = context.getOutputHandle("s1 outlet");
	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service setup: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}
};

void Service1::update()
{
	try
	{
		//do something
	}
	catch (_2Real::Exception &e)
	{
		e.rethrow();
	}
	catch (std::exception &e)
	{
		std::ostringstream msg;
		msg << "exception during service update: " << e.what();
		throw _2Real::ServiceException(msg.str());
	}
};
