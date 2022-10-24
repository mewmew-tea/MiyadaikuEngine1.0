#include "Scripting.h"
#include <filesystem>
#include <assert.h>

#include <Windows.h>


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/exception.h>

#include <iostream>
#include <fstream>

MonoClass*		mainClass = nullptr;
//MonoMethodDesc* mainMethodDesc = nullptr;
//MonoMethodDesc* initMethodDesc = nullptr;
//MonoMethod*		ptrMainMethod = nullptr;
//MonoMethod*		ptrInitMethod = nullptr;
MonoImage*		assemblyImage = nullptr;

MonoDomain*	  domain = nullptr;
MonoDomain*	  rootDomain = nullptr;
MonoAssembly* assembly = nullptr;
MonoObject*	  ptrExObject = nullptr;

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

//========================================================


namespace Miyadaiku
{
ScriptClassType scriptClassType;


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

void LoadMonoAssembly()
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
	mono_add_internal_call("CSScript.GameConsole::PrintMessage()",
						   &PrintMessage);
	mono_add_internal_call("CSScript.GameConsole::SetPosition",
						   &ScriptingAPI_SetPosition);
	mono_add_internal_call("CSScript.GameConsole::GetKey", &GetKey);
	mono_add_internal_call("CSScript.GameConsole::DebugOutStringToVS",
						   &DebugOutStringToVS);
	mono_add_internal_call("CSScript.GameConsole::DebugOutStringLineToVS",
						   &DebugOutStringLineToVS);

	// read class
	scriptClassType.name = "CSClass";
	scriptClassType.nameSpace = "CSScript";
	scriptClassType.ReadClass(assemblyImage);
	// method：Init
	scriptClassType.initMethod->Invoke(nullptr, nullptr, nullptr);
	// method：Update
	scriptClassType.updateMethod->Invoke(nullptr, nullptr, nullptr);

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

bool mono_destory_domain_with_assisisated_assembly(
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

void ReleaseMonoAssembly()
{
	mono_destory_domain_with_assisisated_assembly(domain);
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

				auto spClassType = std::make_shared<ScriptClassType>();
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
	scriptClassType.updateMethod->Invoke(nullptr, nullptr, nullptr);

	//if (!scriptClassType.updateMethod->pMethod)
	//{
	//	MessageBoxA(NULL, "", "Method is not found.", MB_OK);
	//}
	//// ptrExObject = nullptr;
	//// mono_runtime_invoke(ptrMainMethod, nullptr, nullptr, &ptrExObject);
	//mono_runtime_invoke(scriptClassType.updateMethod->pMethod, nullptr, nullptr,
	//					&ptrExObject);
	//// Report exception
	//if (ptrExObject)
	//{
	//	MonoString* exString = mono_object_to_string(ptrExObject, nullptr);
	//	const char* exCString = mono_string_to_utf8(exString);
	//	MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK | MB_ICONERROR);
	//}
}
void Scripting::Release()
{
}
void Scripting::LoadUserAssembly(std::string_view _path)
{
}
//==============================================

ScriptMethod::ScriptMethod(ScriptClassType* _pClassType, std::string_view _name)
{
	pClassType = _pClassType;
	name = _name;
}

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
		MessageBoxA(NULL, "", "Faild loading method desc.", MB_OK);
		return false;
	}

	// Find main in mainclass
	pMethod = mono_method_desc_search_in_class(
		pMethodDesc, pClassType->pMonoClass);
	if (!pMethod)
	{
		MessageBoxA(NULL, "", "Faild loading method.", MB_OK);
		return false;
	}

	isRead = true;
	return true;
}

bool ScriptClassType::ReadClass(MonoImage* _pImage)
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

	isRead = true;

	// method：Update
	updateMethod = std::make_shared<ScriptMethod>(this, "Update");
	updateMethod->Read();
	// method：Init
	initMethod = std::make_shared<ScriptMethod>(this, "Init");
	initMethod->Read();

	return true;
}


bool ScriptMethod::Invoke(void* _instance, void** _params, MonoObject** _ret)
{
	if (!pMethod)
	{
		return false;
	}

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
		MessageBoxA(NULL, exCString, "Mono Invoke issue", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
} // namespace Miyadaiku