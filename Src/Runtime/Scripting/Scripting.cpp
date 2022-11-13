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

#include "../Core/Math/Vector2.h"
#include "../Core/Math/Vector3.h"
#include "../Core/Math/Vector4.h"
#include "../Core/Math/Quaternion.h"
#include "../Core/Math/Matrix.h"

#include "../Core/Engine.h"

#include "Component.h"
#include "ScriptFieldInfo.h"

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
	//OutputDebugStringA(
	//	"I am C/C++ Function.こっちは日本語も行けます(;^ω^)\n");
	//printf("I am C/C++ Function.こっちは日本語も行けます(;^ω^)\n");
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

	// serialize test
#include <ostream>
	{
		nlohmann::ordered_json jsonData;
		spGame2->Serialize(jsonData);
		std::ofstream writing_file;
		std::string	  filename = "typeInfo.json";
		writing_file.open(filename, std::ios::out);
		std::string writing_text = jsonData.dump();
		writing_file << writing_text << std::endl;
		writing_file.close();
	}
	{
		nlohmann::ordered_json jsonData;
		for (auto& spComp : go2->m_spComponents)
		{
			if (spComp->m_spClassInstance->m_pClassType->name == spGame2->name)
			{
				spComp->m_spClassInstance->Serialize(jsonData);
				break;
			}
		}
		std::ofstream writing_file;
		std::string	  filename = "typeData.json";
		writing_file.open(filename, std::ios::out);
		std::string writing_text = jsonData.dump();
		writing_file << writing_text << std::endl;
		writing_file.close();
	}

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
	else if (typeName == "System.IntPtr")
	{
		return ScriptValueType::IntPtr;
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
	case Miyadaiku::ScriptValueType::IntPtr:
		return sizeof(void*);
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

MonoImage* Scripting::GetMonoImage()
{
	return assemblyImage;
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
	for (auto& field : scriptInst->m_pClassType->spFieldInfos)
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

} // namespace Miyadaiku