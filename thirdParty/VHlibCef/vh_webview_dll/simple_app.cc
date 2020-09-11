// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"
#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"


namespace {

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
class SimpleWindowDelegate : public CefWindowDelegate {
 public:
  explicit SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

  void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
    // Add the browser view and show the window.
    window->AddChildView(browser_view_);
    window->Show();

    // Give keyboard focus to the browser view.
    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
    browser_view_ = NULL;
  }

  bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
    // Allow the window to close if the browser says it's OK.
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser)
      return browser->GetHost()->TryCloseBrowser();
    return true;
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;

  IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
  DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
};

}  // namespace

SimpleApp::SimpleApp() {}

SimpleApp::~SimpleApp() {

}

void SimpleApp::Close() {
    mHandler->CloseAllBrowsers(false);
}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
  const bool use_views = false;

  // SimpleHandler implements browser-level callbacks.
  mHandler = new SimpleHandler(use_views);

  //// Specify CEF browser settings here.
  //CefBrowserSettings browser_settings;

  //std::string url;

  //// Check if a "--url=" value was provided via the command-line. If so, use
  //// that instead of the default URL.
  //url = command_line->GetSwitchValue("url");
  //if (url.empty())
  //  url = "http://e.vhall.com";

  //if (use_views) {
  //  // Create the BrowserView.
  //  CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
  //      mHandler, url, browser_settings, NULL, NULL);

  //  // Create the Window. It will show itself after creation.
  //  CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(browser_view));
  //} else {
  //  // Information used when creating the native window.
  //  CefWindowInfo window_info;
  //  window_info.SetAsPopup(NULL, "cefsimple");


  //  // Create the first browser window.
  //  CefBrowserHost::CreateBrowser(window_info, mHandler, url, browser_settings,
  //                                NULL);
  //}
}

void SimpleApp::OnBeforeCommandLineProcessing(
   const CefString& process_type,
   CefRefPtr<CefCommandLine> command_line) {
   command_line->AppendSwitch("--ppapi-out-of-process");
   command_line->AppendSwitch("--disable-web-security");//关闭同源策略

#ifdef X64
   CefString ppapi = "pepflash\\x64\\pepflashplayer.dll";
#else
   CefString ppapi = "pepflash\\x86\\pepflashplayer.dll";
#endif // X64

   command_line->AppendSwitchWithValue("ppapi-flash-path", ppapi);     //加载flash插件
   command_line->AppendSwitchWithValue("ppapi-flash-version","99.0.0.126");    //manifest.json中的version


   command_line->AppendSwitch("--disable-gpu");
   command_line->AppendSwitch("--disable-gpu-compositing");
   command_line->AppendSwitchWithValue("profile.default_content_setting_values.plugins", "1");     //加载flash插件
   command_line->AppendSwitchWithValue("plugin-policy", "allow");
 }

//
//void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
//   CefRefPtr<CefFrame> frame,
//   CefRefPtr<CefV8Context> context) {
//
//   // The var type can accept all object or variable  
//   // Retrieve the context's window object.
//   CefRefPtr<CefV8Value> object = context->GetGlobal();
//   // Create the "NativeLogin" function.
//   CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("NativeLogin", mHandler);
//   // Add the "NativeLogin" function to the "window" object.
//   object->SetValue("NativeLogin", func, V8_PROPERTY_ATTRIBUTE_NONE);
//}

void SimpleApp::OnHandleCreateWebView(int &id, const char* url) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
   if (mBrowserEventListenPtr) {
      mBrowserEventListenPtr->OnHandleCreateWebView(id, url);
   }
}
/*webview销毁事件通知*/
void SimpleApp::OnHandleWebViewDestoryed(int &id) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
   if(mBrowserEventListenPtr){
      mBrowserEventListenPtr->OnHandleWebViewDestoryed(id);
   }
}

void SimpleApp::OnTitleChanged(const int id, std::string title_name) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
   if (mBrowserEventListenPtr) {
      mBrowserEventListenPtr->OnTitleChanged(id, title_name);
   }
}

void SimpleApp::OnRecvMsg(const int id, std::string fun_name, std::string msg) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
   if (mBrowserEventListenPtr) {
      mBrowserEventListenPtr->OnRecvMsg(id, fun_name, msg);
   }
}

void SimpleApp::RegisterBrowserEventListener(BrowserEventListen* obj) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
    mBrowserEventListenPtr = obj;
}

