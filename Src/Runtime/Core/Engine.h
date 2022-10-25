#pragma once
#include <memory>
#include "Subsystem.h"

namespace Miyadaiku
{
// 各種モジュールからエンジンを取得する用。
// エンジンは、Applicationで作成されます。
class Engine;
Engine* GetEngine();

class Engine
{
public:
	Engine();
	~Engine();

	// ゲームループなど
	void SetUp();

	void Update();

	void RequestShutdown();
	bool IsRequestedShutdown()
	{
		return m_requestedShutdown;
	}

	// void BeginFrame();

	// void EndFrame();

	SubsystemLocator& GetSubsystemLocator()
	{
		return m_subsystemLocator;
	}

	void* m_editorHWnd = nullptr;
	int	  m_editorWidth = 1280;
	int	  m_editorHeight = 720;

private:
	SubsystemLocator m_subsystemLocator;
	bool			 m_requestedShutdown = false;


private:
	// コピー禁止
	Engine(const Engine& src) = delete;
	void operator=(const Engine& src) = delete;
};
} // namespace Miyadaiku