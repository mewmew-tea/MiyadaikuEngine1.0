#pragma once

//================================================
// Includes
//================================================
#include <memory>

//================================================
namespace Miyadaiku
{
    class Engine;
}
//================================================

//================================================
class Application
{
public:
    Application();
    ~Application();

    int Setup();
	void Cleanup();

#if TESTS_ENABLED
	//void Tests_Setup();
#endif

private:
    std::unique_ptr<Miyadaiku::Engine> m_engine;
};
