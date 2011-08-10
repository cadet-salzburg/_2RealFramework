#include "_2RealContext.h"
#include "_2RealOutputContainer.h"

using namespace _2Real;

int main(int argc, char *argv[])
{

	//get _2Real context instance
	ContextPtr context = Context::instance();

	//request creation of new output container
	ConfigPtr config = context->newConfiguration();

	//begin configuration of new output container
	config->beginConfiguration();

	//add a sequence container
	config->beginSequence();

		//add a synchronization container
		config->beginSynchronization();

			//add 1st service
			config->beginServiceConfiguration("RandomImage_ushort", "ImageProcessing");
			config->configureSetupParameter<unsigned short>("image width", 400);
			config->configureSetupParameter<unsigned short>("image height", 400);
			const Variable img1 = config->configureOutputParameter("output image");
			config->endServiceConfiguration();

			//add 2nd service
			config->beginServiceConfiguration("RandomImage_ushort", "ImageProcessing");
			config->configureSetupParameter<unsigned short>("image width", 400);
			config->configureSetupParameter<unsigned short>("image height", 400);
			const Variable img2 = config->configureOutputParameter("output image");
			config->endServiceConfiguration();

		//end synch block
		config->endGroup();

		config->beginServiceConfiguration("ImageAddition_ushort", "ImageProcessing");
		config->configureSetupParameter<unsigned short>("image width", 400);
		config->configureSetupParameter<unsigned short>("image height", 400);
		config->configureInputParameter("input image 1", img1);
		config->configureInputParameter("input image 2", img2);
		config->endServiceConfiguration();

	//end sequence
	config->endGroup();

	if (!config->endConfiguration())
	{
		std::cout << "configuration failed" << std::endl;
	}
	else
	{
	}

	Sleep(100000);
	
	return 0;
}