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
            //c.InstallProductKeys("NKVRC-VXBJ3-2TYXF-F664C-W2QK7", MyCallbackFunc);
            c.InstallConfirmaionID("314497567220222866203080586423868252240020455203121856826787522", "158705200772684133642932006550607893402065354890", MyCallbackFunc);
            Console.ReadLine();
        }
    }
}
