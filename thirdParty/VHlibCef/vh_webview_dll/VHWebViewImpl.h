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
    /*����WebView�ɹ�ʱ�Ļص���
    *������id ��Ӧ����webview��id, url ����webviewʱ��Ӧ���ص�url��ַ��
    *�û����ݷ��ص�id���ɲ���ָ����webview*/
    virtual void OnHandleCreateWebView(int &id, const char* url);
    /*webview�����¼�֪ͨ*/
    virtual void OnHandleWebViewDestoryed(int &id);

    virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg);

    virtual void OnTitleChanged(const int id, std::string title_name);
    /**
    * ��ʼ��.
    * eventListener: �¼��ص���������
    * logPath: ��־�洢·��
    * ����ֵ��0 �ɹ���-1ʧ��
    */
    virtual int InitlibCef();
    /*
    * ��������ȡһ��webview;
    * ������hwnd �����ھ��
    */
    virtual int CreateWebView(VHWebEventCb* cb, void* hwnd, int wnd_width, int wnd_height, const std::string url);
    /**
    * ���¼���url.
    **/
    virtual int LoadUrl(const int id, const char* url);
    /**
    *  ˢ��WebView
    */
    virtual int Reload(const int id);
    /**
    * Ӧ�ò����JS.
    **/
    virtual int AppCallJsFunction(const int web_id, const char* js_function);
    /*
    *  �޸�webview��С
    */
    virtual void ChangeWebViewSize(const int id, int width, int height);
    /*
    * �ر�ָ��webview
    */
    virtual void CloseWebView(const int id);
    /*
    * �ر�����WebView
    */
    virtual void CloseAll();

    virtual void SetProxy(bool enable, const char* proxy_server, const char* proxy_user, const char* proxy_pwd);

private:
    CefRefPtr<SimpleApp> mApp;
    VHWebEventCb* mVHWebEventCbPtr;
    bool bInit;
    CefRefPtr<SimpleHandler> mHandler = nullptr;
};

