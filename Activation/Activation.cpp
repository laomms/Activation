#include "pch.h"
#include <Windows.h>
#include "Activation.h"
#include "slpublicfunc.h"


using namespace Activation;
using namespace System;
using namespace std;

NativeClass::NativeClass() { }
NativeClass::~NativeClass() { }

Activation::Class1::Class1(): m_native(new NativeClass) { }
Activation::Class1::~Class1() { delete m_native; }

void MarshalString(String^ s, wstring& os) {
	using namespace Runtime::InteropServices;
	const wchar_t* chars =	(const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}
int Activation::Class1::InstallProductKeys(String^ ProductKeys, ManagedCallbackHandler^ PrintString)
{
	wstring wProductKeys;
	MarshalString(ProductKeys, wProductKeys);
	return slpublicfunc::InstallProductKey(wProductKeys, PrintString);
}

