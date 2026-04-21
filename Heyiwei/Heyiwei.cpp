#include <iostream>
#include "App.h"

int main()
{
    WaterManager waterManager;
    App app(waterManager);
    app.run();
    return 0;
}