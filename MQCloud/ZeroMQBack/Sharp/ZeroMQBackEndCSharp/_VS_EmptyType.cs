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
using System.Runtime.InteropServices;

namespace ZeroMQBackEnd {
    public class _VS_EmptyType : IDisposable {
        protected bool swigCMemOwn;
        private HandleRef swigCPtr;

        internal _VS_EmptyType(IntPtr cPtr, bool cMemoryOwn) {
            swigCMemOwn = cMemoryOwn;
            swigCPtr = new HandleRef(this, cPtr);
        }

        public _VS_EmptyType() : this(ZeroMQBackEndPINVOKE.new__VS_EmptyType(), true) {}

        public virtual void Dispose() {
            lock (this) {
                if (swigCPtr.Handle != IntPtr.Zero) {
                    if (swigCMemOwn) {
                        swigCMemOwn = false;
                        ZeroMQBackEndPINVOKE.delete__VS_EmptyType(swigCPtr);
                    }
                    swigCPtr = new HandleRef(null, IntPtr.Zero);
                }
                GC.SuppressFinalize(this);
            }
        }

        internal static HandleRef getCPtr(_VS_EmptyType obj) {
            return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
        }

        ~_VS_EmptyType() {
            Dispose();
        }
    }
}