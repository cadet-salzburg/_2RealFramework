#pragma once

#include "_2RealBlock.h"

using namespace _2Real::bundle;

namespace TUIO
{
    class TuioServer;
    class TuioCursor;
}

class TUIOBlock : public Block
{
public:
    TUIOBlock();
    virtual ~TUIOBlock();

    virtual void setup(BlockHandle& handle);
    virtual void update();
    virtual void shutdown();

private:
    struct curStruct
    {
        class TUIO::TuioCursor* cur;
        bool updated;
    };
    InletHandle tracks, host, port;
    std::map<int, curStruct> cursormap;
    class TUIO::TuioServer* server;
};
