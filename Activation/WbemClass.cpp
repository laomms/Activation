#include "pch.h"
#include <string>
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

# pragma comment(lib, "wbemuuid.lib")

#include "WbemClass.h"
#include "Activation.h"

using namespace std;
#pragma warning(disable:4996)
using namespace System;
using namespace System::Management;


int WbemClass::install_productkey(std::string ProductKeys,Activation::ManagedCallbackHandler^ PrintString)
{
	IWbemServices* pServices = NULL;
	IWbemLocator* pLoc = 0;
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr))
		goto cleanup;

	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hr) && RPC_E_TOO_LATE != hr)
		goto cleanup;

	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hr) || (NULL == pLoc))
		goto cleanup;

	hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pServices);
	if (FAILED(hr) || (NULL == pServices))
		goto cleanup;

	hr = CoSetProxyBlanket(pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hr))
		goto cleanup;

	IEnumWbemClassObject* pEnum = NULL;
	BSTR Query = SysAllocString(L"Select * from SoftwareLicensingService");
	hr = pServices->ExecQuery(_bstr_t(L"WQL"), Query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnum);
	IWbemClassObject* spInstance;
	ULONG uNumOfInstances = 0;
	hr = pEnum->Next(WBEM_INFINITE, 1, &spInstance, &uNumOfInstances);
	VARIANT vtPath;
	hr = spInstance->Get(L"__Path", 0, &vtPath, 0, 0);
	if (FAILED(hr))
		goto cleanup;

	IWbemClassObject* pLicensingClsObj = NULL;
	hr = pServices->GetObject(_bstr_t(L"SoftwareLicensingService"), 0, NULL, &pLicensingClsObj, NULL);
	if (FAILED(hr) || (NULL == pLicensingClsObj))
		goto cleanup;

	IWbemClassObject* pInputParamsClass = NULL;
	hr = pLicensingClsObj->GetMethod(L"InstallProductKey", 0, &pInputParamsClass, NULL);
	if (FAILED(hr) || (NULL == pInputParamsClass))
		goto cleanup;

	IWbemClassObject* pInputParams = NULL;
	hr = pInputParamsClass->SpawnInstance(0, &pInputParams);
	if (FAILED(hr) || (NULL == pInputParams))
		goto cleanup;
	{
		VARIANT vtProductKey;
		wstring wProductKey = wstring(ProductKeys.begin(), ProductKeys.end());
		vtProductKey.vt = VT_BSTR;
		vtProductKey.bstrVal = SysAllocString(_bstr_t(ProductKeys.c_str()));
		hr = pInputParams->Put(L"ProductKey", 0, &vtProductKey, 0);
		if (FAILED(hr))
			goto cleanup;
	}

	IWbemClassObject* pOutputParams = NULL;
	hr = pServices->ExecMethod(vtPath.bstrVal, _bstr_t(L"InstallProductKey"), 0, NULL, pInputParams, &pOutputParams, NULL);
	if (FAILED(hr) || (NULL == pOutputParams))
		goto cleanup;

	hr = S_OK;//all success

cleanup:
	if (NULL != pLoc)
	{
		pLoc->Release();
		pLoc = NULL;
	}
	if (NULL != pServices)
	{
		pServices->Release();
		pServices = NULL;
	}
	(VOID)CoUninitialize();
	return hr;
}

int WbemClass::activate_productkey(std::string ProductKeys, char*& SkuId, Activation::ManagedCallbackHandler^ PrintString)
{
	//char* SkuId = (char*)calloc(100, sizeof(char*));
	//初始化
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED); // 初始化 COM。
	if (FAILED(hr))
	{
		//cout << "初始化 COM 库失败。错误代码 = 0x" << hex << hr << endl;
		return hr;
	}
	//设置安全级别	
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_CONNECT, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, 0);
	if (FAILED(hr) && RPC_E_TOO_LATE != hr)
	{
		//cout << "初始化安全性失败。错误代码 = 0x"<< hex << hr << endl;
		CoUninitialize();
		return hr;
	}

	//初始化 IWbemLocator 接口
	IWbemLocator* pLoc = 0;
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hr))
	{
		//cout << "创建 IWbemLocator 对象失败。错误代码 =  0x" << hex << hr << endl;
		CoUninitialize();
		return hr;
	}

	//连接到 WMI
	IWbemServices* pServices = NULL;
	// 以当前用户身份连接到 root\cimv2 名称空间。	
	hr = pLoc->ConnectServer((const BSTR)L"ROOT\\CIMV2", NULL, NULL, 0, NULL, 0, 0, &pServices);
	if (FAILED(hr))
	{
		//cout << "无法连接。错误代码 = 0x" << hex << hr << endl;		
		CoUninitialize();
		return hr;
	}
	// 设置代理，以模拟客户。	
	hr = CoSetProxyBlanket(pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hr))
	{
		//cout << "无法设置代理层。错误代码 = 0x"<< hex << hr << endl;
		CoUninitialize();
		return hr;
	}


	IEnumWbemClassObject* enumerator;
    std:string sql = "SELECT Name,ID,PartialProductKey FROM SoftwareLicensingProduct WHERE PartialProductKey like '" + ProductKeys.substr(24, 5) + "' ";
	wstring wsql = wstring(sql.begin(), sql.end());
	hr = pServices->ExecQuery((const BSTR)L"WQL", (const BSTR)wsql.c_str(), WBEM_FLAG_FORWARD_ONLY, NULL, &enumerator);
	if (SUCCEEDED(hr))
	{
		IWbemClassObject* spInstance = NULL;
		ULONG uNumOfInstances;
		while (1) {
			hr = enumerator->Next(WBEM_INFINITE, 1L, &spInstance, &uNumOfInstances);
			if (SUCCEEDED(hr))
			{
				if (uNumOfInstances > 0)
				{
					_variant_t var_val;
					hr = spInstance->Get(L"PartialProductKey", 0, &var_val, NULL, NULL);
					if (SUCCEEDED(hr))
					{
						if (var_val.vt != VT_NULL) {
							_bstr_t str = (_bstr_t)var_val;
							hr = spInstance->Get(L"ID", 0, &var_val, NULL, NULL);
							_bstr_t PID = (_bstr_t)var_val;
							strcpy(SkuId, PID);
							std::string path = "SoftwareLicensingProduct.ID='" + PID + "'";
							std::wstring wpath = wstring(path.begin(), path.end());

							IWbemClassObject* pObj = NULL;
							hr = pServices->GetObject(_bstr_t(wpath.c_str()), 0, NULL, &pObj, NULL);
							VARIANT vtPath;
							hr = pObj->Get(L"__Path", 0, &vtPath, 0, 0);

							IWbemClassObject* pOutParams = NULL;
							hr = pServices->ExecMethod(vtPath.bstrVal, L"Activate", 0, NULL, NULL, &pOutParams, NULL);
							if (pServices) pServices->Release();
							if (enumerator) enumerator->Release();
							CoUninitialize();
							return hr;
						}
						else
						{
							continue;
						}
					}
					else {

						//std::cerr << "IWbemClassObject::Get failed" << std::endl;
					}

				}
				else break;
			}
		}//end while
	}
	else {
		printf("Query failed: 0x%x\n", hr);
	}

	{
		//getchar();
		if (pServices) pServices->Release();
		if (enumerator) enumerator->Release();
	}

	CoUninitialize();
	return hr;
}



