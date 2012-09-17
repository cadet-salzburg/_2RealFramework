#pragma once
#include "ILaserRangerDevice.h"
#include "urg_ctrl.h"
//struct urg_t;

class LaserRangerHokuyo : public ILaserRangerDevice
{
public:
    LaserRangerHokuyo();
    virtual ~LaserRangerHokuyo();

    void setup(_2Real::bundle::BlockHandle& handle);
    void update();
    void shutdown();

private:
    LaserRangerHokuyo(const LaserRangerHokuyo& other);

    bool connect();
    bool disconnect();

    urg_t dev;
    std::vector<long> dataVals;
    int maxVals;
    bool connected;
};
