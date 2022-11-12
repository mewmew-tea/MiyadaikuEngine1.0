#pragma once

//================================================
// Includes
//================================================
#include <memory>

//================================================
namespace Miyadaiku
{
    class Engine;
}
//================================================

//================================================
class Application
{
public:
    Application();
    ~Application();

    //----------------------------------------------------
    // ゲームのライフサイクル
    // SetUp > Iteration > CleanUpの順で呼びだします。
	//----------------------------------------------------

    /**
     * @brief エンジンを初期化.
     * 
     * @param _isEnableStartUpAsEditor エディタモードとして起動するようにする。
     *          Allow engine to boot as editor mode.
     * @return 
     */
	int Setup(bool _isEnableStartUpAsEditor);
    
    // メインループを実行
    // 終了のリクエストがあるまで続きます。
	void Iteration();

    // エンジンを終了
	void Cleanup();

#if TESTS_ENABLED
	//void Tests_Setup();
#endif

private:
    std::unique_ptr<Miyadaiku::Engine> m_engine;
};
