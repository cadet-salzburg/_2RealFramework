#include "VideoInputBlock.h"
#include "_2RealFrameworkContext.h"
#include "_2RealException.h"
#include "_2RealEnum.h"
#include "_2RealImageT.h"
#include <iostream>
#include <string>

using _2Real::FrameworkContext;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

VideoInputBlock::VideoInputBlock( _2Real::ContextBlock & context )
	:Block()
{
	m_CameraDeviceManager = static_cast<CameraDeviceManager*>( &context );
}
void VideoInputBlock::setup( FrameworkContext &context )
{
	try
	{
		m_DeviceIndexHandle	= context.getInletHandle("deviceIndexInlet");
		m_ImageOutletHandle = context.getOutletHandle( "imageOutlet" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void VideoInputBlock::update()
{
	try
	{
		m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char> >() = m_CameraDeviceManager->getPixels( abs( m_DeviceIndexHandle.getReadableRef<int>() )%m_CameraDeviceManager->getNumberOfDevices() );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};