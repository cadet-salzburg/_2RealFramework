//#include "urg_t.h"
#include "LaserRangerDevice.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using namespace std;

LaserRangerDevice::LaserRangerDevice() : deviceStr(""), baudrate(0),  maxVals(0), connected(false)
{

}

LaserRangerDevice::~LaserRangerDevice()
{
    disconnect();
}

bool LaserRangerDevice::connect()
{
    if (connected)
    {
        return true;
    }

    int ret = urg_connect(&dev, deviceStr.c_str(), baudrate);

    if (ret < 0)
    {
        printf("LASERRANGERDEVICE %s: Unable to connect. Error: %s", deviceStr.c_str(), urg_error(&dev));
    }
    else
    {
        maxVals = urg_dataMax(&dev);
        dataVals.resize(maxVals);
        connected = true;
    }

    return connected;
}

bool LaserRangerDevice::disconnect()
{
    if (!connected)
    {
        return false;
    }

    urg_disconnect(&dev);
    connected = false;
    printf("LASERRANGERDEVICE::disconnect(): Connection closed!\n");
    return true;
}

void LaserRangerDevice::setup(BlockHandle& handle)
{
    try
    {
        devInlet = handle.getInletHandle("Devicepath");
        baudInlet = handle.getInletHandle("Baudrate");
        rangeOutlet = handle.getOutletHandle("Rangeimage");

        baudrate = baudInlet.getReadableRef<int>();
        deviceStr = devInlet.getReadableRef<std::string>();

        connect();
    }
    catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void LaserRangerDevice::update()
{
    try
    {
        rangeOutlet.discard();
        dataVals.clear();

        if (!connected)
        {
            printf("LASERRANGERDEVICE::receive(): Not connected!\n");
            return;
        }

        if (urg_requestData(&dev, URG_GD, URG_FIRST, URG_LAST) < 0)
        {
            printf("LASERRANGERDEVICE::receive(): Unable to request data from the device! Disconnecting.\n");
            disconnect();
            return;
        }

        int recv = urg_receiveData(&dev, &(dataVals[0]), maxVals);
        if (recv < 0)
        {
            printf("LASERRANGERDEVICE::receive(): Unable to receive data from the device!\n");
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


