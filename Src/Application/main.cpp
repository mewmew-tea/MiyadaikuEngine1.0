#include "../Runtime/Core/Application.h"
void main()
{
	std::unique_ptr<Application> upApp = std::make_unique<Application>();
	if (!upApp)
	{
		return;
	}
	upApp->Setup();
	upApp->Cleanup();
}