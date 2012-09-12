#include "_2RealBundle.h"

#include "VideoGStreamerBlock.h"

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

 // Defines the VideoGStreamer Bundle which consists of one block that plays video / audio files
void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		// Bundle information
		info.setName( "VideoGStreamerBundle" );
		info.setDescription( "Video and Audio Playback" );
		info.setAuthor( "Steven Stojanovic" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		// Define the VideoGStreamer Block with its Inlets and Outlets
		BlockMetainfo videoGStreamer = info.exportBlock< VideoGStreamerBlock, WithoutContext >( "VideoGStreamerBlock" );
		videoGStreamer.addInlet<string>( "FilePath", "" );
		videoGStreamer.addInlet<int>( "PlayState", 0 );
		videoGStreamer.addInlet<float>( "Position", 0.0f );
		videoGStreamer.addInlet<int>( "Direction", 0 );
		videoGStreamer.addInlet<float>( "Speed", 1.0f );
		videoGStreamer.addInlet<float>( "Volume", 1.0f );
		videoGStreamer.addInlet<int>( "LoopMode", 0 );
		videoGStreamer.addInlet<bool>( "ManualAudioBuffer", false );
		videoGStreamer.addOutlet<Image>( "Video" );
		videoGStreamer.addOutlet<int>( "Width" );
		videoGStreamer.addOutlet<int>( "Height" );
		videoGStreamer.addOutlet<unsigned long>( "NumberOfFrames" );
		videoGStreamer.addOutlet<float>( "FPS" );
		videoGStreamer.addOutlet<AudioBuffer>( "Audio" );


	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}