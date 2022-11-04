#include "ScriptClassInstance.h"
#include "ScriptClassTypeInfo.h"
#include "ScriptFieldInfo.h"
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

void ScriptClassInstance::Serialize(nlohmann::json& _out,
									bool			_containNonSerialized)
{
	_out["name"] = m_pClassType->name;
	nlohmann::json fieldDatas;
	for (auto& spFieldInfo : m_pClassType->spFieldInfos)
	{
		if (!_containNonSerialized && !spFieldInfo->isSerializeField)
		{
			continue;
		}

		switch (spFieldInfo->type)
		{
		case Miyadaiku::ScriptValueType::Int:
		{
			int data = 0;
			spFieldInfo->GetValue(m_pInstance, &data);
			fieldDatas.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::Float:
		{
			float data = 0.0f;
			spFieldInfo->GetValue(m_pInstance, &data);
			fieldDatas.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::Bool:
		{
			bool data = false;
			spFieldInfo->GetValue(m_pInstance, &data);
			fieldDatas.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::String:
		{
			std::string data = "";
			spFieldInfo->GetValue(m_pInstance, &data);
			fieldDatas.push_back(data);
		}
		break;
		case Miyadaiku::ScriptValueType::IntPtr:
		{
			void* data = nullptr;
			spFieldInfo->GetValue(m_pInstance, &data);
			fieldDatas.push_back((int32_t)data);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector2:
		{
			Vector2 data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y};
			fieldDatas.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector3:
		{
			Vector3 data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z};
			fieldDatas.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Vector4:
		{
			Vector4 data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z, data.w};
			fieldDatas.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Quaternion:
		{
			Quaternion data;
			spFieldInfo->GetValue(m_pInstance, &data);
			nlohmann::json dataArray = {data.x, data.y, data.z, data.w};
			fieldDatas.push_back(dataArray);
		}
		break;
		case Miyadaiku::ScriptValueType::Other:
			fieldDatas.push_back(0);
			break;
		default:
			fieldDatas.push_back(0);
			break;
		}
	}
	_out["fieldDatas"] = fieldDatas;
}

}