﻿#pragma once

#include "../Core/Subsystem.h"
#include "../Core/Math/Matrix.h"

#define _WINSOCKAPI_
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include <stdint.h>

namespace Miyadaiku
{
	class Effect : public Subsystem
	{
	public:
		void OnAwake() override;
		void OnShutdown() override;

		void OnRender();

		void SetCameraMatrix(const Matrix& _mView);
		void SetProjectionMatrix(const Matrix& _mProj);

		static void SetUpInternalCall();
	private:
		static int32_t Internal_Play(float x, float y, float z);


		EffekseerRendererDX11::RendererRef efkRenderer;
		Effekseer::ManagerRef			   efkManager;
		Effekseer::EffectRef			   effect;

		Effekseer::Handle	  efkHandle = 0;
		int32_t				  time = 0;
		::Effekseer::Matrix44 projectionMatrix;
		::Effekseer::Matrix44 cameraMatrix;
	};
}