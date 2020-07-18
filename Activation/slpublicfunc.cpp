#include "pch.h"
#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <tchar.h>
#include <Windows.h>
#include <slerror.h>
#include <slpublic.h>
#include <rpcdce.h>
#include "Activation.h"
#include "slpublicfunc.h"
#pragma comment(lib,"Slwga.lib")
#pragma comment(lib,"Slc.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"Slcext.lib")
#pragma warning(disable:4996)

using namespace System;
using namespace System::Management;
using namespace System::Runtime::InteropServices;

int GetOSLCID()
{
	LCID lcid = GetSystemDefaultLCID();
	if (lcid == 0x409)
	{
		return 0;
	}
	if (lcid == 0x404)
	{
		return 2;
	}

	if (lcid == 0x804)
	{
		return 1;
	}
	return 0;
}

BOOL GetNtVersionNumbers(DWORD& dwMajorVer, DWORD& dwMinorVer, DWORD& dwBuildNumber)
{
	BOOL bRet = FALSE;
	HMODULE hModNtdll = NULL;
	if (hModNtdll = ::LoadLibraryW(L"ntdll.dll"))
	{
		typedef void (WINAPI* pfRTLGETNTVERSIONNUMBERS)(DWORD*, DWORD*, DWORD*);
		pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
		pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
		if (pfRtlGetNtVersionNumbers)
		{
			pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
			dwBuildNumber &= 0x0ffff;
			bRet = TRUE;
		}
		::FreeLibrary(hModNtdll);
		hModNtdll = NULL;
	}
	return bRet;
}

int InstallKey(std::wstring ProductKeys, Activation::ManagedCallbackHandler^ PrintString)
{
	char nErrorCode[32];
	String^ ProductKey = gcnew String(ProductKeys.c_str());
	//ManagementClass^ mClass = gcnew ManagementClass("SELECT Version FROM OfficeSoftwareProtectionService");
	//ManagementObjectCollection^ moCollection;
	//ManagementObject^ mObject = gcnew ManagementObject();
	//moCollection = mClass->GetInstances();
	//for each (mObject in moCollection)
	//{
	//	array<System::String^>^ agrs = { ProductKey };
	//	mObject->InvokeMethod("InstallProductKey", agrs);
	//	//str += mObject["SerialNumber"]->ToString();
	//}
	SelectQuery^ NAQuery = gcnew SelectQuery("SELECT Version FROM SoftwareLicensingService");
	ManagementObjectSearcher^ NASearcher = gcnew ManagementObjectSearcher(NAQuery);
	try
	{
		for each (ManagementObject ^ mObject in NASearcher->Get())
		{
			array<System::String^>^ agrs = { ProductKey };
			mObject->InvokeMethod("InstallProductKey", agrs);
		}
	}
	catch (COMException^ err)
	{
		sprintf_s(nErrorCode, "0x%08X", err->ErrorCode);
		PrintString(GetOSLCID() == 1 ? "安装失败,错误代码:"+ gcnew String(nErrorCode) : "Installation failed, error code:" + gcnew String(nErrorCode));
		return err->ErrorCode;
	}	
	return 0;

	/*ManagementClass mc = new  ManagementClass("Win32_Process");
	ManagementBaseObject obj = mc.GetMethodParameters("Create");
	obj["CommandLine"] = "notepad.exe";
	mc.InvokeMethod("Create", obj, NULL);
	mc.Dispose(); */
}

int ActivateKey(std::string ProductKeys, char*& SkuId, Activation::ManagedCallbackHandler^ PrintString)
{
	return 0;
}

int slpublicfunc::ActivateProductKey(HANDLE hSLC, GUID bSkuId, Activation::ManagedCallbackHandler^ PrintString)
{
	char nErrorCode[32];
	int Status = SLActivateProduct(hSLC, &bSkuId, NULL, NULL, NULL, NULL, NULL);
	if (Status == ERROR_SUCCESS)
	{
		PrintString(GetOSLCID() == 1 ? "激活成功." : "Activation was successful.");
		return 0;
	}
	else
	{
		sprintf_s(nErrorCode, "0x%08X", Status);
		PrintString(GetOSLCID() == 1 ? "激活失败,错误代码:" + gcnew String(nErrorCode) : "Activation failed, error code:" + gcnew String(nErrorCode));
		return Status;
	}
}

int slpublicfunc::InstallProductKey(std::wstring ProductKey,Activation::ManagedCallbackHandler^ PrintString)
{
	char nErrorCode[32];
	GUID PKeyId = GUID_NULL;
	GUID bSkuId = GUID_NULL;
	HANDLE hSLC = NULL;
	std::wstring pwszPKeyAlgorithm;

	PrintString(GetOSLCID() == 1 ? "安装密钥..." : "Install ProductKey...");

	DWORD dwMajorVer, dwMinorVer, dwBuildNumber;
	GetNtVersionNumbers(dwMajorVer, dwMinorVer, dwBuildNumber);
	double verno = stod(std::to_string(dwMajorVer) + "." + std::to_string(dwMinorVer));
	if (verno > 6.1)
	{
		pwszPKeyAlgorithm = L"msft:rm/algorithm/pkey/2009";
	}
	else
	{
		pwszPKeyAlgorithm = L"msft:rm/algorithm/pkey/2005";
	}

	if (SLOpen(&hSLC) == ERROR_SUCCESS) {
		int Status = SLInstallProofOfPurchase(hSLC,(PCWSTR)pwszPKeyAlgorithm.c_str(), ProductKey.c_str(), 0, 0, &PKeyId);//{C5C310D8-B0B2-27E7-7372-E7BE86648DBD}
		if (Status == ERROR_SUCCESS)
		{
			SLDATATYPE peDataType;
			UINT pcbValue = 0;
			PBYTE ppbValue;
			Status = SLGetPKeyInformation(hSLC, &PKeyId, L"ProductSkuId", &peDataType, &pcbValue, &ppbValue);
			if (Status == ERROR_SUCCESS)
			{
				bSkuId = GUID_NULL; //{84832881-46EF-4124-8ABC-EB493CDCF78E}
				memcpy(&bSkuId, ppbValue, 16);
				PrintString(GetOSLCID() == 1 ? "安装成功." : "Successful installation.");
				return ActivateProductKey(hSLC, bSkuId, PrintString);
			}
			else
			{
				sprintf_s(nErrorCode, "0x%08X", Status);
				PrintString(GetOSLCID() == 1 ? "安装失败,错误代码:" + gcnew String(nErrorCode) : "Installation failed, error code:" + gcnew String(nErrorCode));
				if ((std::string)nErrorCode == "C004D302")
					PrintString(GetOSLCID() == 1 ? "请重启系统." : "Please restart the system.");
				return Status;
			}
		}
		else if (Status == -1073422312)//C004E018
		{
			InstallKey(ProductKey, PrintString);
		}
		else if (Status == -1073418160)//0xc004f050
		{
			InstallKey(ProductKey, PrintString);
		}
		else
		{
			sprintf_s(nErrorCode, "0x%08X", Status);
			PrintString(GetOSLCID() == 1 ? "安装失败,错误代码:" + gcnew String(nErrorCode) : "Installation failed, error code:" + gcnew String(nErrorCode));
			if ((std::string)nErrorCode == "C004D302")
				PrintString(GetOSLCID() == 1 ? "请重启系统." : "Please restart the system.");
			return Status;
		}
	}
}


