// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"

class SimpleHandler;
// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp, public CefRenderProcessHandler {
 public:
  SimpleApp();

  //// CefApp methods:
  //virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
  //    OVERRIDE {
  //  return this;
  //}

  //// CefBrowserProcessHandler methods:
  //virtual void OnContextInitialized() OVERRIDE;

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }


  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     CefRefPtr<CefV8Context> context) OVERRIDE;

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);
  CefRefPtr<SimpleHandler> handler;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
