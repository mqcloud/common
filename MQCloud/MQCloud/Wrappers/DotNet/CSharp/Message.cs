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

public class Message : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Message(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Message obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~Message() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          MQCloudPINVOKE.delete_Message(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public int messaageId {
    set {
      MQCloudPINVOKE.Message_messaageId_set(swigCPtr, value);
    } 
    get {
      int ret = MQCloudPINVOKE.Message_messaageId_get(swigCPtr);
      return ret;
    } 
  }

  public CString topic {
    set {
      MQCloudPINVOKE.Message_topic_set(swigCPtr, CString.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.Message_topic_get(swigCPtr);
      CString ret = (cPtr == IntPtr.Zero) ? null : new CString(cPtr, false);
      return ret;
    } 
  }

  public CString data {
    set {
      MQCloudPINVOKE.Message_data_set(swigCPtr, CString.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.Message_data_get(swigCPtr);
      CString ret = (cPtr == IntPtr.Zero) ? null : new CString(cPtr, false);
      return ret;
    } 
  }

  public CString serviceId {
    set {
      MQCloudPINVOKE.Message_serviceId_set(swigCPtr, CString.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.Message_serviceId_get(swigCPtr);
      CString ret = (cPtr == IntPtr.Zero) ? null : new CString(cPtr, false);
      return ret;
    } 
  }

  public Message() : this(MQCloudPINVOKE.new_Message(), true) {
  }

}

}
