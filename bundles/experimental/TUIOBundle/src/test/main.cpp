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

        engine->setBaseDirectory("../lib");
        tuioBundle = &(engine->loadBundle("libTUIOBundle"));
        printf("DESC: %s\n", tuioBundle->getBundleInfo().getDescription().c_str());
        tuioBlockHandle = &(tuioBundle->createBlockInstance("TUIOBlock"));
        if (tuioBlockHandle->isValid())
        {
            printf("TUIOBlockHandle OK!\n");
        }
        tuioBlockHandle->setup();
        printf("SETUP OK!\n");
        tuioBlockHandle->setUpdateRate(30.f);
        printf("UPDATERATE OK!\n");
        tuioBlockHandle->start();
        printf("START OK!\n");

//        tuioBundleTest = &(engine->loadBundle("libTUIOBundleTest"));
//        printf("DESC: %s\n", tuioBundleTest->getBundleInfo().getDescription().c_str());

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

        // Clear screen
        //app.clear(sf::Color(0, 0, 255));

        // Draw the sprite
        app.update();

        app.drawAll();
        app.display();
    }

    //_2Real::app::BundleHandle* tuioBundleTest = nullptr;

	engine->clearAll();




    return 0;
}
