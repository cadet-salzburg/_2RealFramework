#include "TUIO/TuioServer.h"
#include "TUIOBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using namespace std;
using namespace TUIO;

TUIOBlock::TUIOBlock() : Block()
{
    server = new TUIO::TuioServer("127.0.0.1", 3333);
}

TUIOBlock::~TUIOBlock()
{

}

void TUIOBlock::setup(BlockHandle& handle)
{
    try
    {
        tracks = handle.getInletHandle("Tracklist");
    }
    catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void TUIOBlock::update()
{
	try
	{
	    //invalidate all known tracks
		map<int, curStruct>::iterator iter = cursormap.begin();
		for (; iter != cursormap.end(); ++iter)
        {
            iter->second.updated = false;
        }

		//get new tracks
		vector<Point> newtracks = tracks.getReadableRef<vector<Point>>();

		server->initFrame(TUIO::TuioTime::getSessionTime());

		const unsigned int len = newtracks.size();
		for(unsigned int i = 0; i < len; i++)
        {
            const Point& p = newtracks[i];
            iter = cursormap.find(p.getId());
            if (iter != cursormap.end())
            {
                server->updateTuioCursor(iter->second.cur, p.x(), p.y());
                iter->second.updated = true;
            }
            else
            {
                TuioCursor* cur = server->addTuioCursor(p.x(), p.y());
                curStruct c;
                c.cur = cur;
                c.updated = true;
                cursormap.insert(pair<int, curStruct>(p.getId(), c));
            }
        }

        iter = cursormap.begin();
        while (iter != cursormap.end())
        {
            if (!iter->second.updated)
            {
                server->removeTuioCursor(iter->second.cur);
                cursormap.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }

        server->commitFrame();

	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void TUIOBlock::shutdown()
{
    delete server;
}
