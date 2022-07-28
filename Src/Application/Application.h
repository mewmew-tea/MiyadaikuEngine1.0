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

private:
    std::unique_ptr<Miyadaiku::Engine> m_engine;
};
