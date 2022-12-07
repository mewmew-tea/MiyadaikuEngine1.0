#include "Effect.h"
#include "../Core/Engine.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/RHI/D3D11/D3D11_Device.h"

#include <mono/jit/jit.h>

namespace Miyadaiku
{
void Effect::OnAwake()
{
	std::shared_ptr<Renderer> renderer =
		GetEngine()->GetSubsystemLocator().Get<Renderer>();
	if (!renderer)
	{
		assert(0 && "Renderer does not exist.");
	}
	auto device = std::static_pointer_cast<D3D11_Device>(renderer->GetRHIDevice());
	if (!device)
	{
		assert(0 && "Device does not exist.");
	}

	// Create a manager of effects
	// エフェクトのマネージャーの作成
	efkManager = ::Effekseer::Manager::Create(8000);

	//// Setup effekseer modules
	//// Effekseerのモジュールをセットアップする
	// device.SetupEffekseerModules(efkManager);
	// auto efkRenderer = device.GetEffekseerRenderer();

	// Create a  graphics device
	// 描画デバイスの作成
	auto graphicsDevice = ::EffekseerRendererDX11::CreateGraphicsDevice(
		device->GetDevice(), device->GetDeviceContext());

	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	efkRenderer =
		::EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// Specify sound modules
	// サウンドモジュールの設定
	// efkSound = ::EffekseerSound::Sound::Create(GetIXAudio2(), 16, 16);

	// Specify a metho to play sound from an instance of efkSound
	// 音再生用インスタンスから再生機能を指定
	// efkManager->SetSoundPlayer(efkSound->CreateSoundPlayer());

	//// Specify a sound data loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// サウンドデータの読込機能を設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//// efkManager->SetSoundLoader(efkSound->CreateSoundLoader());
	//auto viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	//// Specify a projection matrix
	//// 投影行列を設定
	//projectionMatrix.PerspectiveFovLH(60.0f / 180.0f * 3.14f, 1280 / 720, 1.0f,
	//								  500.0f);

	//// Specify a camera matrix
	//// カメラ行列を設定
	//cameraMatrix.LookAtLH(viewerPosition,
	//					  ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f),
	//					  ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));

	// Load an effect
	// エフェクトの読込
	effect =
		Effekseer::Effect::Create(efkManager, u"Assets/Effects/Explosion/fire.efkefc");
	if (effect.Get() == nullptr)
	{
		assert(0 && "Can't load effect");
	}

	//Internal_Play(0, 0, 5);
}
void Effect::OnShutdown()
{
	effect.Reset();
	efkRenderer.Reset();
	efkManager.Reset();
}
void Effect::OnRender()
{
	// Set layer parameters
	// レイヤーパラメータの設定
	// Effekseer::Manager::LayerParameter layerParameter;
	// layerParameter.ViewerPosition = viewerPosition;
	// efkManager->SetLayerParameter(0, layerParameter);

	// Update the manager
	Effekseer::Manager::UpdateParameter updateParameter;
	efkManager->Update(updateParameter);

	// Update a time
	efkRenderer->SetTime(time);

	// Specify a projection matrix
	efkRenderer->SetProjectionMatrix(projectionMatrix);

	// Specify a camera matrix
	efkRenderer->SetCameraMatrix(cameraMatrix);

	// Begin to rendering effects
	efkRenderer->BeginRendering();

	// Render effects
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix =
		efkRenderer->GetCameraProjectionMatrix();
	efkManager->Draw(drawParameter);

	// Finish to rendering effects
	efkRenderer->EndRendering();

	time++;

	SetUpInternalCall();
}
void Effect::SetCameraMatrix(const Matrix& _mView)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cameraMatrix.Values[i][j] = _mView.m[i][j];
		}
	}
}
void Effect::SetProjectionMatrix(const Matrix& _mProj)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			projectionMatrix.Values[i][j] = _mProj.m[i][j];
		}
	}
}
void Effect::SetUpInternalCall()
{
	mono_add_internal_call("MiyadaikuEngine.EffekseerEmitter::Internal_Play",
						   &Internal_Play);
}
int32_t Effect::Internal_Play(float x, float y, float z)
{
	std::shared_ptr<Effect> effectSubsystem =
		GetEngine()->GetSubsystemLocator().Get<Effect>();
	if (!effectSubsystem)
	{
		return 0;
	}
	auto& effect = effectSubsystem->effect;

	return effectSubsystem->efkManager->Play(effect, x, y, z);
}
} // namespace Miyadaiku