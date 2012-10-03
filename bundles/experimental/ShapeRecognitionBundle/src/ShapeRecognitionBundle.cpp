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
		info.setContact( "support@cadet.at" );
		info.setVersion( 0, 1, 0 );

		Image initDepthImage( (unsigned short*)NULL, false, 640, 480, ImageChannelOrder::A );
		
		BlockMetainfo shapeRecording = info.exportBlock< ShapeRecordingBlock, WithoutContext>( "ShapeRecordingBlock" );
		shapeRecording.setDescription( "Teaches a certain pose to the system." );

		shapeRecording.addInlet< Image >( "depth_image", initDepthImage );
		shapeRecording.addInlet< string >( "output_path", "../build/vc10/ShapeRecognitonTestingApp/data" );
		shapeRecording.addInlet< int > ("count_time", 3);
		shapeRecording.addInlet< int > ("max_distance", 1500); // maximal distance to camera in mm
		shapeRecording.addInlet< bool >( "save", false );


		BlockMetainfo shapeRecognition = info.exportBlock< ShapeRecognitionBlock, WithoutContext>( "ShapeRecognitionBlock" );
		shapeRecognition.setDescription( "Recognizes current pose in set of poses taught to system." );

		shapeRecognition.addInlet< string >( "data_path", "../build/vc10/ShapeRecognitonTestingApp/data" );
		shapeRecognition.addInlet< Image >( "depth_image", initDepthImage );
		shapeRecognition.addInlet< int > ("max_distance", 1500); // maximal distance to camera in mm
		shapeRecognition.addInlet< int > ("min_probability", 50);
		shapeRecognition.addOutlet< string >("file_name");
		shapeRecognition.addOutlet< double >("probability");
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}