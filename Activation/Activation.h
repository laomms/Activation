#pragma once
#include <string>
#include <slpublic.h>
#include <unordered_map>
#using <System.dll>
//#include "slpublicfunc.h"
//#include "WbemClass.h"
using namespace System;

class NativeClass
{
public:
	NativeClass();
	~NativeClass();
};

namespace Activation {
		public delegate void ManagedCallbackHandler(String^);
		public ref class Class1
		{
		public:
			Class1();
			~Class1();

			int InstallProductKeys(String^ ProductKey, ManagedCallbackHandler^ PrintString);

		private:
			NativeClass* m_native;
		};
}
