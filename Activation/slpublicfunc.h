#pragma once
#include "Activation.h"
ref class slpublicfunc
{
public:
	static int InstallProductKey(std::wstring ProductKey, Activation::fnCallBackFunc^ PrintString,bool managedflag);
	static int InstallCID(String^ InstalltionID, String^ ConfirmationID, Activation::fnCallBackFunc^ PrintString, bool managedflag);
};

