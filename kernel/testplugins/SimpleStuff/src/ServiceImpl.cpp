#include "ServiceImpl.h"
#include "_2RealException.h"

using namespace _2Real;
using namespace std;

_2Real::IService *const createVecInit(void)
{
	_2Real::IService *service = new VecInit();
	return service;
}

_2Real::IService *const createVecAdd(void)
{
	_2Real::IService *service = new VecAdd();
	return service;
}

_2Real::IService *const createVecSub(void)
{
	_2Real::IService *service = new VecSub();
	return service;
}

void VecInit::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_ElementValue = context.getInputHandle("element value");
		m_VectorSize = context.getInputHandle("vector size");
		m_OutputVector = context.getOutputHandle("init out");
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

void VecInit::update()
{
	try
	{
		unsigned int const& size = m_VectorSize.data< unsigned int >();
		int const& value = m_ElementValue.data< int >();
		std::vector< int > &out = m_OutputVector.data< std::vector < int > >();

		//std::cout << "default value: " << value << std::endl;

		out.clear();
		out = std::vector< int >(size, value);
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

void VecAdd::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_InputVectorA = context.getInputHandle("add A");
		m_InputVectorB = context.getInputHandle("add B");
		m_OutputVector = context.getOutputHandle("add out");
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

void VecAdd::update()
{
	try
	{
		std::vector< int > const& inA = m_InputVectorA.data< std::vector < int > >();
		std::vector< int > const& inB = m_InputVectorB.data< std::vector < int > >();
		std::vector< int > &out = m_OutputVector.data< std::vector < int > >();

		unsigned int sz = min(inA.size(), inB.size());
		out.resize(sz);
		for (unsigned int i=0; i<sz; ++i)
		{
			int a = inA.at(i);
			int b = inB.at(i);
			int c = a+b;

			//std::cout << a << " " << b << " " << c << std::endl;
			out.at(i) = c;
		}
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



void VecSub::setup(_2Real::ServiceContext &context)
{
	try
	{
		m_InputVectorA = context.getInputHandle("sub A");
		m_InputVectorB = context.getInputHandle("sub B");
		m_OutputVector = context.getOutputHandle("sub out");
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

void VecSub::update()
{
	try
	{
		std::vector< int > const& inA = m_InputVectorA.data< std::vector < int > >();
		std::vector< int > const& inB = m_InputVectorB.data< std::vector < int > >();
		std::vector< int > &out = m_OutputVector.data< std::vector < int > >();

		unsigned int sz = min(inA.size(), inB.size());
		out.resize(sz);
		for (unsigned int i=0; i<sz; ++i)
		{
			out.at(i) = inA.at(i)-inB.at(i);
		}
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