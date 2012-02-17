#include "ServiceImpl.h"
#include "_2RealException.h"

using namespace _2Real;
using namespace std;

_2Real::IService *const createCounter(void)
{
	_2Real::IService *service = new Counter();
	return service;
}

_2Real::IService *const createDoubler(void)
{
	_2Real::IService *service = new Doubler();
	return service;
}

_2Real::IService *const createPrintOut(void)
{
	_2Real::IService *service = new PrintOut();
	return service;
}

// Implement the Interface of the first Service ("Counter")
void Counter::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_CurrentCount = 1;
		// pass name of output slot as defined in metadata.addOutputSlot to get the output handle 
		m_CounterValue = context.getOutputHandle("counter outlet");
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

void Counter::update()
{
	try
	{
		m_CounterValue.data<int>() = m_CurrentCount;
		//cout << "Counter: " << m_CurrentCount << endl;
		m_CurrentCount+=1;
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

// Implement the Interface of the second Service ("Doubler")
void Doubler::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_InputValue = context.getInputHandle("number inlet");
		m_OutputValue = context.getOutputHandle("doubler outlet");
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

void Doubler::update()
{
	try
	{
		//cout << "Doubler1: " << m_InputValue.data<int>() << endl;
		//cout << "doubler: " << d*2 << endl;
		m_OutputValue.data< int>() = 2*m_InputValue.data< int >();
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

// Implement the Interface of the third Service ("PrintOut")
void PrintOut::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_InputValue = context.getInputHandle("input number");
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

void PrintOut::update()
{
	try
	{
		cout << "PrintOut: " << m_InputValue.data< int >() << endl;
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