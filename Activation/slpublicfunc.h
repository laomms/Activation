#pragma once
#include "Activation.h"
ref class slpublicfunc
{
public:
	static int InstallProductKey(std::wstring ProductKey, Activation::ManagedCallbackHandler^ PrintString);
	static int InstallCID(std::string InstalltionID, std::string ConfirmationID, Activation::ManagedCallbackHandler^ PrintString);
};

