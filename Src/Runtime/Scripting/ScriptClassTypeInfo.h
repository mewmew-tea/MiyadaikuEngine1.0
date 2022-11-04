#pragma once

#include "ScriptCommon.h"
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

namespace Miyadaiku
{
struct ScriptFieldInfo;
struct ScriptPropertyInfo;
struct ScriptMethod;
class ScriptClassInstance;

/**
 * Class type info of script
 */
struct ScriptClassTypeInfo
{
	/**
	 * read from assembly.
	 *
	 * @return Is successful reading class
	 */
	bool ReadClass(MonoImage* _pImage);

	/**
	 * Create instance of this class type
	 * 
	 * @param _pDomain domain of mono
	 * @return generated instance
	 */
	std::shared_ptr<ScriptClassInstance> CreateInstance(MonoDomain* _pDomain);


	/**
	 * Serialize this class info.
	 * This function is mainly for editor.
	 * 
	 * @param _out json object for output serialized data
	 * @param _containNonSerialized contain non-serialized data in serialized data
	 */
	void Serialize(nlohmann::json& _out, bool _containNonSerialized = true);
	// void Deserialize(const nlohmann::json& _in);

	std::string name = "";
	std::string nameSpace = "";
	// std::string guid = "";

	bool	   isRead = false;
	bool	   isComponent = false;
	MonoClass* pMonoClass = nullptr;

	std::vector<std::shared_ptr<ScriptFieldInfo>>	 spFieldInfos;
	std::vector<std::shared_ptr<ScriptPropertyInfo>> spPropertyInfos;

	std::shared_ptr<ScriptMethod> initMethod;
	std::shared_ptr<ScriptMethod> updateMethod;
	std::shared_ptr<ScriptMethod> imguiUpdateMethod;
};

}