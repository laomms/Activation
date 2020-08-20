# Activation control

CLR激活控件双语版,顺带一个纯C++版调用IWbem服务安装激活的类.   
由于没有用OSppc控件,所以Office的激活过程使用WMI对象实现.证书很多时理论上比调用API要慢.   


# 调用实例
C++调用
 ```C
#include <iostream>
#include <stdio.h>
#include <Windows.h>


typedef int (*CallbackFun)(std::string input);
typedef void (*SetCallBackFun)(CallbackFun callbackfun);
typedef int (*InstallKeys)(std::wstring ProductKey);
typedef int (*InstallCID)(std::string IID, std::string CID);

int myfunction(std::string input) //回调的激活过程
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

 ```
 c#调用
  ```C
  
    class Program
    {
        static void MyCallbackFunc(string value)
        {
            Console.WriteLine(value.ToString());
        }
        static void Main(string[] args)
        {
            Activation.Class1 c = new Activation.Class1();
            c.InstallProductKeys("NKVRC-VXBJ3-2TYXF-F664C-W2QK7", MyCallbackFunc);  //install ProductKey
            //c.InstallConfirmaionID("314497567220222866203080586423868252240020455203121856826787522", "158705200772684133642932006550607893402065354890", MyCallbackFunc);  //Offline activate
            Console.ReadLine();
        }
    }

   ```
