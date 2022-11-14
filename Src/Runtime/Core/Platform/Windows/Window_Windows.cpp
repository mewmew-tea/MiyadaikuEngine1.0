#include "Window_Windows.h"
#include "../../Engine.h"

#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
// imguiウィンドウメッセージ処理用
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
									   LPARAM lParam);

#ifdef _WIN32

namespace Miyadaiku
{
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ImGuiにイベント通知
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
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

void SetClientSize(int w, int h, HWND _hWnd)
{
	RECT rcWnd, rcCli;

	GetWindowRect(_hWnd, &rcWnd);
	GetClientRect(_hWnd, &rcCli);

	MoveWindow(_hWnd,
			   rcWnd.left, // X座標
			   rcWnd.top,  // Y座標
			   w + (rcWnd.right - rcWnd.left) - (rcCli.right - rcCli.left),
			   h + (rcWnd.bottom - rcWnd.top) - (rcCli.bottom - rcCli.top),
			   TRUE);
}

bool Window_Windows::Create()
{
	m_hInst = (HINSTANCE)::GetModuleHandle(0);

	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	// wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	 wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "MiyadaikuEngineApplication";
	wcex.hIconSm = nullptr;
	RegisterClassEx(&wcex);

	m_hWnd =
		CreateWindow("MiyadaikuEngineApplication", "MiyadaikuEngineApplication",
					 WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, m_rect.width,
					 m_rect.height, nullptr,
					 nullptr, m_hInst, nullptr);


	if (!GetEngine()->IsEditorMode())
	{
		SetClientSize(m_rect.width, m_rect.height, m_hWnd);
		ShowWindow(m_hWnd, SW_SHOW);
	}

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

void Window_Windows::SetParentWindowHandle(HWND _parentHWnd)
{
	if (_parentHWnd)
	{
		SetWindowLong((HWND)m_hWnd, GWL_STYLE, WS_CHILD);

		SetParent((HWND)m_hWnd, _parentHWnd);
		SetClientSize(m_rect.width, m_rect.height, m_hWnd);
	}
}

void Window_Windows::Show()
{
	ShowWindow(m_hWnd, SW_SHOW);
}

HINSTANCE Window_Windows::GetInstanceHandle() const
{
	return m_hInst;
}
} // namespace Miyadaiku

#endif
