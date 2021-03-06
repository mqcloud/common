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

public class CoreMessage : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal CoreMessage(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(CoreMessage obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~CoreMessage() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          MQCloudPINVOKE.delete_CoreMessage(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public Message Message {
    set {
      MQCloudPINVOKE.CoreMessage_Message_set(swigCPtr, Message.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.CoreMessage_Message_get(swigCPtr);
      Message ret = (cPtr == IntPtr.Zero) ? null : new Message(cPtr, false);
      return ret;
    } 
  }

  public CString PatternName {
    set {
      MQCloudPINVOKE.CoreMessage_PatternName_set(swigCPtr, CString.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.CoreMessage_PatternName_get(swigCPtr);
      CString ret = (cPtr == IntPtr.Zero) ? null : new CString(cPtr, false);
      return ret;
    } 
  }

  public CString PatternData {
    set {
      MQCloudPINVOKE.CoreMessage_PatternData_set(swigCPtr, CString.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.CoreMessage_PatternData_get(swigCPtr);
      CString ret = (cPtr == IntPtr.Zero) ? null : new CString(cPtr, false);
      return ret;
    } 
  }

  public CoreMessage() : this(MQCloudPINVOKE.new_CoreMessage(), true) {
  }

}

}
