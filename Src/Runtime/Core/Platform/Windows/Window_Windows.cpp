#include "Window_Windows.h"
#include "../../Engine.h"

#ifdef _WIN32

namespace Miyadaiku
{
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)

{
	switch (message)
	{
	case WM_COMMAND:
	{
		//int wmId = LOWORD(wParam);
		//switch (wmId)
		{
		//default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		/*HDC		hdc = */ BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:

			DestroyWindow(hWnd);
			hWnd = nullptr;
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:

		DestroyWindow(hWnd);
		hWnd = nullptr;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
bool Window_Windows::Create()
{

	m_hInst = (HINSTANCE)::GetModuleHandle(0);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "MiyadaikuEngineApplication";
	wcex.hIconSm = nullptr;
	RegisterClassEx(&wcex);

	m_hWnd =
		CreateWindow("MiyadaikuEngineApplication", "MiyadaikuEngineApplication",
					 WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 1280, 720, nullptr,
					 nullptr, m_hInst, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}

void Window_Windows::Destroy()
{
	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

void Window_Windows::ProcessSystemEventQueue()
{
	// メッセージ取得、処理
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// 終了メッセージ
		if (msg.message == WM_QUIT)
		{
			GetEngine()->RequestShutdown();
			return;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return;
}

bool Window_Windows::IsCreated() const
{
	return (m_hWnd != nullptr);
}

HINSTANCE Window_Windows::GetInstanceHandle() const
{
	return m_hInst;
}
} // namespace Miyadaiku

#endif
