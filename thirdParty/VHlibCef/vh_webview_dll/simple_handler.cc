// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {

SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler(bool use_views,  CefString startup_url)
    : use_views_(use_views), is_closing_(false) , mStartUrl(startup_url){
  //DCHECK(!g_instance);
  //g_instance = this;
}

SimpleHandler::~SimpleHandler() {
  //g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

bool SimpleHandler::OnBeforePluginLoad(const CefString& mime_type,
   const CefString& plugin_url,
   bool is_main_frame,
   const CefString& top_origin_url,
   CefRefPtr<CefWebPluginInfo> plugin_info,
   PluginPolicy* plugin_policy) {

   *plugin_policy = PLUGIN_POLICY_ALLOW;
   return true;
}

bool SimpleHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
   CefRefPtr<CefFrame> frame,
   CefProcessId source_process,
   CefRefPtr<CefProcessMessage> message) {
   int id = browser->GetIdentifier();
   const std::string& messageName = message->GetName();
   if (messageName == "JsCallQtMsg" || messageName == "JsCallQtOpenUrl" || messageName == "JsCallQtStartLive" ||
      messageName == "JSCallQtUserOnline" || messageName == "JSCallQtUserOffline" || messageName == "JsCallQtShareTo" ||
      messageName == "JsCallQtStartVoiceLive" || messageName == "JsCallFadeOutTip"){
      CefRefPtr<CefListValue> value = message->GetArgumentList();
      if (value->GetSize() > 0) {
         CefString argv = value->GetString(0);
         std::unique_lock<std::mutex> lock(mEventCbMutex);
         std::list<SimpleHandleEvent*>::iterator iter = mSimpleHandleEventCbList.begin();
         while (iter != mSimpleHandleEventCbList.end()) {
            (*iter)->OnRecvMsg(id, messageName, argv.ToString());
            iter++;
         }
      }
   }
   else if (messageName == "JsCallQtJoinActivity" || "ClientCallCloseExamCallback" == messageName
      || "sendQtCloseSuccess" == messageName || "sendQtStopAnswer" == messageName) {
      std::unique_lock<std::mutex> lock(mEventCbMutex);
      std::list<SimpleHandleEvent*>::iterator iter = mSimpleHandleEventCbList.begin();
      while (iter != mSimpleHandleEventCbList.end()) {
         (*iter)->OnRecvMsg(id, messageName , "");
         iter++;
      }
   }
   return true;
}

void SimpleHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) {
   int a = 0;
   a = 0;
}

/*--cef()--*/
bool SimpleHandler::OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source){
   int a = 0;
   a = 0;

   return false;
}

/*--cef()--*/
void SimpleHandler::OnGotFocus(CefRefPtr<CefBrowser> browser){
   int a = 0;
   a = 0;
}

bool SimpleHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
   CefRefPtr<CefFrame> frame,
   CefRefPtr<CefRequest> request,
   bool user_gesture,
   bool is_redirect) {

   if (mbEnableProxy) {
      CefRefPtr<CefRequestContext> context = browser->GetHost()->GetRequestContext();

      CefString error;
      CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();
      dict->SetString("mode", "fixed_servers");
      dict->SetString("server", mProxyServer);
      CefRefPtr<CefValue> value = CefValue::Create();
      value->SetDictionary(dict);
      context->SetPreference("proxy", value, error);
   }
   return false;
}

bool SimpleHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
   CefRefPtr<CefFrame> frame,
   bool isProxy,
   const CefString& host,
   int port,
   const CefString& realm,
   const CefString& scheme,
   CefRefPtr<CefAuthCallback> callback) {
   if (isProxy) {
      callback->Continue(mProxyUser, mProxyPwd);
      return true;
   }    
   return false;
}

void SimpleHandler::SetProxy(bool enable, const CefString& proxy_server, const CefString& proxy_user, const CefString& proxy_pwd) {
   mbEnableProxy = enable;
   mProxyServer = proxy_server;
   mProxyUser = proxy_user;
   mProxyPwd = proxy_pwd;
}


void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
  int id = browser->GetIdentifier();
  std::unique_lock<std::mutex> lock(mEventCbMutex);
  std::list<SimpleHandleEvent*>::iterator iter = mSimpleHandleEventCbList.begin();
  while (iter != mSimpleHandleEventCbList.end()) {
     (*iter)->OnTitleChanged(id, title.ToString().c_str());
     iter++;
  }

}

void SimpleHandler::SetUrl() {
    std::list<CefRefPtr<CefBrowser>>::iterator iter = browser_list_.begin();
    while (iter != browser_list_.end()) {
        iter->get()->GetMainFrame()->LoadURL(CefString("https://live.vhall.com/room/watch/547870006"));
        iter++;
    }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
  int id = browser->GetIdentifier();
  CefString url = browser->GetMainFrame()->GetURL();
}

void SimpleHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
   CefRefPtr<CefFrame> frame,
   int httpStatusCode) {
   int id = browser->GetIdentifier();
   CefString url = browser->GetMainFrame()->GetURL();
   //HWND cefHandle = browser->GetHost()->GetWindowHandle();
   //HDC hdc = GetDC(cefHandle);
   //int ppix = GetDeviceCaps(hdc, LOGPIXELSX);
   //int ppiy = GetDeviceCaps(hdc, LOGPIXELSY);
   //ReleaseDC(cefHandle, hdc);
   //double newZoomLevel = 0;
   //if (ppix > 96) {
   //   newZoomLevel = (ppix - 96) / 24;
   //}
   //browser->GetHost()->SetZoomLevel(1.5);
   std::unique_lock<std::mutex> lock(mEventCbMutex);
   std::list<SimpleHandleEvent*>::iterator iter = mSimpleHandleEventCbList.begin();
   while (iter != mSimpleHandleEventCbList.end()) {
      (*iter)->OnHandleCreateWebView(id, url.ToString().c_str());
      iter++;
   }

}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void SimpleHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
    bool isLoading,
    bool canGoBack,
    bool canGoForward) {

}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";
  //frame->LoadString(ss.str(), failedUrl);
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it) {
      (*it)->GetHost()->CloseBrowser(force_close);
  }
}

int SimpleHandler::LoadUrl(const int id, const char* url) {
    BrowserList::const_iterator it = browser_list_.begin();
    for (; it != browser_list_.end(); ++it) {
        if ((*it)->GetIdentifier() == id) {
            (*it)->GetMainFrame()->LoadURL(url);
            return 0;
        }
    }
    return -1;
}

int SimpleHandler::Reload(const int id) {
    BrowserList::const_iterator it = browser_list_.begin();
    for (; it != browser_list_.end(); ++it) {
        if ((*it)->GetIdentifier() == id) {
            (*it)->Reload();
            return 0;
        }
    }
    return -1;
}

int SimpleHandler::CloseBrowser(int id) {
    bool force_close = false;
    if (!CefCurrentlyOn(TID_UI)) {
        // Execute on the UI thread.
        CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
            id));
        return 0;
    }

    if (browser_list_.empty())
        return 0;

    BrowserList::const_iterator it = browser_list_.begin();
    for (; it != browser_list_.end(); ++it) {
        if (id == (*it)->GetIdentifier()) {
            (*it)->GetHost()->CloseBrowser(false);
        }
    }
}

int SimpleHandler::ChangeWebViewSize(const int id, int width, int height) {
    BrowserList::const_iterator it = browser_list_.begin();
    for (; it != browser_list_.end(); ++it) {
        if ((*it)->GetIdentifier() == id) {
            auto windowHandle = (*it)->GetHost()->GetWindowHandle();
            if (windowHandle) {
                auto hdwp = BeginDeferWindowPos(1);
                hdwp = DeferWindowPos(hdwp,
                    windowHandle,
                    NULL,
                    0,
                    0,
                    width,
                    height,
                    SWP_NOZORDER);
                EndDeferWindowPos(hdwp);
                return 0;
            }
        }
    }
    return -1;
}

void SimpleHandler::RegisterCallbackObj(SimpleHandleEvent* cb) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
    mSimpleHandleEventCbList.push_back(cb);
}

void SimpleHandler::RemoveCallbackObj(SimpleHandleEvent* cb) {
   std::unique_lock<std::mutex> lock(mEventCbMutex);
   std::list<SimpleHandleEvent*>::iterator iter = mSimpleHandleEventCbList.begin();
   while (iter != mSimpleHandleEventCbList.end()) {
      if ((*iter) == cb) {
         mSimpleHandleEventCbList.erase(iter);
         return;
      }
      iter++;
   }
}

bool SimpleHandler::Execute(const CefString& name,  /*JavaScript调用的C++方法名字*/
   CefRefPtr<CefV8Value> object,					/*JavaScript调用者对象*/
   const CefV8ValueList& arguments,				/*JavaScript传递的参数*/
   CefRefPtr<CefV8Value>& retval,				/*返回给JS的值设置给这个对*/
   CefString& exception)/*通知异常信息给JavaScript*/ {
   int a = 0;
   a = 0;
   return true;
}


void SimpleHandler::AppCallJsFunction(const int web_id, const char* js_function) {
   if (js_function) {
      BrowserList::const_iterator it = browser_list_.begin();
      for (; it != browser_list_.end(); ++it) {
         if ((*it)->GetIdentifier() == web_id) {
            CefRefPtr<CefFrame> frame = (*it)->GetMainFrame();
            frame->ExecuteJavaScript(js_function, frame->GetURL(), 0);
            return;
         }
      }
   }
}