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
	~ScriptClassInstance();

	MonoObject* m_pInstance = nullptr;
	/**
	 * GC handle of managed instance
	 */
	uint32_t	m_managedHandle = 0;

	ScriptClassTypeInfo* m_pClassType = nullptr;

	bool InvokeInitMethod();
	bool InvokeUpdateMethod();
	bool InvokeImGuiUpdateMethod();

	/**
	 * Free GC handle of managed instance
	 */
	void FreeManagedHandle();

	void Serialize(nlohmann::json& _out, bool _containNonSerialized = true);
};

}