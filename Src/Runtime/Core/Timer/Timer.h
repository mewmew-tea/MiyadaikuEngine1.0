#pragma once
#include "../Subsystem.h"
#include <chrono>
#include <thread>

namespace Miyadaiku
{

class Timer : public Subsystem
{
public:
	void OnAwake() override
	{

		// デルタタイム
		m_deltaTime = 0.0f;
		m_prevTime = std::chrono::system_clock::now();
		m_baseTime = std::chrono::system_clock::now();
		count = 0;
	}

	void  Tick();

	float DeltaTime()
	{
		return m_deltaTime;
	}
	float FPS()
	{
		return m_fps;
	}

private:
	// 直前フレームからの経過時間（秒）
	float								  m_deltaTime = 0.0f;
	// 直前フレームのタイムスタンプ
	std::chrono::system_clock::time_point m_prevTime;
	//
	std::chrono::system_clock::time_point m_baseTime;
	// fps
	float								  m_fps = 0;

	int count = 0;
};
}