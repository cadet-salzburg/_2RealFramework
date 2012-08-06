#include "_2RealDatatypes.h"
#include "FaceCastBlock.h"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

FaceCastBlock::FaceCastBlock() : Block() {}
FaceCastBlock::~FaceCastBlock() {}

void FaceCastBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;
		/*
		Image &output = block.getOutletHandle( "image_out" ).getWriteableRef< Image >();
		output = Image();

		m_OutWidth = output.getWidth();
		m_OutHeight = output.getHeight();
		m_OutChannelOrder = output.getChannelOrder();
		m_OutImageType = output.getImageType();
		*/
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void FaceCastBlock::update()
{
	try
	{
		/*
		Image &output = m_Block.getOutletHandle( "image_out" ).getWriteableRef< Image >();

		// inlets are accessible in the same order they were declared in the metadata
		vector< InletHandle > inlets = m_Block.getAllInletHandles();
		Image const& input = inlets[ 0 ].getReadableRef< Image>();
		unsigned char kernelX = inlets[ 1 ].getReadableRef< unsigned char >();
		unsigned char kernelY = inlets[ 2 ].getReadableRef< unsigned char >();
		double const& sigmaX = inlets[ 3 ].getReadableRef< double >();
		double const& sigmaY = inlets[ 4 ].getReadableRef< double >();
		int const& border = inlets[ 5 ].getReadableRef< int >();

		//if ( input.isEmpty() )
		//{
		//	m_Block.getOutletHandle( "image_out" ).discard();
		//	std::cout << "input image is empty" << std::endl;
		//	return;
		//}

		// requires a range -> maye be checked by the fw in the future
		if ( sigmaX < 0. || sigmaY < 0. )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			std::cout << "sigma invalid" << std::endl;
			return;
		}

		// kernel size must be odd. i don't think the framework should handle this
		if ( kernelX%2 != 1 || kernelY%2 != 1 )
		{
			// (?) throwing an exception is not really a good idea in this case
			// ( exc requires re-setup, and that really would not help )
			// nonetheless, the user should be notified somehow?
			m_Block.getOutletHandle( "image_out" ).discard();
			std::cout << "kernel size invalid" << std::endl;
			return;
		}

		if ( !( m_OutImageType == input.getImageType() && m_OutChannelOrder == input.getChannelOrder() ) || m_OutWidth != input.getWidth() || m_OutHeight != input.getHeight() )
		{
			cout << "creating new out image" << endl;
			output = input;	// this involves a copy of the data, thus making sure that the outlet has the correct datatype & channel order
							// could, however, be more efficient as the memcpy that happens is not needed
			m_OutWidth = output.getWidth();
			m_OutHeight = output.getHeight();
			m_OutChannelOrder = output.getChannelOrder();
			m_OutImageType = output.getImageType();
		}

		// no copies or anything involved here, this just allows 'viewing' the imagesource as cv mat
		cv::Mat *matSrc = convertToCvMat( input );
		cv::Mat *matDst = convertToCvMat( output );

		cv::GaussianBlur( *matSrc, *matDst, cv::Size( kernelX, kernelY ), sigmaX, sigmaY, border );

		delete matSrc;
		delete matDst;
		*/
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void FaceCastBlock::shutdown() {}