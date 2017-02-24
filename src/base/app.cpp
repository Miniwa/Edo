#include <thread>
#include <chrono>

#include "edo/base/app.hpp"

edo::IApplication::IApplication()
{
    setExit(false);
}

void edo::IApplication::run()
{
    setExit(false);
    while(!shouldExit())
    {
        main();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool edo::IApplication::shouldExit()
{
    return exit;
}

void edo::IApplication::setExit(bool exit)
{
    this->exit = exit;
}
