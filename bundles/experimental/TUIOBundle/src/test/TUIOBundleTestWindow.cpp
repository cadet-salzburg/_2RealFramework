#include <cstdio>
#include "app/_2RealBlockHandle.h"
#include "app/_2RealInletHandle.h"
#include "helpers/_2RealException.h"
#include "TUIOBundleTestWindow.h"


#define CIRCLE_W 10.f

using namespace _2Real;

TUIOBundleTestWindow::TUIOBundleTestWindow() : sf::RenderWindow(sf::VideoMode(640,480), "TUIOBundleTest"), circle(CIRCLE_W), handle(0)
{
    winX = (float)getSize().x;
    winY = (float)getSize().y;
}

TUIOBundleTestWindow::~TUIOBundleTestWindow()
{

}

//void TUIOBundleTestWindow::setup(bundle::BlockHandle& handle)
//{
//    try
//    {
//        outletHandle = handle.getOutletHandle("TracklistOut");
//    }
//    catch ( Exception &e )
//	{
//		std::cout << e.message() << std::endl;
//		e.rethrow();
//	}
//}

void TUIOBundleTestWindow::update()
{
    tracks.clear();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        float x = (float) sf::Mouse::getPosition(*this).x;
        float y = (float) sf::Mouse::getPosition(*this).y;
        if (x < 0.f || x > winX || y < 0.f || y > winY)
        {
            circle.setPosition(-100.f, -100.f);
            return;
        }
        circle.setPosition(x - CIRCLE_W, y - CIRCLE_W);
        printf("X: %1.2f, Y: %1.2f\n", x / winX, y / winY);
        Point p(Number(x / winX), Number(y / winY), Number(0.f), "TRACK", 0);
        tracks.push_back(p);
    }
    else
    {
        circle.setPosition(-100.f, -100.f);
    }

//    if (outletHandle.isValid())
//        outletHandle.getWriteableRef<std::vector<Point>>() = tracks;

//
//    sf::Event Event;
//    while (pollEvent(Event))
//    {
//        // Close window : exit
//        if (Event.type == sf::Event::Closed)
//            close();
//    }
//
//    // Clear screen
//    clear(sf::Color(128, 128, 196));
//
//    drawAll();
//    display();
}

void TUIOBundleTestWindow::drawAll()
{
    clear(sf::Color(128, 128, 196));
    draw(circle);
    if (handle && tracks.size() > 0)
    {
        try
        {
            handle->getInletHandle("Tracklist").setValue(tracks);
            printf("GOT INLET HANDLE!\n");
            //handle->singleStep();
            printf("SINGLESTEP SUCCEEDED!\n");
        }
        catch ( _2Real::Exception &e )
        {
            std::cout << e.message() << std::endl;
            e.rethrow();
        }
    }
}

void TUIOBundleTestWindow::onResize()
{
    winX = (float)getSize().x;
    winY = (float)getSize().y;
}

void TUIOBundleTestWindow::shutdown()
{

}
