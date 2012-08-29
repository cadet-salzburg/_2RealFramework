#include <iostream>
#include <string>
#include <sstream>
#include "EpocAffectivBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using namespace _2Real::bundle;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

EpocAffectivBlock::EpocAffectivBlock( ContextBlock & context )
	:Block()
{
	m_EpocDeviceManager = static_cast<EpocDeviceManager*>( &context );
}

EpocAffectivBlock::~EpocAffectivBlock()
{
}

void EpocAffectivBlock::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_userIdIn = block.getInletHandle("user_id");

		// outlet handles
		m_engagemementOut = block.getOutletHandle("engagement");
		m_frustrationOut = block.getOutletHandle("frustration");
		m_meditationOut = block.getOutletHandle("meditation");
		m_excitementOut = block.getOutletHandle("excitement");

		m_engagemementOut.getWriteableRef<double>() = 0.0;
		m_frustrationOut.getWriteableRef<double>() = 0.0;
		m_meditationOut.getWriteableRef<double>() = 0.0;
		m_excitementOut.getWriteableRef<double>() = 0.0;


	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocAffectivBlock::update()
{
	try
	{
		if(m_EpocDeviceManager)
		{
			if(m_EpocDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no device connected there is nothing todo so return
			{
				std::cout << "EpocAffectivBlock: EmoEngine is unable to acquire data for processing." << std::endl;
				
				m_engagemementOut.discard();
				m_frustrationOut.discard();
				m_meditationOut.discard();
				m_excitementOut.discard();
				return;
			}

			if(m_EpocDeviceManager->getDeviceID() != m_userIdIn.getReadableRef<unsigned int>() )
			{
				m_engagemementOut.discard();
				m_frustrationOut.discard();
				m_meditationOut.discard();
				m_excitementOut.discard();
				return;
			}

			Vec4 affectiv( m_EpocDeviceManager->getAffectiv() );

			m_engagemementOut.getWriteableRef<double>() = affectiv[0];
			m_frustrationOut.getWriteableRef<double>() = affectiv[1];
			m_meditationOut.getWriteableRef<double>() = affectiv[2];
			m_excitementOut.getWriteableRef<double>() = affectiv[3];
		}
		else
		{
			m_engagemementOut.discard();
			m_frustrationOut.discard();
			m_meditationOut.discard();
			m_excitementOut.discard();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocAffectivBlock::shutdown()
{
	std::cout << "shutdown block" << std::endl;
}

