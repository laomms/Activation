#pragma once
#include "Activation.h"
ref class slpublicfunc
{
public:
	static int InstallProductKey(std::wstring ProductKey, Activation::ManagedCallbackHandler^ PrintString);
	static int ActivateProductKey(HANDLE hSLC, GUID bSkuId, Activation::ManagedCallbackHandler^ PrintString);
};

