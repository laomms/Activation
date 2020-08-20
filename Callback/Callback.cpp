
#include <iostream>
#include <stdio.h>
#include <Windows.h>


typedef int (*CallbackFun)(std::string input);
typedef void (*SetCallBackFun)(CallbackFun callbackfun);
typedef int (*InstallKeys)(std::wstring ProductKey);
typedef int (*InstallCID)(std::string IID, std::string CID);

int myfunction(std::string input)
{
    std::cout <<  input << "\n";
    return 0;
}

int main()
{
    HMODULE p = LoadLibrary(L"Activation.dll");
    SetCallBackFun setcallbackfun = (SetCallBackFun)GetProcAddress(p, "SetCallBackFun");
    InstallKeys myInstallKeys = (InstallKeys)GetProcAddress(p, "InstallKeys");
    InstallCID myInstallCID = (InstallCID)GetProcAddress(p, "InstallCID");
    setcallbackfun(myfunction);
    myInstallKeys(L"NKVRC-VXBJ3-2TYXF-F664C-W2QK7");//install ProductKey
    //myInstallCID("314497567220222866203080586423868252240020455203121856826787522", "158705200772684133642932006550607893402065354890"); //Offline activate
    system("pause");
    return 0;
}

