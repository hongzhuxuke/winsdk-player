#pragma once
#include "VHWebView.h"
#include "simple_app.h"
#include "include/cef_request_context_handler.h"

using namespace vhall_paas;

class ClientRequestContextHandler : public CefRequestContextHandler {
public:
   ClientRequestContextHandler();
   ~ClientRequestContextHandler();

   virtual void AddRef() const;

   ///
   // Called to decrement the reference count for the object. Returns true if
   // the reference count is 0, in which case the object should self-delete.
   ///
   virtual bool Release() const;

   ///
   // Returns true if the reference count is 1.
   ///
   virtual bool HasOneRef() const;

   ///
   // Returns true if the reference count is at least 1.
   ///
   virtual bool HasAtLeastOneRef() const;


   virtual bool OnBeforePluginLoad(const CefString& mime_type,
      const CefString& plugin_url,
      bool is_main_frame,
      const CefString& top_origin_url,
      CefRefPtr<CefWebPluginInfo> plugin_info,
      PluginPolicy* plugin_policy) OVERRIDE;
private:
   mutable base::AtomicRefCount ref_count_;
};

class VHWebViewImpl: public VHWebViewInterface ,public BrowserEventListen
{
public:
    VHWebViewImpl();
    virtual ~VHWebViewImpl();
    /*创建WebView成功时的回调。
    *参数：id 对应创建webview的id, url 创建webview时对应加载的url地址。
    *用户根据返回的id，可操作指定的webview*/
    virtual void OnHandleCreateWebView(int &id, const char* url);
    /*webview销毁事件通知*/
    virtual void OnHandleWebViewDestoryed(int &id);

    virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg);

    virtual void OnTitleChanged(const int id, std::string title_name);
    /**
    * 初始化.
    * eventListener: 事件回调监听对象
    * logPath: 日志存储路径
    * 返回值：0 成功，-1失败
    */
    virtual int InitlibCef();
    /*
    * 创建并获取一个webview;
    * 参数：hwnd 父窗口句柄
    */
    virtual int CreateWebView(VHWebEventCb* cb, void* hwnd, int wnd_width, int wnd_height, const std::string url);
    /**
    * 重新加载url.
    **/
    virtual int LoadUrl(const int id, const char* url);
    /**
    *  刷新WebView
    */
    virtual int Reload(const int id);
    /**
    * 应用层调用JS.
    **/
    virtual int AppCallJsFunction(const int web_id, const char* js_function);
    /*
    *  修改webview大小
    */
    virtual void ChangeWebViewSize(const int id, int width, int height);
    /*
    * 关闭指定webview
    */
    virtual void CloseWebView(const int id);
    /*
    * 关闭所有WebView
    */
    virtual void CloseAll();

    virtual void SetProxy(bool enable, const char* proxy_server, const char* proxy_user, const char* proxy_pwd);

private:
    CefRefPtr<SimpleApp> mApp;
    VHWebEventCb* mVHWebEventCbPtr;
    bool bInit;
    CefRefPtr<SimpleHandler> mHandler = nullptr;
};

