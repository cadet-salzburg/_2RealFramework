#pragma once
#include "_2RealBlock.h"

class ILaserRangerDevice : public _2Real::bundle::Block
{
public:
	ILaserRangerDevice() : baudrate(0) {}
	virtual ~ILaserRangerDevice() {}

	virtual void setup(_2Real::bundle::BlockHandle& handle)
	{
		try
		{
			devInlet = handle.getInletHandle("Devicepath");
			baudInlet = handle.getInletHandle("Baudrate");
			rangeOutlet = handle.getOutletHandle("Rangeimage");

			baudrate = baudInlet.getReadableRef<int>();
			deviceStr = devInlet.getReadableRef<std::string>();
		}
		catch ( _2Real::Exception &e )
		{
			std::cout << e.message() << std::endl;
			e.rethrow();
		}
	}

	virtual void update() = 0;
	virtual void shutdown() {}

private:
	ILaserRangerDevice(const ILaserRangerDevice& other) { *this = other; }

protected:
	_2Real::bundle::InletHandle devInlet, baudInlet;
	_2Real::bundle::OutletHandle rangeOutlet;

	int baudrate;
	std::string deviceStr;
};
