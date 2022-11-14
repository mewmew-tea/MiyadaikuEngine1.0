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

	void EnableStartUpAsEditor();
	bool IsEditorMode();

	// void BeginFrame();

	// void EndFrame();

	SubsystemLocator& GetSubsystemLocator()
	{
		return m_subsystemLocator;
	}

private:
	SubsystemLocator m_subsystemLocator;
	bool			 m_requestedShutdown = false;

	bool m_isEditorMode = false;


private:
	// コピー禁止
	Engine(const Engine& src) = delete;
	void operator=(const Engine& src) = delete;
};
} // namespace Miyadaiku