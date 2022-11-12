#include "../Runtime/Core/Application.h"
int main()
{
	std::unique_ptr<Application> upApp = std::make_unique<Application>();
	if (!upApp)
	{
		return 1;
	}

#if ENABLE_EDITOR_MODE
	upApp->Setup(true);
#else
	upApp->Setup(false);
#endif

	upApp->Iteration();
	upApp->Cleanup();

	return 0;
}