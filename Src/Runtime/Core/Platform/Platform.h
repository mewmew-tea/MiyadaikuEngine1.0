#pragma once
#include <memory>
#include "../Subsystem.h"

namespace Miyadaiku
{
class Window;
class Platform final : public Subsystem
{
public:
	std::shared_ptr<Window> GetMainWindow()
	{
		return m_mainWindow;
	}
	void OnAwake() override;

	void OnShutdown() override;

	// システムのイベントを処理します。
	// Windowsなら、TranslateMessageやDispatchMessageが実行されます。
	void ProcessSystemEventQueue();


protected:
	std::shared_ptr<Window> m_mainWindow = nullptr;
	bool					m_isEndSystem = false;
};
}
