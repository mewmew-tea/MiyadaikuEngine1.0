#include "ScriptClassInstance.h"
#include "ScriptClassTypeInfo.h"
#include "ScriptFieldInfo.h"
#include "ScriptPropertyInfo.h"
#include "ScriptMethod.h"


#include "../Core/Math/Vector2.h"
#include "../Core/Math/Vector3.h"
#include "../Core/Math/Vector4.h"
#include "../Core/Math/Quaternion.h"
#include "../Core/Math/Matrix.h"

#include <mono/jit/jit.h>

namespace Miyadaiku
{

//==============================================
// ScriptClassInstance
//==============================================
ScriptClassInstance::ScriptClassInstance(MonoObject*		  _pInst,
										 ScriptClassTypeInfo* _pClassType)
{
	m_pInstance = _pInst;
	m_pClassType = _pClassType;
}

ScriptClassInstance::~ScriptClassInstance()
{
	FreeManagedHandle();
}

bool ScriptClassInstance::InvokeInitMethod()
{
	if (!m_pInstance || !m_pClassType || !m_pClassType->initMethod)
	{
		return false;
	}

	return m_pClassType->initMethod->Invoke(m_pInstance, nullptr, nullptr);
}

bool ScriptClassInstance::InvokeUpdateMethod()
{
	if (!m_pInstance || !m_pClassType || !m_pClassType->updateMethod)
	{
		return false;
	}

	return m_pClassType->updateMethod->Invoke(m_pInstance, nullptr, nullptr);
}
bool ScriptClassInstance::InvokeImGuiUpdateMethod()
{
	if (!m_pInstance || !m_pClassType || !m_pClassType->imguiUpdateMethod)
	{
		return false;
	}

	return m_pClassType->imguiUpdateMethod->Invoke(m_pInstance, nullptr,
												   nullptr);
}

void ScriptClassInstance::FreeManagedHandle()
{
	if (m_managedHandle == 0)
	{
		return;
	}

	mono_gchandle_free(m_managedHandle);
	m_managedHandle = 0;
}

void ScriptClassInstance::Serialize(nlohmann::ordered_json& _out,
									bool			_containNonSerialized)
{
	_out["name"] = m_pClassType->name;
	nlohmann::ordered_json values;
	// Fields
	for (auto& spFieldInfo : m_pClassType->spFieldInfos)
	{
		if (!_containNonSerialized && !spFieldInfo->isSerializeField)
		{
			continue;
		}
		// ignore property's internal fields. (they begin with '<')
		if (spFieldInfo->name[0] == '<')
		{
			continue;
		}

		switch (spFieldInfo->type)
		{
		case Miyadaiku::ScriptValueType::Int:
		{
			int data = 0;
			spFieldInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::Float:
		{
			float data = 0.0f;
			spFieldInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::Bool:
		{
			bool data = false;
			spFieldInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::String:
		{
			std::string data = "";
			spFieldInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::IntPtr:
		{
			void* data = nullptr;
			spFieldInfo->GetValue(m_pInstance, &data);
			values.push_back((int32_t)data);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector2:
		{
			Vector2 data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector3:
		{
			Vector3 data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector4:
		{
			Vector4 data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z, data.w};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Quaternion:
		{
			Quaternion data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z, data.w};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Other:
			values.push_back(0);
			break;
		default:
			values.push_back(0);
			break;
		}
	}
	// props
	for (auto& spPropertyInfo : m_pClassType->spPropertyInfos)
	{
		if (!_containNonSerialized && !spPropertyInfo->isSerializeField)
		{
			continue;
		}
		// ignore property's internal fields. (they begin with '<')
		if (spPropertyInfo->name[0] == '<')
		{
			continue;
		}

		switch (spPropertyInfo->type)
		{
		case Miyadaiku::ScriptValueType::Int:
		{
			int data = 0;
			spPropertyInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::Float:
		{
			float data = 0.0f;
			spPropertyInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::Bool:
		{
			bool data = false;
			spPropertyInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::String:
		{
			std::string data = "";
			spPropertyInfo->GetValue(m_pInstance, &data);
			values.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::IntPtr:
		{
			void* data = nullptr;
			spPropertyInfo->GetValue(m_pInstance, &data);
			values.push_back((int32_t)data);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector2:
		{
			Vector2 data;
			spPropertyInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector3:
		{
			Vector3 data;
			spPropertyInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector4:
		{
			Vector4 data;
			spPropertyInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z, data.w};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Quaternion:
		{
			Quaternion data;
			spPropertyInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z, data.w};
			values.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Other:
			values.push_back(0);
			break;
		default:
			values.push_back(0);
			break;
		}
	}

	_out["values"] = values;
}

}