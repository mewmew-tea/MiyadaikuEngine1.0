#include "ScriptClassTypeInfo.h"
#include <mono/jit/jit.h>
#include "ScriptFieldInfo.h"
#include "ScriptPropertyInfo.h"
#include "ScriptMethod.h"
#include "Scripting.h"

namespace Miyadaiku
{

//==============================================
// ScriptClassTypeInfo
//==============================================
bool ScriptClassTypeInfo::ReadClass(MonoImage* _pImage)
{
	if (!_pImage)
	{
		return false;
	}
	pMonoClass = mono_class_from_name(_pImage, nameSpace.c_str(), name.c_str());
	if (!pMonoClass)
	{
		// MessageBoxA(NULL, "", "Failed loading mono class.", MB_OK);
		return false;
	}

	// get fields
	{
		MonoClass* klass = pMonoClass;
		while (true)
		{
			MonoClassField* field;
			void*			iter = nullptr;
			spFieldInfos.reserve(spFieldInfos.size()
								 + mono_class_num_fields(klass));
			while ((field = mono_class_get_fields(klass, &iter)))
			{
				printf("Field: %s, flags 0x%x\n", mono_field_get_name(field),
					   mono_field_get_flags(field));

				auto spFieldInfo = std::make_shared<ScriptFieldInfo>();
				spFieldInfo->pField = field;
				spFieldInfo->name = mono_field_get_name(field);

				// jugde type
				spFieldInfo->pMonoType = mono_field_get_type(field);
				spFieldInfo->type =
					Scripting::ConvertTypeMonoToRuntime(spFieldInfo->pMonoType);

				// get attributes
				MonoClass*			parentClass = mono_field_get_parent(field);
				MonoCustomAttrInfo* attrInfo =
					mono_custom_attrs_from_field(parentClass, field);
				if (attrInfo != nullptr)
				{
					// serach serialize field
					auto attrClass = mono_class_from_name(
						_pImage, "MiyadaikuEngine", "SerializeFieldAttribute");
					MonoObject* foundAttr =
						mono_custom_attrs_get_attr(attrInfo, attrClass);
					mono_custom_attrs_free(attrInfo);
					if (foundAttr)
					{
						spFieldInfo->isSerializeField = true;
						printf("Serialize Field Attribute!!\n");
					}
				}
				spFieldInfos.push_back(spFieldInfo);
			}

			klass = mono_class_get_parent(klass);
			if (klass == nullptr)
			{
				break;
			}
		}
	}

	// get properties
	MonoProperty* prop;
	void*		  iter = nullptr;
	spPropertyInfos.reserve(mono_class_num_properties(pMonoClass));
	while ((prop = mono_class_get_properties(pMonoClass, &iter)))
	{
		printf("Field: %s, flags 0x%x\n", mono_property_get_name(prop),
			   mono_property_get_flags(prop));

		auto spPropInfo = std::make_shared<ScriptPropertyInfo>();
		spPropInfo->pProp = prop;
		spPropInfo->name = mono_property_get_name(prop);

		// get,set method
		spPropInfo->pGetMethod = mono_property_get_get_method(prop);
		spPropInfo->pSetMethod = mono_property_get_set_method(prop);

		// jugde type
		spPropInfo->pMonoType = mono_signature_get_return_type(
			mono_method_signature(spPropInfo->pGetMethod));
		spPropInfo->type =
			Scripting::ConvertTypeMonoToRuntime(spPropInfo->pMonoType);

		// get attributes
		MonoClass*			parentClass = mono_property_get_parent(prop);
		MonoCustomAttrInfo* attrInfo =
			mono_custom_attrs_from_property(parentClass, prop);
		if (attrInfo != nullptr)
		{
			// serach serialize field
			auto attrClass = mono_class_from_name(_pImage, "MiyadaikuEngine",
												  "SerializeFieldAttribute");
			MonoObject* foundAttr =
				mono_custom_attrs_get_attr(attrInfo, attrClass);
			mono_custom_attrs_free(attrInfo);
			if (foundAttr)
			{
				spPropInfo->isSerializeField = true;
				printf("Serialize Field Attribute!!\n");
			}
		}
		spPropertyInfos.push_back(spPropInfo);
	}

	isRead = true;

	// read methods

	// method：Init
	initMethod = std::make_shared<ScriptMethod>();
	initMethod->pClassType = this;
	initMethod->name = "Init";
	initMethod->Read();
	// method：Update
	updateMethod = std::make_shared<ScriptMethod>();
	updateMethod->pClassType = this;
	updateMethod->name = "Update";
	updateMethod->Read();
	// method：ImGuiUpdate
	imguiUpdateMethod = std::make_shared<ScriptMethod>();
	imguiUpdateMethod->pClassType = this;
	imguiUpdateMethod->name = "ImGuiUpdate";
	imguiUpdateMethod->Read();

	return true;
}
std::shared_ptr<ScriptClassInstance>
ScriptClassTypeInfo::CreateInstance(MonoDomain* _pDomain)
{
	if (!pMonoClass)
	{
		return nullptr;
	}
	MonoObject* pInst;
	pInst = mono_object_new(_pDomain, pMonoClass);
	// invoke constructor
	mono_runtime_object_init(pInst);
	auto spInst = std::make_shared<ScriptClassInstance>(pInst, this);

	// new GC handle：Pin the object to IntPtr not be broken
	spInst->m_managedHandle = mono_gchandle_new(pInst, true);

	return spInst;
}

void ScriptClassTypeInfo::Serialize(nlohmann::json& _out,
									bool			_containNonSerialized)
{
	// name
	_out["name"] = name;
	_out["nameSpace"] = nameSpace;
	_out["containNonSerialized"] = _containNonSerialized;
	// fields
	nlohmann::json fields;
	for (auto& spFieldInfo : spFieldInfos)
	{
		if (!_containNonSerialized && !spFieldInfo->isSerializeField)
		{
			continue;
		}

		std::tuple<int, std::string> fieldInfo = {(int)spFieldInfo->type,
												  spFieldInfo->name};
		fields.push_back(fieldInfo);
	}
	_out["fields"] = fields;
}
//
// void ScriptClassTypeInfo::Deserialize(const nlohmann::json& _out)
//{
//	name = _out["name"];
//	nameSpace = _out["nameSpace"];
//
//	auto scripting = GetEngine()->GetSubsystemLocator().Get<Scripting>();
//	if (!ReadClass(scripting->GetMonoImage()))
//	{
//		return;
//	}
//	auto& fields = _out["fields"];
//	for (auto& spFieldInfo : spFieldInfos)
//	{
//		std::tuple<int, std::string> fieldInfo = fields[spFieldInfo->name];
//
//	}
//}

}