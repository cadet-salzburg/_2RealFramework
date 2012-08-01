#pragma once
#include "_2RealBlock.h"

class RangingImageBlock : public _2Real::bundle::Block
{
public:
    RangingImageBlock();
    virtual ~RangingImageBlock();

    virtual void setup(_2Real::bundle::BlockHandle& handle);
    virtual void update();
    virtual void shutdown();

private:

    std::vector<_2Real::bundle::InletHandle> rangerHandles;
    std::vector<_2Real::bundle::InletHandle> rangerTransforms;
    _2Real::bundle::OutletHandle blobHandle;
};
