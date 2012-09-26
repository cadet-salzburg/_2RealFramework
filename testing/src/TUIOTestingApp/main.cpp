#include "TUIOBundleTestWindow.h"
#include "_2RealApplication.h"
#include "helpers/_2RealException.h"

int main()
{
    _2Real::app::Engine* engine = nullptr;
    _2Real::app::BundleHandle* tuioBundle = nullptr;
    _2Real::app::BlockHandle* tuioBlockHandle = nullptr;

    try
    {
        engine = &(_2Real::app::Engine::instance());

        engine->setBaseDirectory("../bin");
        tuioBundle = &(engine->loadBundle("libTUIOBundle"));
        printf("DESC: %s\n", tuioBundle->getBundleInfo().description.c_str());
        tuioBlockHandle = &(tuioBundle->createBlockInstance("TUIOBlock"));
        if (tuioBlockHandle->isValid())
        {
            printf("TUIOBlockHandle OK!\n");
        }
        tuioBlockHandle->getInletHandle("host").setValue(std::string("192.168.5.76"));
        tuioBlockHandle->setup();
        tuioBlockHandle->setUpdateRate(30.f);
        tuioBlockHandle->start();

    }
    catch ( _2Real::Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}

    TUIOBundleTestWindow app;
    app.setVerticalSyncEnabled(true);

    app.setBlockHandle(tuioBlockHandle);

    while (app.isOpen())
    {
        // Process events
        sf::Event Event;
        while (app.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                app.close();
        }

        // Draw the sprite
        app.update();

        app.drawAll();
        app.display();
    }

	engine->clearAll();

    return 0;
}
