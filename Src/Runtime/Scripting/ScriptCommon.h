#pragma once
#include <cstdint>
typedef struct _MonoClass	  MonoClass;
typedef struct _MonoAssembly  MonoAssembly;
typedef struct _MonoImage	  MonoImage;
typedef struct MonoMethodDesc MonoMethodDesc;
typedef struct _MonoMethod	  MonoMethod;

typedef struct _MonoType MonoType;

typedef struct _MonoObject MonoObject;

typedef struct _MonoDomain	MonoDomain;
typedef struct _MonoJitInfo MonoJitInfo;

typedef struct MonoVTable MonoVTable;

typedef struct _MonoClassField MonoClassField;
typedef struct _MonoProperty   MonoProperty;
typedef struct _MonoEvent	   MonoEvent;

namespace Miyadaiku
{

enum class ScriptValueType : uint16_t
{
	// built-in types
	Int = 0,
	Float = 10,
	Bool = 20,
	String = 30,
	IntPtr = 50,
	// MiyadaikuEngine types
	Vector2 = 100,
	Vector3,
	Vector4,
	Quaternion,
	// unsupported types
	Other = UINT16_MAX,
};

}