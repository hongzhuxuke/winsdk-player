// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "simple_handler.h"
#include <mutex>

class BrowserEventListen {
public:
   /*����WebView�ɹ�ʱ�Ļص���
   *������id ��Ӧ����webview��id, url ����webviewʱ��Ӧ���ص�url��ַ��
   *�û����ݷ��ص�id���ɲ���ָ����webview*/
   virtual void OnHandleCreateWebView(int id, const char* url) = 0;
   /*webview�����¼�֪ͨ*/
   virtual void OnHandleWebViewDestoryed(int id) = 0;
   /**��Ϣ����*/
   virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg) = 0;
   /**title �ı� ***/
   virtual void OnTitleChanged(const int id, std::string title_name) = 0;
};

// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp, 
                  public CefBrowserProcessHandler,
                  public SimpleHandleEvent {
public:
   SimpleApp();
   ~SimpleApp();

   // CefApp methods:
   virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
      return this;
   }

   // CefBrowserProcessHandler methods:
   virtual void OnContextInitialized() OVERRIDE;

   virtual void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE;

   //virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
   //   CefRefPtr<CefFrame> frame,
   //   CefRefPtr<CefV8Context> context) OVERRIDE;

   /*����WebView�ɹ�ʱ�Ļص���
    *������id ��Ӧ����webview��id, url ����webviewʱ��Ӧ���ص�url��ַ��
    *�û����ݷ��ص�id���ɲ���ָ����webview
    */
   virtual void OnHandleCreateWebView(int id, const char* url);
   
   /*
   * webview�����¼�֪ͨ
   */
   virtual void OnHandleWebViewDestoryed(int id);

   /**title �ı� ***/
   virtual void OnTitleChanged(const int id, std::string title_name);

   virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg);

   void Close();

   void RegisterBrowserEventListener(BrowserEventListen* obj);
private:
   CefRefPtr<SimpleHandler> mHandler;
   std::mutex mEventCbMutex;
   BrowserEventListen *mBrowserEventListenPtr;
   // Include the default reference counting implementation.
   IMPLEMENT_REFCOUNTING(SimpleApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
