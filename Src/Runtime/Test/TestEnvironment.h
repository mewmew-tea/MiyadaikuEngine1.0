#include <memory>

namespace Miyadaiku
{
class Engine;
}

class TestEnviroment
{
public:
	static void SetUp();
	static void TearDown();

	static std::unique_ptr<Miyadaiku::Engine> m_upEngine;
};
