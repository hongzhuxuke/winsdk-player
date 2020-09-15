#pragma once
#ifdef  VH_WEBVIEW_EXPORT
#define VH_WEBVIEW_EXPORT     __declspec(dllimport)
#else
#define VH_WEBVIEW_EXPORT     __declspec(dllexport)
#endif

#include <string>
#include <windows.h>
#include "simple_handler_cb.h"




namespace vhall_paas {

    //class VHWebEventCb {
    //public:
    //    /*创建WebView成功时的回调。
    //    *参数：id 对应创建webview的id, url 创建webview时对应加载的url地址。
    //    *用户根据返回的id，可操作指定的webview*/
    //    virtual void OnCreateWebView(int id, const char* url) = 0;
    //    /*webview销毁事件通知*/       
    //    virtual void OnWebViewDestoryed(int id) = 0;

    //    virtual void OnRecvMsg(const int id, std::string fun_name, std::string msg) = 0;

    //    virtual void OnTitleChanged(const int id, std::string title_name) = 0;
    //};


    class VHWebViewInterface
    {
    public:

       VHWebViewInterface() {};

       virtual ~VHWebViewInterface() {};
        /**
        * 初始化。
        * 返回值：0 成功，-1失败
        */
       virtual int InitlibCef() = 0;
        /*
        * 创建并获取一个webview;
        * 参数：hwnd 父窗口句柄
        */
        virtual int CreateWebView(SimpleHandleEvent* cb, void* hwnd, int wnd_width, int wnd_height, const std::string url) = 0;
        /**
        * 重新加载url.
        **/
        virtual int LoadUrl(const int id, const char* url) = 0;
        /**
        * 应用层调用JS.
        **/
        virtual int AppCallJsFunction(const int web_id,const char* js_function) = 0;
        /**
        *  刷新WebView
        */
        virtual int Reload(const int id) = 0;
        /*
        *  修改webview大小
        */
        virtual void ChangeWebViewSize(const int id, int width, int height) = 0;
        /*
        * 关闭指定webview        
        */
        virtual void CloseWebView(const int id) = 0;
        /*
        * 关闭所有WebView
        */
        virtual void CloseAll() = 0;
        /*
         * http 代理；
         * proxy_server： server:port
         */
        virtual void SetProxy(bool enable, const char* proxy_server, const char* proxy_user, const char* proxy_pwd) = 0;
    };

    VH_WEBVIEW_EXPORT VHWebViewInterface* GetVHWebViewObject();
    VH_WEBVIEW_EXPORT void DestoryVHWebViewObject(VHWebViewInterface* obj);
}