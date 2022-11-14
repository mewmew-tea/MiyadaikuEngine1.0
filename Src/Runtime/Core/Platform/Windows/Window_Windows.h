#pragma once
#include "../Window.h"
// struct HINSTANCE;
// struct HWND;
#ifdef _WIN32
#include <Windows.h>

namespace Miyadaiku
{
class Window_Windows : public Window
{
public:
	bool Create() override;
	void Destroy() override;
	void ProcessSystemEventQueue() override;
	bool IsCreated() const;

	void SetParentWindowHandle(HWND _parentHWnd);
	void Show();

	HWND GetWindowHandle() const
	{
		return m_hWnd;
	}

	HINSTANCE GetInstanceHandle() const;

private:
	HWND	  m_hWnd = nullptr;
	HINSTANCE m_hInst = nullptr;
};
} // namespace Miyadaiku

#endif // WIN32
