#include "VHWebViewImpl.h"
#include <windows.h>
#include "simple_handler.h"
#include "include/cef_sandbox_win.h"
#include "include/internal/cef_types.h"
#include <mutex>

std::mutex mTaskMutex;
VHWebViewImpl* globalWebView = nullptr;
CefRefPtr<SimpleApp> gApp = nullptr;
bool bInit = false;
#define IMPLEMENT_REFCOUNTING(ClassName)                             \
 public:                                                             \
  void AddRef() const OVERRIDE { ref_count_.AddRef(); }              \
  bool Release() const OVERRIDE {                                    \
    if (ref_count_.Release()) {                                      \
      delete static_cast<const ClassName*>(this);                    \
      return true;                                                   \
    }                                                                \
    return false;                                                    \
  }                                                                  \
  bool HasOneRef() const OVERRIDE { return ref_count_.HasOneRef(); } \
  bool HasAtLeastOneRef() const OVERRIDE {                           \
    return ref_count_.HasAtLeastOneRef();                            \
  }                                                                  \
                                                                     \
 private:                                                            \
  CefRefCount ref_count_

class Handler : public CefRequestContextHandler {
public:
   Handler() {}

private:
   IMPLEMENT_REFCOUNTING(Handler);
};


ClientRequestContextHandler::ClientRequestContextHandler(): ref_count_(0) {

}
ClientRequestContextHandler::~ClientRequestContextHandler() {

}

bool ClientRequestContextHandler::OnBeforePluginLoad(const CefString& mime_type,
   const CefString& plugin_url,
   bool is_main_frame,
   const CefString& top_origin_url,
   CefRefPtr<CefWebPluginInfo> plugin_info,
   PluginPolicy* plugin_policy) {
   //*plugin_policy = PLUGIN_POLICY_ALLOW;
    return true;
}

///
 // Increment the reference count.
 ///
void ClientRequestContextHandler::AddRef() const { base::AtomicRefCountInc(&ref_count_); }

///
// Decrement the reference count. Returns true if the reference count is 0.
///
bool ClientRequestContextHandler::Release() const { return !base::AtomicRefCountDec(&ref_count_); }

///
// Returns true if the reference count is 1.
///
bool ClientRequestContextHandler::HasOneRef() const { return base::AtomicRefCountIsOne(&ref_count_); }

///
// Returns true if the reference count is at least 1.
///
bool ClientRequestContextHandler::HasAtLeastOneRef() const {
   return !base::AtomicRefCountIsZero(&ref_count_);
}


VHWebViewImpl::VHWebViewImpl()
{
}


VHWebViewImpl::~VHWebViewImpl()
{
   if (gApp) {
      gApp->RegisterBrowserEventListener(nullptr);
      //mHandler = SimpleHandler::GetInstance();
      //mApp->Release();
      //delete mApp;
      //mApp = nullptr;
   }
   if (mHandler) {
      mHandler->RemoveCallbackObj(mVHWebEventCbPtr);
   }
   //if (mHandler) {
   //   delete mHandler;
   //   mHandler = nullptr;
   //}
}

int VHWebViewImpl::InitlibCef() {
   if (bInit) {
      return 0;
   }

    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();
    void* sandbox_info = NULL;

    //HINSTANCE hInstance1 = GetModuleHandle(NULL);
    // Provide CEF with command-line arguments.
    CefMainArgs main_args;


    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    //int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
    //if (exit_code >= 0) {
    //    // The sub-process has completed so return here.
    //    return exit_code;
    //}

    // Specify CEF global settings here.
    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = true;
    settings.background_color = 0xFF434343;
    settings.remote_debugging_port = 9663;
    CefString(&settings.locale) = L"zh-CN";
      

    TCHAR exeFullPath[MAX_PATH]; // Full path   
    GetModuleFileName(NULL, exeFullPath, MAX_PATH);
    std::wstring strFullPath = (std::wstring)(exeFullPath);
    int nStart = strFullPath.find_last_of(TEXT("\\"));
    std::wstring strExeName = strFullPath.substr(0, nStart + 1);
    strExeName = strExeName + L"cefsimple.exe";
    cef_string_from_wide(strExeName.c_str(), MAX_PATH, &settings.browser_subprocess_path);

    //cef_string_from_wide(L"G:/3rdCode/cef_binary_build/tests/cefsimple/Debug/cefsimple.exe", MAX_PATH, &settings.browser_subprocess_path);

    // SimpleApp implements application-level callbacks for the browser process.
    // It will create the first browser instance in OnContextInitialized() after
    // CEF has initialized.
    gApp = new SimpleApp;
    if (gApp) {
       gApp->RegisterBrowserEventListener(this);
    }
    // Initialize CEF.
    bool bRet = CefInitialize(main_args, settings, gApp.get(), sandbox_info);
    bInit = true;
    //// Run the CEF message loop. This will block until CefQuitMessageLoop() is called.
    //CefRunMessageLoop();

    //// Shut down CEF.
    //CefShutdown();

    return 0;
}

void VHWebViewImpl::OnHandleCreateWebView(int id, const char* url) {
   if (mVHWebEventCbPtr) {
      mVHWebEventCbPtr->OnHandleCreateWebView(id, url);
   }
}
/*webview销毁事件通知*/
void VHWebViewImpl::OnHandleWebViewDestoryed(int id) {
   if (mVHWebEventCbPtr) {
      mVHWebEventCbPtr->OnHandleWebViewDestoryed(id);
   }
}

void VHWebViewImpl::OnRecvMsg(const int id, std::string msg_name ,std::string msg) {
   if (mVHWebEventCbPtr) {
      mVHWebEventCbPtr->OnRecvMsg(id, msg_name, msg);
   }
}

void VHWebViewImpl::OnTitleChanged(const int id, std::string title_name) {
   if (mVHWebEventCbPtr) {
      mVHWebEventCbPtr->OnTitleChanged(id, title_name);
   }
}

/*
* 创建并获取一个webview;
* 参数：hwnd 父窗口句柄
*/
int VHWebViewImpl::CreateWebView(SimpleHandleEvent* cb, void* hwnd, int wnd_width, int wnd_height, const std::string loadUrl) {
    mVHWebEventCbPtr = cb;
    mHandler = new SimpleHandler(true, loadUrl);//SimpleHandler::GetInstance();
    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;
    browser_settings.plugins = STATE_ENABLED;

    // Check if a "--url=" value was provided via the command-line. If so, use
    // that instead of the default URL.
    CefString url = loadUrl;

    RECT windowRect;
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = wnd_width;
    windowRect.bottom = wnd_height;

    // Information used when creating the native window.
    CefWindowInfo window_info;
    window_info.SetAsChild(reinterpret_cast<HWND>(hwnd), windowRect);

    if (mHandler) {
       mHandler->RegisterCallbackObj(mVHWebEventCbPtr);
    }

    //CefRequestContextSettings rcsettings;
    //CefString(&rcsettings.cache_path).FromWString(L"C:\\Users\\Documents\\");
    //auto request_content = CefRequestContext::CreateContext(rcsettings, mHandler);
    //CefString error;
    //CefRefPtr<CefValue> value = CefValue::Create();
    //value->SetInt(1);
    //request_content->SetPreference("profile.default_content_setting_values.plugins", value, error);


    CefBrowserHost::CreateBrowser(window_info,
       mHandler,
       url,
       browser_settings,
       nullptr,
       nullptr);

    return 0;
}

/**
* 重新加载url.
**/
int VHWebViewImpl::LoadUrl(const int id, const char* url) {
    if (mHandler) {
       mHandler->LoadUrl(id, url);
    }
    return 0;
}


void VHWebViewImpl::SetProxy(bool enable, const char* proxy_server, const char* proxy_user, const char* proxy_pwd) {
   //mHandler = SimpleHandler::GetInstance();
   if (mHandler) {
      mHandler->SetProxy(enable, proxy_server, proxy_user, proxy_pwd);
   }
}

/**
* 应用层调用JS.
**/
int VHWebViewImpl::AppCallJsFunction(const int web_id, const char* js_function) {
   if (mHandler) {
      mHandler->AppCallJsFunction(web_id, js_function);
   }
   return 0;
}


/**
*  刷新WebView
*/
int VHWebViewImpl::Reload(const int id) {
    if (mHandler) {
       mHandler->Reload(id);
    }
    return 0;
}
/*
*  修改webview大小
*/
void VHWebViewImpl::ChangeWebViewSize(const int id, int width, int height) {
    if (mHandler) {
       mHandler->ChangeWebViewSize(id, width, height);
    }
}

void VHWebViewImpl::CloseWebView(const int id) {
    if (mHandler) {
       mHandler->CloseBrowser(id);
    }
}

/*
* 关闭所有WebView
*/
void VHWebViewImpl::CloseAll() {
    if (gApp) {
       gApp->Close();
    }
}



namespace vhall_paas {
    VHWebViewInterface* GetVHWebViewObject() {
        std::unique_lock<std::mutex> lock(mTaskMutex);
        //if (globalWebView == nullptr) {
        //   globalWebView = new VHWebViewImpl();
        //}
        //return globalWebView;
         return new VHWebViewImpl();
    }
    void DestoryVHWebViewObject(VHWebViewInterface* obj) {
       if (obj) {
          delete obj;
          obj = nullptr;
        }
    }
}
