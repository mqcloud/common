/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace MQCloud {

using System;
using System.Runtime.InteropServices;

public class CString : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal CString(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(CString obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~CString() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          MQCloudPINVOKE.delete_CString(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public string data {
    set {
      MQCloudPINVOKE.CString_data_set(swigCPtr, value);
    } 
    get {
      string ret = MQCloudPINVOKE.CString_data_get(swigCPtr);
      return ret;
    } 
  }

  public int length {
    set {
      MQCloudPINVOKE.CString_length_set(swigCPtr, value);
    } 
    get {
      int ret = MQCloudPINVOKE.CString_length_get(swigCPtr);
      return ret;
    } 
  }

  public CString() : this(MQCloudPINVOKE.new_CString(), true) {
  }

}

}