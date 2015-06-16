//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.5
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

using System;
using System.IO;
using System.Runtime.InteropServices;

namespace ZeroMQBackEnd {
    internal class ZeroMQBackEndPINVOKE {
        protected static SWIGExceptionHelper swigExceptionHelper = new SWIGExceptionHelper();
        protected static SWIGStringHelper swigStringHelper = new SWIGStringHelper();

        static ZeroMQBackEndPINVOKE() {}

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_A_SetName")]
        public static extern void A_SetName(HandleRef jarg1, string jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_A_SetNameSwigExplicitA")]
        public static extern void A_SetNameSwigExplicitA(HandleRef jarg1, string jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_A_SayHello")]
        public static extern string A_SayHello(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_A_SayHelloSwigExplicitA")]
        public static extern string A_SayHelloSwigExplicitA(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_delete_A")]
        public static extern void delete_A(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_new_A")]
        public static extern IntPtr new_A();

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_A_director_connect")]
        public static extern void A_director_connect(HandleRef jarg1, A.SwigDelegateA_0 delegate0,
            A.SwigDelegateA_1 delegate1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_SetA")]
        public static extern void B_SetA(HandleRef jarg1, HandleRef jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_SetASwigExplicitB")]
        public static extern void B_SetASwigExplicitB(HandleRef jarg1, HandleRef jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_SetSF")]
        public static extern void B_SetSF(HandleRef jarg1, HandleRef jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_SetSFSwigExplicitB")]
        public static extern void B_SetSFSwigExplicitB(HandleRef jarg1, HandleRef jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_CallA")]
        public static extern void B_CallA(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_CallASwigExplicitB")]
        public static extern void B_CallASwigExplicitB(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_delete_B")]
        public static extern void delete_B(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_new_B")]
        public static extern IntPtr new_B();

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_B_director_connect")]
        public static extern void B_director_connect(HandleRef jarg1, B.SwigDelegateB_0 delegate0,
            B.SwigDelegateB_1 delegate1, B.SwigDelegateB_2 delegate2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_new__VS_EmptyType")]
        public static extern IntPtr new__VS_EmptyType();

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_delete__VS_EmptyType")]
        public static extern void delete__VS_EmptyType(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_new_GenericActionB")]
        public static extern IntPtr new_GenericActionB();

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_GenericActionB_OnAction")]
        public static extern void GenericActionB_OnAction(HandleRef jarg1, HandleRef jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_GenericActionB_OnActionSwigExplicitGenericActionB")]
        public static extern void GenericActionB_OnActionSwigExplicitGenericActionB(HandleRef jarg1, HandleRef jarg2);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_delete_GenericActionB")]
        public static extern void delete_GenericActionB(HandleRef jarg1);

        [DllImport("ZeroMQBackEnd", EntryPoint = "CSharp_GenericActionB_director_connect")]
        public static extern void GenericActionB_director_connect(HandleRef jarg1,
            GenericActionB.SwigDelegateGenericActionB_0 delegate0);

        protected class SWIGExceptionHelper {
            public delegate void ExceptionArgumentDelegate(string message, string paramName);

            public delegate void ExceptionDelegate(string message);

            private static readonly ExceptionDelegate applicationDelegate = SetPendingApplicationException;
            private static readonly ExceptionDelegate arithmeticDelegate = SetPendingArithmeticException;
            private static readonly ExceptionDelegate divideByZeroDelegate = SetPendingDivideByZeroException;
            private static readonly ExceptionDelegate indexOutOfRangeDelegate = SetPendingIndexOutOfRangeException;
            private static readonly ExceptionDelegate invalidCastDelegate = SetPendingInvalidCastException;
            private static readonly ExceptionDelegate invalidOperationDelegate = SetPendingInvalidOperationException;
            private static readonly ExceptionDelegate ioDelegate = SetPendingIOException;
            private static readonly ExceptionDelegate nullReferenceDelegate = SetPendingNullReferenceException;
            private static readonly ExceptionDelegate outOfMemoryDelegate = SetPendingOutOfMemoryException;
            private static readonly ExceptionDelegate overflowDelegate = SetPendingOverflowException;
            private static readonly ExceptionDelegate systemDelegate = SetPendingSystemException;
            private static readonly ExceptionArgumentDelegate argumentDelegate = SetPendingArgumentException;
            private static readonly ExceptionArgumentDelegate argumentNullDelegate = SetPendingArgumentNullException;

            private static readonly ExceptionArgumentDelegate argumentOutOfRangeDelegate =
                SetPendingArgumentOutOfRangeException;

            static SWIGExceptionHelper() {
                SWIGRegisterExceptionCallbacks_ZeroMQBackEnd(
                    applicationDelegate,
                    arithmeticDelegate,
                    divideByZeroDelegate,
                    indexOutOfRangeDelegate,
                    invalidCastDelegate,
                    invalidOperationDelegate,
                    ioDelegate,
                    nullReferenceDelegate,
                    outOfMemoryDelegate,
                    overflowDelegate,
                    systemDelegate);

                SWIGRegisterExceptionCallbacksArgument_ZeroMQBackEnd(
                    argumentDelegate,
                    argumentNullDelegate,
                    argumentOutOfRangeDelegate);
            }

            [DllImport("ZeroMQBackEnd", EntryPoint = "SWIGRegisterExceptionCallbacks_ZeroMQBackEnd")]
            public static extern void SWIGRegisterExceptionCallbacks_ZeroMQBackEnd(
                ExceptionDelegate applicationDelegate,
                ExceptionDelegate arithmeticDelegate,
                ExceptionDelegate divideByZeroDelegate,
                ExceptionDelegate indexOutOfRangeDelegate,
                ExceptionDelegate invalidCastDelegate,
                ExceptionDelegate invalidOperationDelegate,
                ExceptionDelegate ioDelegate,
                ExceptionDelegate nullReferenceDelegate,
                ExceptionDelegate outOfMemoryDelegate,
                ExceptionDelegate overflowDelegate,
                ExceptionDelegate systemExceptionDelegate);

            [DllImport("ZeroMQBackEnd", EntryPoint = "SWIGRegisterExceptionArgumentCallbacks_ZeroMQBackEnd")]
            public static extern void SWIGRegisterExceptionCallbacksArgument_ZeroMQBackEnd(
                ExceptionArgumentDelegate argumentDelegate,
                ExceptionArgumentDelegate argumentNullDelegate,
                ExceptionArgumentDelegate argumentOutOfRangeDelegate);

            private static void SetPendingApplicationException(string message) {
                SWIGPendingException.Set(new ApplicationException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingArithmeticException(string message) {
                SWIGPendingException.Set(new ArithmeticException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingDivideByZeroException(string message) {
                SWIGPendingException.Set(new DivideByZeroException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingIndexOutOfRangeException(string message) {
                SWIGPendingException.Set(new IndexOutOfRangeException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingInvalidCastException(string message) {
                SWIGPendingException.Set(new InvalidCastException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingInvalidOperationException(string message) {
                SWIGPendingException.Set(new InvalidOperationException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingIOException(string message) {
                SWIGPendingException.Set(new IOException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingNullReferenceException(string message) {
                SWIGPendingException.Set(new NullReferenceException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingOutOfMemoryException(string message) {
                SWIGPendingException.Set(new OutOfMemoryException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingOverflowException(string message) {
                SWIGPendingException.Set(new OverflowException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingSystemException(string message) {
                SWIGPendingException.Set(new SystemException(message, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingArgumentException(string message, string paramName) {
                SWIGPendingException.Set(new ArgumentException(message, paramName, SWIGPendingException.Retrieve()));
            }

            private static void SetPendingArgumentNullException(string message, string paramName) {
                var e = SWIGPendingException.Retrieve();
                if (e != null) {
                    message = message + " Inner Exception: " + e.Message;
                }
                SWIGPendingException.Set(new ArgumentNullException(paramName, message));
            }

            private static void SetPendingArgumentOutOfRangeException(string message, string paramName) {
                var e = SWIGPendingException.Retrieve();
                if (e != null) {
                    message = message + " Inner Exception: " + e.Message;
                }
                SWIGPendingException.Set(new ArgumentOutOfRangeException(paramName, message));
            }
        }

        public class SWIGPendingException {
            [ThreadStatic]
            private static Exception pendingException;

            private static int numExceptionsPending;

            public static bool Pending
            {
                get
                {
                    var pending = false;
                    if (numExceptionsPending > 0) {
                        if (pendingException != null) {
                            pending = true;
                        }
                    }
                    return pending;
                }
            }

            public static void Set(Exception e) {
                if (pendingException != null) {
                    throw new ApplicationException(
                        "FATAL: An earlier pending exception from unmanaged code was missed and thus not thrown (" +
                        pendingException + ")", e);
                }
                pendingException = e;
                lock (typeof (ZeroMQBackEndPINVOKE)) {
                    numExceptionsPending++;
                }
            }

            public static Exception Retrieve() {
                Exception e = null;
                if (numExceptionsPending > 0) {
                    if (pendingException != null) {
                        e = pendingException;
                        pendingException = null;
                        lock (typeof (ZeroMQBackEndPINVOKE)) {
                            numExceptionsPending--;
                        }
                    }
                }
                return e;
            }
        }

        protected class SWIGStringHelper {
            public delegate string SWIGStringDelegate(string message);

            private static readonly SWIGStringDelegate stringDelegate = CreateString;

            static SWIGStringHelper() {
                SWIGRegisterStringCallback_ZeroMQBackEnd(stringDelegate);
            }

            [DllImport("ZeroMQBackEnd", EntryPoint = "SWIGRegisterStringCallback_ZeroMQBackEnd")]
            public static extern void SWIGRegisterStringCallback_ZeroMQBackEnd(SWIGStringDelegate stringDelegate);

            private static string CreateString(string cString) {
                return cString;
            }
        }
    }
}