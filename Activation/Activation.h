#pragma once
#include <string>
#include <slpublic.h>
#include <unordered_map>
#using <System.dll>

using namespace System;


typedef void (*CallbackFun)(std::string input);
extern "C" _declspec(dllexport) void SetCallBackFun(CallbackFun callbackfun);
extern "C" _declspec(dllexport) int InstallKeys(std::wstring ProductKey);
extern "C" _declspec(dllexport) int InstallCID(std::string IID, std::string CID);

class NativeClass
{
public:
	NativeClass();
	~NativeClass();
};

namespace Activation {
		public delegate void fnCallBackFunc(String^);
		public ref class Class1
		{
		public:
			Class1();
			~Class1();

			int InstallProductKeys(String^ ProductKey, fnCallBackFunc^ PrintString);
			int InstallConfirmaionID(String^ IID, String^ CID, fnCallBackFunc^ PrintString);
		private:
			NativeClass* m_native;
		};
}
