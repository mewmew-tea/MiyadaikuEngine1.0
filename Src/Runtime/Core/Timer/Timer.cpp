#include "Timer.h"
#include <Windows.h>
#include <cstdio>

namespace Miyadaiku
{
void Timer::Tick()
{
	// 最大FPS
	constexpr int						  kMaxFps = 60;
	// １秒あたりのm秒
	constexpr int						  kOneSecondInMS = 1000000;
	//constexpr int						  kOneSecondInMS = 1000;
	// 計測のベース時間からの経過時間
	float								  elapsedTimeFromBaseTime;
	//------------------------------
	// デルタタイム算出
	//------------------------------
	std::chrono::system_clock::time_point nowTime =
		std::chrono::system_clock::now();
	float deltaTimeMs = std::chrono::duration_cast<std::chrono::microseconds>(
							nowTime - m_prevTime)
							.count();

	//------------------------------
	// FPS計測
	//------------------------------
	// FPS計測(誤差が大きいシンプルバージョン、1F間のデルタタイムだけから算出)
	// m_fps = 1 / m_deltaTime;
	// FPS計測(高精度バージョン、一定時間での経過フレームを計測)
	++count; // FPS計測用のフレームカウントを加算
	// 計測のベース時間からの経過時間
	elapsedTimeFromBaseTime =
		std::chrono::duration_cast<std::chrono::microseconds>(nowTime
															  - m_baseTime)
			.count();
	// 経過時間が１秒を超えた地点で、「１秒間」としてFPS計算
	if (elapsedTimeFromBaseTime >= kOneSecondInMS)
	{
		// FPS算出
		m_fps = float(count * kOneSecondInMS) / elapsedTimeFromBaseTime;
		// FPS計測のベース時間を、今の時間にリセット
		m_baseTime = nowTime;
		// FPS計測用のフレームカウントをリセット
		count = 0;
	}

	//-------------------------------------
	// デルタタイムをmsからsに変換
	//-------------------------------------
	m_deltaTime = deltaTimeMs / float(kOneSecondInMS);
	m_prevTime = nowTime;

	//=========================================
	//
	// フレームレート制御
	//
	//=========================================

	// 最大FPSを越えたら待機
	DWORD ms = 1000 / kMaxFps;
	if (m_deltaTime * 1000 < ms)
	{
		// std::this_thread::sleep_for(
		//	std::chrono::milliseconds(ms - (m_deltaTime)));
		Sleep(ms - (m_deltaTime * 1000)); // 速すぎたら待つ
	}

	//printf("fps : %f\n", m_fps);
}

} // namespace Miyadaiku