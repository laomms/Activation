#include "pch.h"
#include <Windows.h>
#include "Activation.h"
#include "slpublicfunc.h"
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace Activation;
using namespace System;
using namespace std;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

NativeClass::NativeClass() { }
NativeClass::~NativeClass() { }

Activation::Class1::Class1(): m_native(new NativeClass) { }
Activation::Class1::~Class1() { delete m_native; }

void MarshalString(String^ s, string& os) {
	using namespace Runtime::InteropServices;
	const char* chars =	(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

void MarshalString(String^ s, wstring& os) {
	using namespace Runtime::InteropServices;
	const wchar_t* chars =	(const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}

#pragma region Managed
int Activation::Class1::InstallProductKeys(String^ ProductKeys, fnCallBackFunc^ PrintString)
{	
	wstring wProductKeys;
	MarshalString(ProductKeys, wProductKeys);
	return slpublicfunc::InstallProductKey(wProductKeys, PrintString, true);
}
int Activation::Class1::InstallConfirmaionID(String^ IID, String^ CID, fnCallBackFunc^ PrintString)
{
	return slpublicfunc::InstallCID(IID, CID, PrintString, true);
}
#pragma endregion Managed



#pragma region UnManaged
int InstallKeys(std::wstring ProductKey)
{
	wstring wProductKeys;
    fnCallBackFunc^ GetResult;
	return slpublicfunc::InstallProductKey(ProductKey, GetResult, false);
}
int InstallCID(std::string IID, std::string CID)
{
	fnCallBackFunc^ GetResult;
	return slpublicfunc::InstallCID(gcnew String(IID.c_str()), gcnew String(CID.c_str()), GetResult, false);
}
#pragma endregion UnManaged