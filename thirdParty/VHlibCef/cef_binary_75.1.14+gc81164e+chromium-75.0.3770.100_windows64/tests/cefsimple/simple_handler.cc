// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "tests/cefsimple/simple_handler.h"

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

SimpleHandler::SimpleHandler(bool use_views)
    : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

SimpleHandler::~SimpleHandler() {
  g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
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
}

void SimpleHandler::SetUrl() {
   std::list<CefRefPtr<CefBrowser>>::iterator iter = browser_list_.begin();
   while (iter != browser_list_.end()) {
      iter->get()->GetMainFrame()->LoadURL(CefString("http://e.vhall.com/room/assitant/755760929?assistantType=doc&assistant_token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6IjE4NDYxNjgxIiwicGhvbmUiOiIxMzU1MjE0NDA2MCIsIm5pY2tfbmFtZSI6Ilx1OTBiMVx1NWI4NzExMTExIn0.IiHoFHjc65jfDM9XLHs6oQxP4xZtokGNuJNC-D9oR_o&hide=1"));
      iter++;
   }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool SimpleHandler::Execute(const CefString& name,
   CefRefPtr<CefV8Value> object,
   const CefV8ValueList& arguments,
   CefRefPtr<CefV8Value>& retval,
   CefString& exception) {
   
   if (name == "JsCallQtMsg" || name == "JsCallQtOpenUrl" || name == "JsCallQtStartLive" ||
      name == "JSCallQtUserOnline" || name == "JSCallQtUserOffline" || name == "JsCallQtShareTo" ||
      name == "JsCallQtStartVoiceLive" || name == "JsCallQtMsg") {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(name);
      if (arguments.size() > 0) {
         // Retrieve the argument list object.
         CefRefPtr<CefListValue> args = msg->GetArgumentList();
         CefString dataValue = arguments.at(0)->GetStringValue();
         // Populate the argument values.
         args->SetSize(1);
         args->SetString(0, dataValue);
         // Send the process message to the browser process.
         CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_BROWSER, msg);
      }
   }
   else if (name == "JsCallQtJoinActivity" || name == "ClientCallCloseExamCallback" 
      || name ==  "sendQtCloseSuccess"|| name == "sendQtStopAnswer") {
      CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("JsCallQtJoinActivity");
      CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_BROWSER, msg);
   }

   return true;
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
  frame->LoadString(ss.str(), failedUrl);
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
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}
