#include "_2RealContext.h"
#include "_2RealFramework.h"

using namespace _2Real;

int main(int argc, char *argv[])
{
	ContextPtr context = ::Context::instance();

	context->start();

	unsigned int count = 0;

	//currently, services are updated in sequence
	while (count < 1000)
	{
		context->update();
		count++;
	}

	//will crash when uninstalling kinect test plugin
	//due to incomplete service registry implementation
	//context->stop();

	return 0;
}