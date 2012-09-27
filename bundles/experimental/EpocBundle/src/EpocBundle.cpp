#include "EpocDeviceManager.h"
#include "EpocAffectivBlock.h"
#include "EpocCognitivBlock.h"
#include "EpocExpressivBlock.h"
#include "EpocGyroBlock.h"

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
		info.setName("EpocBundle");
		info.setDescription( "Connection to Emotiv Epoc BCI" );
		info.setAuthor( "Veronika Pauser" );
		info.setCategory( "Devices" );
		info.setContact( "support@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< EpocDeviceManager >();
		
		BlockMetainfo epocGyro = info.exportBlock< EpocGyroBlock, WithContext >( "EpocGyroBlock" );

		epocGyro.addInlet<unsigned int>("user_id", 0);
		epocGyro.addOutlet<int>("gyro_x");
		epocGyro.addOutlet<int>("gyro_y");

		epocGyro.setDescription( "Epoc Gyro Data" );
		
		BlockMetainfo epocAffectiv = info.exportBlock< EpocAffectivBlock, WithContext >( "EpocAffectivBlock" );

		epocAffectiv.addInlet<unsigned int>( "user_id", 0 );
		epocAffectiv.addOutlet<double>( "engagement" );
		epocAffectiv.addOutlet<double>( "frustration" );
		epocAffectiv.addOutlet<double>( "meditation" );
		epocAffectiv.addOutlet<double>( "excitement" );

		epocAffectiv.setDescription( "Epoc Affectiv Data" );

		BlockMetainfo epocExpressiv = info.exportBlock< EpocExpressivBlock, WithContext >( "EpocExpressivBlock" );

		epocExpressiv.addInlet<unsigned int>( "user_id", 0 );
		epocExpressiv.addOutlet<bool>( "blink" );
		epocExpressiv.addOutlet<bool>( "right_wink" );
		epocExpressiv.addOutlet<bool>( "left_wink" );
		epocExpressiv.addOutlet<bool>( "look_right" );
		epocExpressiv.addOutlet<bool>( "look_left" );
		epocExpressiv.addOutlet<double>( "raise_brow" );
		epocExpressiv.addOutlet<double>( "furrow_brow" );
		epocExpressiv.addOutlet<double>( "smile" );
		epocExpressiv.addOutlet<double>( "clench" );
		epocExpressiv.addOutlet<double>( "smirk_right" );
		epocExpressiv.addOutlet<double>( "smirk_left" );
		epocExpressiv.addOutlet<double>( "laugh" );

		epocExpressiv.setDescription( "Epoc Expressiv Data" );
		
		
		BlockMetainfo epocCognitiv = info.exportBlock< EpocCognitivBlock, WithContext >( "EpocCognitivBlock" );

		epocCognitiv.addInlet<unsigned int>( "user_id", 0 );
		epocCognitiv.addOutlet<double>( "neutral" );
		epocCognitiv.addOutlet<double>( "push" );
		epocCognitiv.addOutlet<double>( "pull" );
		epocCognitiv.addOutlet<double>( "lift" );
		epocCognitiv.addOutlet<double>( "drop" );
		epocCognitiv.addOutlet<double>( "left" );
		epocCognitiv.addOutlet<double>( "right" );
		epocCognitiv.addOutlet<double>( "rotate_left" );
		epocCognitiv.addOutlet<double>( "rotate_right" );
		epocCognitiv.addOutlet<double>( "rotate_clockwise" );
		epocCognitiv.addOutlet<double>( "rotate_counter_clockwise" );
		epocCognitiv.addOutlet<double>( "rotate_forwards" );
		epocCognitiv.addOutlet<double>( "rotate_reverse" );
		epocCognitiv.addOutlet<double>( "disappear" );

		epocCognitiv.setDescription( "Epoc Cognitiv Data" );
		
		
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}