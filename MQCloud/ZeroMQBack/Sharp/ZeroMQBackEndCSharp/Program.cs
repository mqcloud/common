using System;
using System.Threading;
using ZeroMQBackEnd;

namespace ZeroMQBackEndCSharp {
    internal class Abra : A {
        public override void SetName(string n) {
            base.SetName("C# 666");
        }

        public override string SayHello() {
            Console.WriteLine("C# Hell is upon SWIG");
            return "C# Hell!";
        }
    }

    internal class Baba : B {
        public override void SetA(A p) {
            Console.WriteLine("C# ruls?");
            base.SetA(p);
        }
    }

    internal class Program {
        private static void Main(string[] args) {
            {
                GetValue();
            }
            GC.Collect();
            Thread.Sleep(1000);
            {
                GetValue2();
            }
            GC.Collect();
            Thread.Sleep(1000);
            {
                GetValue3();
            }
            GC.Collect();
            Thread.Sleep(1000);

            Console.WriteLine("All tests passed!=)");
            Console.ReadLine();
        }

        private static void GetValue3() {
            Console.WriteLine("SharedPtr + Inheritance scope game test");
            var b3 = new Baba();
            {
                var a3 = new Abra();
                a3.SetName("rar");
                b3.SetA(a3);
            }
            GC.Collect();
            Thread.Sleep(1000);

            b3.CallA();
        }

        private static void GetValue2() {
            Console.WriteLine("Inheritance test");
            var a2 = new Abra();
            var b2 = new Baba();
            a2.SetName("hell");
            a2.SayHello();
            b2.SetA(a2);
            b2.CallA();
        }

        private static void GetValue() {
            var a = new A();
            var b = new B();

            var aaa = new GenericActionB();
            aaa.OnAction(b);


            Console.WriteLine("A test");
            a.SetName("Bla-Bla");
            a.SayHello();

            Console.WriteLine("B test");
            b.SetA(a);
            b.CallA();
        }
    }
}