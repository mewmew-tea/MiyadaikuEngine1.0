#pragma once
#include "ScriptCommon.h"
#include <nlohmann/json.hpp>

namespace Miyadaiku
{
struct ScriptClassTypeInfo;

class ScriptClassInstance
{
public:
	ScriptClassInstance(MonoObject* _pInst, ScriptClassTypeInfo* _pClassType);
	MonoObject*			 m_pInstance = nullptr;
	ScriptClassTypeInfo* m_pClassType = nullptr;

	bool InvokeInitMethod();
	bool InvokeUpdateMethod();
	bool InvokeImGuiUpdateMethod();

	void Serialize(nlohmann::json& _out, bool _containNonSerialized = true);
};

}