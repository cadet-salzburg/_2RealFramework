#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <unistd.h>

#include "app/_2RealBlockHandle.h"
#include "datatypes/_2RealPoint.h"


class TUIOBundleTestWindow : public sf::RenderWindow/*, public _2Real::bundle::Block*/
{
public:
    TUIOBundleTestWindow();
    ~TUIOBundleTestWindow();

    void update();
    void drawAll();
    void onResize();
    //void setup(_2Real::bundle::BlockHandle& handle);
    void shutdown();
    void setBlockHandle(_2Real::app::BlockHandle* h) {handle = h;}

private:
    sf::CircleShape circle;
    float winX, winY;
    std::vector<_2Real::Point> tracks;
    _2Real::app::BlockHandle* handle;

    //_2Real::bundle::OutletHandle outletHandle;
};
