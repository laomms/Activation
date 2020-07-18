using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test
{
    class Program
    {
        static void MyCallbackFunc(string value)
        {
            Console.WriteLine(value.ToString());
        }
        static void Main(string[] args)
        {
            Activation.Class1 c = new Activation.Class1();
            c.InstallProductKeys("2K932-JWGBH-M83T8-KP2BX-74PKT", MyCallbackFunc);
            //c.KMS38ACT(MyCallbackFunc);
            Console.ReadLine();
        }
    }
}
