#include "_2RealBundlesUnitTest.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Eigen/Dense"
#include "Eigen/Geometry"

#include "Poco/Mutex.h"

#include <Windows.h>

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

int main( int argc, char *argv[] )
{
	try
	{
		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "..\\..\\bin" );

		BundleHandle sfmlBundle = engine.loadBundle( "SFMLBundle" );

		engine.loadConfig( "modelimport.xml" );

		/*string filePath = "D:\\opengl\\data\\Bunny.ply";

		string vertexSrcBunny = "model.vert";
		string fragmentSrcBunny = "model.frag";

		string attribIndices = "indices ()";
		string attribPosition = "position ( 3 0 )";

		BlockHandle bunnyImporter = sfmlBundle.createBlockInstance( "ModelImporterBlock" );
		bunnyImporter.setup();
		bunnyImporter.setUpdateRate( 30.0 );
		bunnyImporter.start();
		bunnyImporter.getInletHandle( "FilePath" ).setValue( filePath );

		BlockHandle bunnyCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
		bunnyCombiner.setup();
		bunnyCombiner.setUpdateRate( 30.0 );
		bunnyCombiner.getInletHandle( "VertexShaderSource" ).setValueToString( vertexSrcBunny );
		bunnyCombiner.getInletHandle( "FragmentShaderSource" ).setValueToString( fragmentSrcBunny );
		bunnyCombiner.start();

		BlockHandle bunnyDisplay = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
		bunnyDisplay.setup();
		bunnyDisplay.setUpdateRate( 30.0 );
		bunnyDisplay.start();

		InletHandle bunnyBuffers = bunnyCombiner.getInletHandle( "Buffers" );
		bunnyBuffers.add();
		bunnyBuffers[ 0 ].link( bunnyImporter.getOutletHandle( "Indices" ) );
		bunnyBuffers.add();
		bunnyBuffers[ 1 ].link( bunnyImporter.getOutletHandle( "VertexPositions" ) );

		InletHandle bunnyAttribs = bunnyCombiner.getInletHandle( "AttributeDescriptions" );
		bunnyAttribs.add();
		bunnyAttribs[ 0 ].setValue( attribIndices );
		bunnyAttribs.add();
		bunnyAttribs[ 1 ].setValue( attribPosition );

		InletHandle bunnyUniforms = bunnyCombiner.getInletHandle( "UniformValues" );
		InletHandle bunnyMatModel = bunnyUniforms.add();
		bunnyMatModel.link( bunnyDisplay.getOutletHandle( "TransformationMatrix" ) );
		InletHandle bunnyMatView = bunnyUniforms.add();
		bunnyMatView.link( bunnyDisplay.getOutletHandle( "ViewMatrix" ) );
		InletHandle bunnyMatProj = bunnyUniforms.add();
		bunnyMatProj.link( bunnyDisplay.getOutletHandle( "ProjectionMatrix" ) );

		InletHandle bunnyData = bunnyDisplay.getInletHandle( "RenderData" );
		InletHandle bunnyData0 = bunnyData.add();
		bunnyData0.link( bunnyCombiner.getOutletHandle( "RenderData" ) );*/

		while( 1 )
		{
			string line;
			char lineEnd = '\n';
			getline( cin, line, lineEnd );
			if ( line == "q" )
			{
				break;
			}
		}

		//engine.safeConfig( "modelimport.xml" );
	}
	catch( Exception &e )
	{
		cout << e.message() << endl;
	}
	catch ( ... )
	{
		cout << "unknown exception" << endl;
	}

	while( 1 )
	{
		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	return 0;
}
