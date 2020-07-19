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
            c.InstallProductKeys("PM3NG-YFYG2-P43DG-8KWVX-M98XG", MyCallbackFunc);
            //c.KMS38ACT(MyCallbackFunc);
            Console.ReadLine();
        }
    }
}
