#include "RangingImageBlock.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::bundle::InletHandle;
using _2Real::Exception;
using namespace std;

RangingImageBlock::RangingImageBlock()
{

}

RangingImageBlock::~RangingImageBlock()
{

}

void RangingImageBlock::setup(BlockHandle& handle)
{
    InletHandle h;

    try
    {
        h = handle.getInletHandle("RangerA");
        if (h.isValid())
        {
            rangerHandles.push_back(h);
            rangerTransforms.push_back(handle.getInletHandle("RangerATransform"));
        }

        h = handle.getInletHandle("RangerB");
        if (h.isValid())
        {
            rangerHandles.push_back(h);
            rangerTransforms.push_back(handle.getInletHandle("RangerBTransform"));
        }

        h = handle.getInletHandle("RangerC");
        if (h.isValid())
        {
            rangerHandles.push_back(h);
            rangerTransforms.push_back(handle.getInletHandle("RangerCTransform"));
        }

        h = handle.getInletHandle("RangerD");
        if (h.isValid())
        {
            rangerHandles.push_back(h);
            rangerTransforms.push_back(handle.getInletHandle("RangerDTransform"));
        }

        h = handle.getInletHandle("RangerE");
        if (h.isValid())
        {
            rangerHandles.push_back(h);
            rangerTransforms.push_back(handle.getInletHandle("RangerETransform"));
        }

        h = handle.getInletHandle("RangerF");
        if (h.isValid())
        {
            rangerHandles.push_back(h);
            rangerTransforms.push_back(handle.getInletHandle("RangerFTransform"));
        }
    }
    catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void RangingImageBlock::update()
{
    const unsigned int len = rangerHandles.size();
    try
    {
        for (unsigned i = 0; i < len; i++)
        {
            std::vector<long> rangingVals = rangerHandles[i].getReadableRef<std::vector<long>>();
            const unsigned int lenVals = rangingVals.size();
            for (unsigned value = 0; value < lenVals; value++)
            {
                //apply the ranger's transformation to every value in the image

                //overlay all images (OpenGL would be easiest)
            }
        }
    }
    catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void RangingImageBlock::shutdown()
{

}
