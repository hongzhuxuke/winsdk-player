// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "include/cef_v8.h"
#include "include/cef_request_context_handler.h"
#include "simple_handler_cb.h"
#include <list>
#include <mutex>
#include "simple_handler_cb.h"


//class SimpleHandleEvent {
//public:
//   virtual void OnHandleCreateWebView(int id, const char* url) = 0;
//   virtual void OnHandleWebViewDestoryed(int id) = 0;
//   virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg) = 0;
//   virtual void OnTitleChanged(const int id, std::string title_name) = 0;
//};

class SimpleHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler, 
                      public CefV8Handler,
                      public CefFocusHandler,
                      public CefRequestHandler,
                      public CefRequestContextHandler {
 public:
  explicit SimpleHandler(bool use_views, CefString startup_url);
  ~SimpleHandler();

  // Provide access to the single global instance of this object.
  static SimpleHandler* GetInstance();


  CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }

  // CefClient methods:
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     CefProcessId source_process,
     CefRefPtr<CefProcessMessage> message) OVERRIDE;

  virtual bool OnBeforePluginLoad(const CefString& mime_type,
     const CefString& plugin_url,
     bool is_main_frame,
     const CefString& top_origin_url,
     CefRefPtr<CefWebPluginInfo> plugin_info,
     PluginPolicy* plugin_policy);

  virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
     CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     CefRefPtr<CefRequest> request,
     bool is_navigation,
     bool is_download,
     const CefString& request_initiator,
     bool& disable_default_handling) {
     return nullptr;
  }
  ///
  // Called when the browser component is about to loose focus. For instance, if
  // focus was on the last HTML element and the user pressed the TAB key. |next|
  // will be true if the browser is giving focus to the next component and false
  // if the browser is giving focus to the previous component.
  ///
  /*--cef()--*/
  virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) OVERRIDE;

  ///
  // Called when the browser component is requesting focus. |source| indicates
  // where the focus request is originating from. Return false to allow the
  // focus to be set or true to cancel setting the focus.
  ///
  /*--cef()--*/
  virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) OVERRIDE;

  ///
  // Called when the browser component has received focus.
  ///
  /*--cef()--*/
  virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) OVERRIDE;


  //virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
  //   CefRefPtr<CefFrame> frame,
  //   CefProcessId source_process,
  //   CefRefPtr<CefProcessMessage> message) OVERRIDE;

  // CefDisplayHandler methods:
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) OVERRIDE;

  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     int httpStatusCode) OVERRIDE;

  // CefLifeSpanHandler methods:
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefLoadHandler methods:
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
      bool isLoading,
      bool canGoBack,
      bool canGoForward) OVERRIDE;

  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  int CloseBrowser(int id);

  bool IsClosing() const { return is_closing_; }

  void SetUrl();

  int LoadUrl(const int id, const char* url);

  int Reload(const int id);

  int ChangeWebViewSize(const int id, int width, int height);

  void RegisterCallbackObj(SimpleHandleEvent* cb);

  void RemoveCallbackObj(SimpleHandleEvent* cb);

  void AppCallJsFunction(const int web_id, const char* js_function);  


   void SetProxy(bool enable, const CefString& proxy_server,const CefString& proxy_user, const CefString& proxy_pwd);

  virtual bool Execute(const CefString& name,  /*JavaScript调用的C++方法名字*/
     CefRefPtr<CefV8Value> object,					/*JavaScript调用者对象*/
     const CefV8ValueList& arguments,				/*JavaScript传递的参数*/
     CefRefPtr<CefV8Value>& retval,				/*返回给JS的值设置给这个对*/
     CefString& exception);								/*通知异常信息给JavaScript*/



  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     CefRefPtr<CefRequest> request,
     bool user_gesture,
     bool is_redirect);

  virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     bool isProxy,
     const CefString& host,
     int port,
     const CefString& realm,
     const CefString& scheme,
     CefRefPtr<CefAuthCallback> callback);

 private:
  // Platform-specific implementation.
  void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title);

  // True if the application is using the Views framework.
  const bool use_views_;

  // List of existing browser windows. Only accessed on the CEF UI thread.
  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;

  bool is_closing_;

   std::mutex mEventCbMutex;
  std::list<SimpleHandleEvent*> mSimpleHandleEventCbList;
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleHandler);

   bool mbLoadEnd = false;

   bool mbEnableProxy = false;
   CefString mProxyServer;
   CefString mProxyUser;
   CefString mProxyPwd;
   
   CefString mStartUrl;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
