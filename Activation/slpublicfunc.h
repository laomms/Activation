#pragma once
#include "Activation.h"
ref class slpublicfunc
{
public:
	static int InstallProductKey(std::wstring ProductKey, Activation::ManagedCallbackHandler^ PrintString);
	static int InstallCID(String^ InstalltionID, String^ ConfirmationID, Activation::ManagedCallbackHandler^ PrintString);
};

