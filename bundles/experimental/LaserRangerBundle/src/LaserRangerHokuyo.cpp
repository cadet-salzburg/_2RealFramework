//#include "urg_t.h"
#include "LaserRangerHokuyo.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using namespace std;

LaserRangerHokuyo::LaserRangerHokuyo() 
: ILaserRangerDevice()
, maxVals (0)
, connected(false)
{
	memset (&dev, 0, sizeof(dev));
}

LaserRangerHokuyo::~LaserRangerHokuyo()
{
	disconnect();
}

void LaserRangerHokuyo::shutdown()
{
	ILaserRangerDevice::shutdown();
	disconnect();
}

bool LaserRangerHokuyo::connect()
{
	if (connected)
	{
		return true;
	}

	int ret = urg_connect(&dev, deviceStr.c_str(), baudrate);

	if (ret < 0)
	{
		printf("LASERRANGERHOKUYO %s: Unable to connect. Error: %s", deviceStr.c_str(), urg_error(&dev));
	}
	else
	{
		maxVals = urg_dataMax(&dev);
		dataVals.resize(maxVals);
		connected = true;
	}

	return connected;
}

bool LaserRangerHokuyo::disconnect()
{
	if (!connected)
	{
		return false;
	}

	urg_disconnect(&dev);
	connected = false;
	printf("LASERRANGERHOKUYO::disconnect(): Connection closed!\n");
	return true;
}

void LaserRangerHokuyo::setup(BlockHandle& handle)
{
	try
	{
		ILaserRangerDevice::setup(handle);

		connect();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void LaserRangerHokuyo::update()
{
	try
	{
		dataVals.clear();

		if (!connected)
		{
			printf("LASERRANGERHOKUYO::receive(): Not connected!\n");
			return;
		}

		if (urg_requestData(&dev, URG_GD, URG_FIRST, URG_LAST) < 0)
		{
			printf("LASERRANGERHOKUYO::receive(): Unable to request data from the device! Disconnecting.\n");
			disconnect();
			return;
		}

		int recv = urg_receiveData(&dev, &(dataVals[0]), maxVals);
		if (recv < 0)
		{
			printf("LASERRANGERHOKUYO::receive(): Unable to receive data from the device!\n");
			return;
		}

		rangeOutlet.getWriteableRef<std::vector<long>>() = dataVals;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}


