#include "MultiplierBlock.h"
#include "ThroughputBlock.h"

#include "_2RealBundle.h"
#include <sstream>
#include <iostream>

using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName("TutorialBundle");
		info.setDescription( "Tutorial Bundle" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "Tutorial" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		// export info
		// multiplier block
		BlockMetainfo multiplierBlock = info.exportBlock< MultiplierBlock, WithoutContext >( "MultiplierBlock" );
		multiplierBlock.setDescription( "Multiplier Block" );
		multiplierBlock.addInlet<int>( "NumberA", 0 );
		multiplierBlock.addInlet<int>( "NumberB", 0 );
		multiplierBlock.addOutlet<int>( "Result" );
		
		// throughput block (for testing unit test datatypes)
		BlockMetainfo throughputBlock = info.exportBlock< ThroughputBlock, WithoutContext >( "ThroughPutBlock" );
		throughputBlock.setDescription( "ThroughPut Block" );
		throughputBlock.addInlet<_2Real::Number>( "number", _2Real::Number(0) );
		throughputBlock.addInlet<bool>( "bool", 0 );
		throughputBlock.addInlet<unsigned char>( "uchar", 0 );
		throughputBlock.addInlet<char>( "char", 0 );
		throughputBlock.addInlet<unsigned short>( "ushort", 0 );
		throughputBlock.addInlet<short>( "short", 0 );
		throughputBlock.addInlet<unsigned int>( "uint", 0 );
		throughputBlock.addInlet<int>( "int", 0 );
		throughputBlock.addInlet<unsigned long>( "ulong", 0 );
		throughputBlock.addInlet<long>( "long", 0 );
		throughputBlock.addInlet<float>( "float", 0 );
		throughputBlock.addInlet<double>( "double", 0 );
		throughputBlock.addInlet<_2Real::Point>( "point", _2Real::Point());
		throughputBlock.addInlet<std::string>( "string", "" );
		throughputBlock.addInlet<std::vector<unsigned char>>( "vector", std::vector<unsigned char>() );

		throughputBlock.addOutlet<_2Real::Number>( "number" );
		throughputBlock.addOutlet<bool>( "bool" );
		throughputBlock.addOutlet<unsigned char>( "uchar" );
		throughputBlock.addOutlet<char>( "char");
		throughputBlock.addOutlet<unsigned short>( "ushort" );
		throughputBlock.addOutlet<short>( "short");
		throughputBlock.addOutlet<unsigned int>( "uint" );
		throughputBlock.addOutlet<int>( "int" );
		throughputBlock.addOutlet<unsigned long>( "ulong" );
		throughputBlock.addOutlet<long>( "long" );
		throughputBlock.addOutlet<float>( "float" );
		throughputBlock.addOutlet<double>( "double" );
		throughputBlock.addOutlet<_2Real::Point>( "point" );
		throughputBlock.addOutlet<string>( "string" );

		throughputBlock.addOutlet<std::vector<unsigned char>>( "vector" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
