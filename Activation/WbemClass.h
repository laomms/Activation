#pragma once
#include "Activation.h"
ref class WbemClass
{
public:
	static int install_productkey(std::string ProductKeys, Activation::ManagedCallbackHandler^ PrintString);
	static int activate_productkey(std::string ProductKeys, char*& SkuId, Activation::ManagedCallbackHandler^ PrintString);

};

