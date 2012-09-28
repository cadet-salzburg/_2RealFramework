#include "_2RealBundle.h"
#include "ShapeRecognitionBlock.h"
#include "ShapeRecordingBlock.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "ShapeRecognitionBundle" );
		info.setDescription( "Detects user poses in depth images." );
		info.setAuthor( "Veronika Pauser" );
		info.setCategory( "Image Processing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		Image initDepthImage( (unsigned short*)NULL, false, 640, 480, ImageChannelOrder::A );
		
		BlockMetainfo shapeRecording = info.exportBlock< ShapeRecordingBlock, WithoutContext>( "ShapeRecordingBlock" );
		shapeRecording.setDescription( "Teaches a certain pose to the system." );

		shapeRecording.addInlet< Image >( "depth_image", initDepthImage );
		shapeRecording.addInlet< string >( "output_path", "./data" );
		shapeRecording.addInlet< bool >( "save", false );

		BlockMetainfo shapeRecognition = info.exportBlock< ShapeRecognitionBlock, WithoutContext>( "ShapeRecognitionBlock" );
		shapeRecognition.setDescription( "Recognizes current pose in set of poses taught to system." );

		shapeRecognition.addInlet< string >( "data_path", "./data" );
		shapeRecognition.addInlet< Image >( "depth_image", initDepthImage );

		shapeRecognition.addOutlet< string >("file_name");
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}