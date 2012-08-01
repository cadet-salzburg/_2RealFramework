#pragma once
#include "_2RealBlock.h"
#include "urg_ctrl.h"
//struct urg_t;

class LaserRangerDevice : public _2Real::bundle::Block
{
public:
    LaserRangerDevice();
    virtual ~LaserRangerDevice();

    virtual void setup(_2Real::bundle::BlockHandle& handle);
    virtual void update();
    virtual void shutdown();

private:
    LaserRangerDevice(const LaserRangerDevice& other);


    bool connect();
    bool disconnect();
    urg_t dev;
    std::string deviceStr;
    std::vector<long> dataVals;
    int baudrate, maxVals;
    bool connected;

    _2Real::bundle::InletHandle devInlet, baudInlet;
    _2Real::bundle::OutletHandle rangeOutlet;
};
