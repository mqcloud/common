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

public class EventsHandler : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal EventsHandler(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(EventsHandler obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~EventsHandler() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          MQCloudPINVOKE.delete_EventsHandler(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public SWIGTYPE_p_f_int_int__void OnSendError {
    set {
      MQCloudPINVOKE.EventsHandler_OnSendError_set(swigCPtr, SWIGTYPE_p_f_int_int__void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.EventsHandler_OnSendError_get(swigCPtr);
      SWIGTYPE_p_f_int_int__void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_int_int__void(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_f_int__void OnReadError {
    set {
      MQCloudPINVOKE.EventsHandler_OnReadError_set(swigCPtr, SWIGTYPE_p_f_int__void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.EventsHandler_OnReadError_get(swigCPtr);
      SWIGTYPE_p_f_int__void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_int__void(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_f_int__void OnExchangeError {
    set {
      MQCloudPINVOKE.EventsHandler_OnExchangeError_set(swigCPtr, SWIGTYPE_p_f_int__void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.EventsHandler_OnExchangeError_get(swigCPtr);
      SWIGTYPE_p_f_int__void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_int__void(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_f___void OnConnectedToExchange {
    set {
      MQCloudPINVOKE.EventsHandler_OnConnectedToExchange_set(swigCPtr, SWIGTYPE_p_f___void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.EventsHandler_OnConnectedToExchange_get(swigCPtr);
      SWIGTYPE_p_f___void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f___void(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_f_p_ServiceId__void OnServiceRegistred {
    set {
      MQCloudPINVOKE.EventsHandler_OnServiceRegistred_set(swigCPtr, SWIGTYPE_p_f_p_ServiceId__void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.EventsHandler_OnServiceRegistred_get(swigCPtr);
      SWIGTYPE_p_f_p_ServiceId__void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_p_ServiceId__void(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_f_p_CString_p_ServiceId__void OnNewSubscriber {
    set {
      MQCloudPINVOKE.EventsHandler_OnNewSubscriber_set(swigCPtr, SWIGTYPE_p_f_p_CString_p_ServiceId__void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = MQCloudPINVOKE.EventsHandler_OnNewSubscriber_get(swigCPtr);
      SWIGTYPE_p_f_p_CString_p_ServiceId__void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_p_CString_p_ServiceId__void(cPtr, false);
      return ret;
    } 
  }

  public EventsHandler() : this(MQCloudPINVOKE.new_EventsHandler(), true) {
  }

}

}