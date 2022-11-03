#include "Scripting.h"
#include <filesystem>
#include <assert.h>
#include <cstdarg>

#include <Windows.h>


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/attrdefs.h>

#include <iostream>
#include <fstream>
#include <cassert>

#include "../Core/Math/Vector3.h"
#include "../Core/Engine.h"

// dear imgui
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW


void DebugOutStringToVS(MonoString* str)
{
	return OutputDebugStringA(mono_string_to_utf8(str));
}

void DebugOutStringLineToVS(MonoString* str)
{
	std::string msg = mono_string_to_utf8(str);
	msg += "\n";
	return OutputDebugStringA(msg.c_str());
}
void PrintMessage()
{
	OutputDebugStringA(
		"I am C/C++ Function.こっちは日本語も行けます(;^ω^)\n");
	printf("I am C/C++ Function.こっちは日本語も行けます(;^ω^)\n");
}
void PrintMessageWithString(const char* str)
{
	printf("I am C/C++ Function. %s\n", str);
}
float x = 0;
float y = 0;

bool GetKey(char c)
{
	bool a = GetAsyncKeyState(c);
	return a;
}

void ScriptingAPI_SetPosition(float _x, float _y)
{
	x = _x;
	y = _y;
}

float ScriptingAPI_GetPositionX()
{
	return x;
}

float ScriptingAPI_GetPositionY()
{
	return y;
}

bool ScriptingAPI_ImGui_Begin(MonoString* _label)
{
	return ImGui::Begin(mono_string_to_utf8(_label));
}

void ScriptingAPI_ImGui_End()
{
	ImGui::End();
}

void ScriptingAPI_ImGui_Text(MonoString* _str)
{
	char* str = mono_string_to_utf8(_str);
	ImGui::Text(str);
}

//========================================================


namespace Miyadaiku
{
std::shared_ptr<ScriptClassTypeInfo> spScriptClassType;
//std::shared_ptr<ScriptClassInstance> spScriptClassInstance;


static std::string execute_command(const char* cmd)
{
	std::array<char, 1024>							buffer();
	std::string										result;
	const std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"),
														 _pclose);
	if (!pipe)
	{
		assert("popen() failed");
		return result;
	}
	char data[1024] = {};
	while (fgets(data, static_cast<int>(1024), pipe.get()) != nullptr)
	{
		result += data;
	}

	return result;
}

void CopyAssemblyFile()
{
	std::filesystem::path assembly_path_from =
		"../CSScript/bin/x64/Debug/CSScript.dll";
	// std::filesystem::path assembly_path_from = "../bin/Debug/CSScript.dll";
	std::filesystem::path assembly_path = "./Assets/CSScript.dll";
	if (std::filesystem::exists(assembly_path_from))
	{

		if (std::filesystem::exists(assembly_path))
			std::filesystem::remove(assembly_path);

		std::filesystem::copy(assembly_path_from, assembly_path);
	}
	else
		MessageBoxA(NULL, "オリジナルパスが", "おかしい", MB_OK);
}

void Scripting::LoadMonoAssembly()
{

	static bool isCalled = false;

	// domain = mono_domain_create_appdomain(NULL, NULL);
	// mono_domain_set(domain, false);
	if (!isCalled)
	{
		// rootDomain作る

		mono_set_dirs(".\\MonoAssembly\\bin\\", ".\\MonoAssembly\\etc\\");
		rootDomain = mono_jit_init("Root Domain");
		isCalled = true;
	}
	// mono_set_dirs("C:\\Program Files\\Mono\\bin\\", "C:\\Program
	// Files\\Mono\\etc\\"); mono_set_dirs(".", "."); mono_version
	/*static int cnt = 0;
	std::string name = "CSScriptTest" + std::to_string(cnt);
	domain = mono_jit_init(name.c_str());
	cnt++;*/
	// Current directory as mono dir

	// mono_set_dirs(".\\MonoAssembly\\bin\\", ".\\MonoAssembly\\etc\\");

	// domain = mono_jit_init("CSScriptTest");
	domain =
		mono_domain_create_appdomain(const_cast<char*>("CSScriptTest"), NULL);

	mono_domain_set(domain, 0);
	// MSVCは以外はまだ未対応
	// assembly = mono_domain_assembly_open(domain,
	// ".\\MonoAssembly\\CSScript.dll");
	assembly = mono_domain_assembly_open(domain, "./Assets/CSScript.dll");
	// Loading mono image
	assemblyImage = mono_assembly_get_image(assembly);

	
	// add InternalCalls
	mono_add_internal_call("MiyadaikuEngine.GameConsole::PrintMessage()",
						   &PrintMessage);
	mono_add_internal_call("MiyadaikuEngine.GameConsole::SetPosition",
						   &ScriptingAPI_SetPosition);
	mono_add_internal_call("MiyadaikuEngine.GameConsole::GetKey", &GetKey);
	mono_add_internal_call("MiyadaikuEngine.GameConsole::DebugOutStringToVS",
						   &DebugOutStringToVS);
	mono_add_internal_call("MiyadaikuEngine.GameConsole::DebugOutStringLineToVS",
						   &DebugOutStringLineToVS);

	mono_add_internal_call("MiyadaikuEngine.ImGui::Begin",
						   &ScriptingAPI_ImGui_Begin);
	mono_add_internal_call("MiyadaikuEngine.ImGui::End",
						   &ScriptingAPI_ImGui_End);
	mono_add_internal_call("MiyadaikuEngine.ImGui::Text_Internal",
						   &ScriptingAPI_ImGui_Text);

	GameObject::SetUpInternalCall();
	Component::SetUpInternalCall();

	// read class
	{
		auto spType = std::make_shared<ScriptClassTypeInfo>();
		spType->name = "GameObject";
		spType->nameSpace = "MiyadaikuEngine";
		if (spType->ReadClass(assemblyImage))
		{
			m_spScriptClasses.push_back(spType);
			m_spGameObjectTypeInfo = spType;
		}
	}

	// read class
	auto spTransformType = std::make_shared<ScriptClassTypeInfo>();
	spTransformType->name = "Transform";
	spTransformType->nameSpace = "MiyadaikuEngine";
	if (spTransformType->ReadClass(assemblyImage))
	{
		m_spScriptClasses.push_back(spTransformType);
		m_spTransformTypeInfo = spTransformType;
		//// create a instance of the class
		//spScriptClassInstance = scriptClassType.CreateInstance(domain);
		//// method：Init
		//spScriptClassInstance->InvokeInitMethod();
		//// method：Update
		//spScriptClassInstance->InvokeUpdateMethod();
	}
	spScriptClassType = std::make_shared<ScriptClassTypeInfo>();
	spScriptClassType->name = "Game";
	spScriptClassType->nameSpace = "CSScript";
	if (spScriptClassType->ReadClass(assemblyImage))
	{
		m_spScriptClasses.push_back(spScriptClassType);
		//// create a instance of the class
		// spScriptClassInstance = scriptClassType.CreateInstance(domain);
		//// method：Init
		// spScriptClassInstance->InvokeInitMethod();
		//// method：Update
		// spScriptClassInstance->InvokeUpdateMethod();
	}

	auto spGame2= std::make_shared<ScriptClassTypeInfo>();
	spGame2->name = "Game2";
	spGame2->nameSpace = "CSScript";
	if (spGame2->ReadClass(assemblyImage))
	{
		m_spScriptClasses.push_back(spGame2);
		//// create a instance of the class
		// spScriptClassInstance = scriptClassType.CreateInstance(domain);
		//// method：Init
		// spScriptClassInstance->InvokeInitMethod();
		//// method：Update
		// spScriptClassInstance->InvokeUpdateMethod();
	}


	// guid provider
	guidProviderClassType.name = "GuidProvider";
	guidProviderClassType.nameSpace = "MiyadaikuEngine";
	if (guidProviderClassType.ReadClass(assemblyImage))
	{
		guidProviderMethod_NewGuid.pClassType = &guidProviderClassType;
		guidProviderMethod_NewGuid.name = "NewGuid";
		if (guidProviderMethod_NewGuid.Read())
		{
		}
	}


	auto go = CreateGameObject();
	go->AddComponent(spScriptClassType);
	auto go2 = CreateGameObject();
	go2->AddComponent(spGame2);
	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Init();
	}


	//
	// if (rootDomain)
	//{
	//	mono_domain_unload(rootDomain);
	//	rootDomain = nullptr;
	//}
	// GetKey(1);
	// int val = 0;
	// MonoClassField* field = mono_class_get_field_from_name(mainClass, "val");
}

bool Scripting::DestoryDomainWithAssisisatedAssembly(
	MonoDomain* domain_to_destroy)
{
	if (domain_to_destroy != rootDomain)
	{
		mono_domain_set(rootDomain, 0);
		mono_domain_unload(domain_to_destroy);
		return true;
	}
	return false;
}

void Scripting::ReleaseMonoAssembly()
{
	DestoryDomainWithAssisisatedAssembly(domain);
	// ptrMainMethod = nullptr;
	// mono_image_close(assemblyImage);
	//// Free desc
	// mono_method_desc_free(mainMethodDesc);
	////mono_jit_cleanup(domain);
	// MonoDomain* domainToUnload = mono_domain_get();
	// if (domain && domain != mono_get_root_domain())
	//{
	//	mono_domain_set(mono_get_root_domain(), false);
	//	//mono_thread_pop_appdomain_ref();
	// }
	// mono_domain_unload(domain);
	////mono_jit_cleanup(domain);
	// domain = nullptr;
}

void Scripting::OnAwake()
{
	std::ifstream ifs("./Assets/CSFileList.txt");
	std::string	  str;

	LoadMonoAssembly();

	return;
	if (ifs.fail())
	{
		std::cerr << "Failed to open file." << std::endl;
		// return -1;
	}
	else
	{
		while (getline(ifs, str))
		{
			// 「.」や「,」が含まれるものは除外
			if (str.find(".") == std::string::npos
				&& str.find(",") == std::string::npos
				&& str.find("AssemblyInfo") == std::string::npos)
			{
				str = str.erase(str.find(" "));
				std::cout << "#" << str << "#" << std::endl;

				auto spClassType = std::make_shared<ScriptClassTypeInfo>();
				spClassType->name = str;
				spClassType->ReadClass(assemblyImage);
			}
		}
	}
}
void Scripting::OnShutdown()
{
}
void Scripting::Update()
{
	//scriptClassType.updateMethod->Invoke(nullptr, nullptr, nullptr);
	//spScriptClassInstance->InvokeUpdateMethod();

	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->Update();
	}
}
void Scripting::ImGuiUpdate()
{

	for (auto& spGameObject : m_spGameObjects)
	{
		spGameObject->ImGuiUpdate();
	}


	if (ImGui::Begin("C# Scripting Debug"))
	{
		if (ImGui::Button("Reload script"))
		{
		}

		for (auto& spGameObject : m_spGameObjects)
		{
			for (auto& spComp : spGameObject->m_spComponents)
			{
				auto spClassInstance = spComp->GetClassInstance();
				ImGui::Text("【%s】",
							spClassInstance->m_pClassType->name.c_str());
				for (auto& fieldInfo :
					 spClassInstance->m_pClassType->spFieldInfos)
				{
					switch (fieldInfo->type)
					{
					case ScriptValueType::Float:
					{
						float value = 0;
						fieldInfo->GetValue(spClassInstance->m_pInstance,
											&value);
						ImGui::Text("field name: %s = %3f\n",
									fieldInfo->name.c_str(), value);
						break;
					}
					case ScriptValueType::Vector3:
					{
						Vector3 value;
						fieldInfo->GetValue(spClassInstance->m_pInstance,
											&value);
						ImGui::Text("field name: %s = %3f, %3f, %3f\n",
									fieldInfo->name.c_str(), value.x, value.y,
									value.z);
						break;
					}
					default:
						ImGui::Text("field name: %s is unsupported.\n",
									fieldInfo->name.c_str());
						break;
					}
				}
			}
		}
	}
	ImGui::End();
}
void Scripting::Release()
{
}
void Scripting::LoadUserAssembly(std::string_view _path)
{
}

//==============================================
// ScriptMethod
//==============================================
//ScriptMethod::ScriptMethod(ScriptClassTypeInfo* _pClassType,
//						   std::string_view		_name)
//{
//	pClassType = _pClassType;
//	name = _name;
//}

bool ScriptMethod::Read()
{
	if (!pClassType || !pClassType->isRead || name == "")
	{
		return false;
	}

	std::string methodName =
		pClassType->name + ":" + name;
	pMethodDesc = mono_method_desc_new(methodName.c_str(), false);
	if (!pMethodDesc)
	{
		//MessageBoxA(NULL, "", "Faild loading method desc.", MB_OK);
		return false;
	}

	pMethod = mono_method_desc_search_in_class(
		pMethodDesc, pClassType->pMonoClass);
	if (!pMethod)
	{
		//MessageBoxA(NULL, "", "Faild loading method.", MB_OK);
		return false;
	}

	isRead = true;
	return true;
}

ScriptValueType Scripting::ConvertTypeMonoToRuntime(MonoType* _pMonoType)
{
	std::string typeName = mono_type_get_name(_pMonoType);
	if (typeName == "System.Int32")
	{
		return ScriptValueType::Int;
	}
	else if (typeName == "System.Single")
	{
		return ScriptValueType::Float;
	}
	else if (typeName == "System.Boolean")
	{
		return ScriptValueType::Bool;
	}
	else if (typeName == "System.String")
	{
		return ScriptValueType::String;
	}
	else if (typeName == "MiyadaikuEngine.Vector3")
	{
		return ScriptValueType::Vector3;
	}
	// unsupported types
	else
	{
		return ScriptValueType::Other;
	}
}
unsigned int Scripting::GetSizeFromValueType(ScriptValueType _type)
{
	switch (_type)
		{
		case Miyadaiku::ScriptValueType::Int:
			return sizeof(int32_t);
			break;
		case Miyadaiku::ScriptValueType::Float:
			return sizeof(float);
			break;
		case Miyadaiku::ScriptValueType::Bool:
			return sizeof(bool);
			break;
		case Miyadaiku::ScriptValueType::String:
			return 0;
			break;
		case Miyadaiku::ScriptValueType::Vector2:
			return sizeof(float) * 2;
			break;
		case Miyadaiku::ScriptValueType::Vector3:
			return sizeof(float) * 3;
			break;
		case Miyadaiku::ScriptValueType::Vector4:
			return sizeof(float) * 4;
			break;
		case Miyadaiku::ScriptValueType::Quaternion:
			return sizeof(float) * 4;
			break;
		case Miyadaiku::ScriptValueType::Other:
			return 0;
			break;
		default:
			return 0;
			break;
		}
}

std::string Scripting::GenerateGUID()
{
	MonoObject* ret = nullptr;
	guidProviderMethod_NewGuid.Invoke(nullptr, nullptr, &ret);
	MonoString* str = nullptr;
	str = (MonoString*)ret;
	std::string guid = mono_string_to_utf8(str);

	return guid;
}

MonoDomain* Scripting::GetMonoDomain()
{
	return domain;
}

std::list<std::shared_ptr<GameObject>>& Scripting::GetGameObjects()
{
	return m_spGameObjects;
}

std::shared_ptr<GameObject> Scripting::CreateGameObject()
{
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
	auto scriptInst = m_spGameObjectTypeInfo->CreateInstance(domain);
	gameObject->m_spClassInstance = scriptInst;
	auto transform = gameObject->AddComponent(m_spTransformTypeInfo);
	gameObject->m_spTransform = transform;

	// setup instance
	for (auto field : scriptInst->m_pClassType->spFieldInfos)
	{
		if (field->name == "chachedPtr")
		{
			void* value = gameObject.get();
			field->SetValue(scriptInst->m_pInstance, &value);	
		}
	}
	m_spGameObjects.push_back(gameObject);

	return gameObject;
}


//==============================================
// ScriptFieldInfo
//==============================================
bool ScriptFieldInfo::IsStatic()
{
	return (mono_field_get_flags(pField) & MONO_FIELD_ATTR_STATIC) != 0;
}

void ScriptFieldInfo::SetValue(MonoObject* _pInstance, void* _pValue)
{
	if (!_pInstance || !_pValue)
	{
		return;
	}
	auto spScripting = GetEngine()->GetSubsystemLocator().Get<Scripting>();
	auto pMonoDomain = spScripting->GetMonoDomain();

	MonoType* pMonoType = mono_field_get_type(pField);
	if (mono_type_is_reference(pMonoType))
	{
		MonoString* str = mono_string_new(pMonoDomain, (char*)_pValue);
		mono_field_set_value(_pInstance, pField, str);
	}
	else
	{
		mono_field_set_value(_pInstance, pField, _pValue);		
	}
}

void ScriptFieldInfo::GetValue(MonoObject* _pInstance, void* _pOutValue)
{
	if (!_pInstance || !_pOutValue)
	{
		return;
	}

	MonoObject*	  ret;
	pMonoType = mono_field_get_type(pField);
	mono_field_get_value(_pInstance, pField, _pOutValue);

	//
	//if (mono_type_is_reference(pMonoType))
	//{
	//	if (type == ScriptVaueType::String)
	//	{
	//		// stirng:
	//		_pOutValue = mono_string_to_utf8((MonoString*)ret);
	//	}
	//}
	//else 
	if (mono_type_is_reference(pMonoType))
	{
		assert(0 && "Assertion: Reference types aren't unsupported. Do for each member of the class.");
	}
	// value type
	else
	{
		//_pOutValue = ret;
	}
}

//==============================================
// ScriptClassTypeInfo
//==============================================
bool ScriptClassTypeInfo::ReadClass(MonoImage* _pImage)
{
	if (!_pImage)
	{
		return false;
	}
	pMonoClass = mono_class_from_name(_pImage, nameSpace.c_str(),
							 name.c_str());
	if (!pMonoClass)
	{
		//MessageBoxA(NULL, "", "Failed loading mono class.", MB_OK);
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
std::shared_ptr<ScriptClassInstance> ScriptClassTypeInfo::CreateInstance(MonoDomain* _pDomain)
{
	if (!pMonoClass)
	{
		return nullptr;		
	}
    MonoObject* pInst;
	pInst = mono_object_new(_pDomain, pMonoClass);
	mono_runtime_object_init(pInst);
	auto spInst = std::make_shared<ScriptClassInstance>(pInst, this);
	return spInst;
}

//==============================================
// ScriptMethod
//==============================================
bool ScriptMethod::Invoke(void* _instance, void** _params, MonoObject** _ret)
{
	if (!pMethod)
	{
		return false;
	}
	MonoObject* ptrExObject = nullptr;

	if (!_ret)
	{
		mono_runtime_invoke(pMethod, _instance, _params, &ptrExObject);
	}
	else
	{
		*_ret = mono_runtime_invoke(pMethod, _instance, _params, &ptrExObject);
	}
	// Report exception
	if (ptrExObject)
	{
		MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
		const char* exCString = mono_string_to_utf8(exString);
		//MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

//==============================================
// ScriptClassInstance
//==============================================
ScriptClassInstance::ScriptClassInstance(MonoObject*			_pInst,
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

	return m_pClassType->imguiUpdateMethod->Invoke(m_pInstance, nullptr, nullptr);
}

bool ScriptPropertyInfo::IsStatic()
{
	return false;
}
void ScriptPropertyInfo::SetValue(MonoObject* _pInstance, void* _pValue)
{

	if (!pSetMethod)
	{
		return;
	}
	MonoObject* ptrExObject = nullptr;

	void* args[1];
	args[0] = _pValue;
	mono_runtime_invoke(pSetMethod, _pInstance, args, &ptrExObject);
	// Report exception
	if (ptrExObject)
	{
		MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
		const char* exCString = mono_string_to_utf8(exString);
		// MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK |
		// MB_ICONERROR);
		return;
	}
}
void ScriptPropertyInfo::GetValue(MonoObject* _pInstance, void* _pOutValue)
{
	if (!pGetMethod || !_pOutValue)
	{
		return;
	}
	MonoObject* ptrExObject = nullptr;

	MonoObject* ret = mono_runtime_invoke(pGetMethod, _pInstance, nullptr,
								  &ptrExObject);

	// Report exception
	if (ptrExObject)
	{
		MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
		const char* exCString = mono_string_to_utf8(exString);
		// MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK |
		// MB_ICONERROR);
		return;
	}

	//
	if (mono_type_is_reference(pMonoType))
	{
		if (type == ScriptValueType::String)
		{
			// stirng:
			_pOutValue = mono_string_to_utf8((MonoString*)ret);
		}
	}
	// if it's a valuetype, mono_runtime_invoke() always boxes the return value
	else
	{
		MonoObject* unboxed = nullptr;
		unboxed = (MonoObject*)mono_object_unbox(ret);
		int size = 0;
		size = Scripting::GetSizeFromValueType(type);
		//size = mono_object_get_size(ret);
		//printf("size: %d\n", size);
		std::memcpy(_pOutValue, unboxed, size);
	}
}
void Component::SetUpInternalCall()
{
	mono_add_internal_call("MiyadaikuEngine.Component::Internal_GetGameObject",
						   &Internal_GetGameObject);
}
MonoObject* Component::Internal_GetGameObject(Component* _chachedPtr)
{
	return _chachedPtr->m_spOwner->GetClassInstance()->m_pInstance;
}
} // namespace Miyadaiku